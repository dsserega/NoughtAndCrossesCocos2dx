//
//  Field.cpp
//  NoughtAndCrossesCocos2dx
//
//  Created by Volodymyr Dyachenko on 4/7/17.
//
//

#include "Field.h"

#include <algorithm>
#include <exception>
#include <map>
#include <cstdlib>

namespace ttt
{
    

using std::vector;

static const char kOutOfIndexError[] = "Out of index";

Field::Field()
{
    std::fill(std::begin(_field), std::end(_field), Mark::kFree);
}

void Field::checkIndex(int index) const
{
    if(index <0 || index >=_field.size())
        throw std::runtime_error(kOutOfIndexError);
}

void Field::setMark(int index, Mark mark)
{
    checkIndex(index);
    _field[index] = mark;
}

Mark Field::getMark(int index) const
{
    checkIndex(index);
    return _field[index];
}

void Field::doMove(int index, Mark mark)
{
    if(_field[index] != Mark::kFree)
        return;
    setMark(index, mark);
    
    onChanged(index);
}

bool Field::checkWin(Mark mark) const
{
    static const int lines [][3] =
    {
        //horisontal
        {0,1,2},
        {3,4,5},
        {6,7,8},
        //vertical
        {0,3,6},
        {1,4,7},
        {2,5,8},
        //diagonals
        {0,4,8},
        {6,4,2}
    };
    
    for(const auto& line : lines)
    {
        bool win= std::all_of(std::begin(line), std::end(line), [this, mark](const int& index)
                              {
                                  return getMark(index) == mark;
                              } );
        
        if(win)
            return true;
    }
    
    return false;
}

vector<int> Field::getEmpties() const
{
    vector<int> empties;
    for(int i = 0; i< kFieldSize; ++i)
        if(_field[i] == Mark::kFree)
            empties.push_back(i);
    
    return empties;
}


bool Field::isAnyMovies() const
{
    return std::any_of(std::begin(_field), std::end(_field), [](Mark mark) { return mark == Mark::kFree; });
}

}
