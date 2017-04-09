//
//  Field.hpp
//  NoughtAndCrossesCocos2dx
//
//  Created by Volodymyr Dyachenko on 4/7/17.
//
//

#ifndef Field_hpp
#define Field_hpp

#include <vector>
#include <array>
#include <functional>

namespace ttt //from american name of this game: Tic Tac Toe
{

enum class Mark
{
    kFree,
    kNought,
    kCross
};

class Field
{
public:
    static constexpr int kLineSize = 3;
    static constexpr int kFieldSize = kLineSize * kLineSize;
    
    std::function<void(int index)> onChanged;
    
    Field();
    
    void setMark(int index, Mark mark);
    Mark getMark(int index) const;
    Mark getMark(int j, int i) const { return getMark(j * kLineSize + i); }
    
    void doMove(int index, Mark mark);
    
    bool checkWin(Mark mark) const;
    bool isAnyMovies() const;
    
    std::vector<int> getEmpties() const;
private:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    std::array<Mark, kFieldSize> _field;
    
    void checkIndex(int index) const;
};

}
#endif /* Field_hpp */
