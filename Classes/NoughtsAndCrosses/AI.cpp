//
//  AI.cpp
//  NoughtAndCrossesCocos2dx
//
//  Created by Volodymyr Dyachenko on 4/9/17.
//
//

#include "AI.h"

#include <vector>

using std::vector;


namespace  ttt
{
namespace ai
{
 
    
struct Move
{
    int index;
    int score;
    
    Move(){};
    
    Move(int idx, int scr)
    :index(idx), score(scr)
    {
    }
};

Move minmax(Field& field, Mark player);
    
void makeMove(Field& field, Mark mark)
{
    auto move = minmax(field, mark);
    field.doMove(move.index, mark);
}
    

Move minmax(Field& field, Mark player)
{
    if(field.checkWin(Mark::kCross))
        return Move(0, 10);
    if(field.checkWin(Mark::kNought))
        return Move(0, -10);
    
    auto empties = field.getEmpties();
    if(empties.empty())
        return Move(0,0);
    
    vector<Move> movies;
    
    for(int index : empties)
    {
        Move move;
        move.index = index;
        field.setMark(index, player);
        move.score = minmax(field, player == Mark::kCross ? Mark::kNought : Mark::kCross).score;
        field.setMark(index, Mark::kFree);
        movies.push_back(move);
    }
    
    Move best;
    auto compFunc = [](const Move& m1, const Move& m2)
    {
        return m1.score < m2.score;
    };
    
    if(player == Mark::kCross)
    {
        best = *std::max_element(std::begin(movies), std::end(movies), compFunc);
    }
    else
    {
        best = *std::min_element(std::begin(movies), std::end(movies), compFunc);
    }
    
    movies.erase(std::remove_if(std::begin(movies), std::end(movies), [&best](const Move& move) { return move.score != best.score; }), std::end(movies));
    
    return movies[rand() % movies.size()];
}
    
}
}
