#ifndef __NoughtAndCrossesScene_SCENE_H__
#define __NoughtAndCrossesScene_SCENE_H__

#include "cocos2d.h"

#include "NoughtsAndCrosses/Field.h"

class NoughtsAndCrossesScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene(bool crossesAI, bool noughtsAI);

    virtual bool init(bool crossesAI, bool noughtsAI);
    
    
    // implement the "static create()" method manually
    static NoughtsAndCrossesScene* create(bool crossesAI, bool noughtsAI);
private:
    static constexpr int kCellSize = 192;
    
    ttt::Field _field;
    bool _crossesAI;
    bool _noughtsAI;
    ttt::Mark currentPlayer = ttt::Mark::kCross;
    
    bool _gameEnded = false;
    bool _sceneIsBusy = false;
    cocos2d::Vec2 _screenCenter;
    cocos2d::Menu* _menu;

    void initField();
    cocos2d::Vec2 calcCellPosition(int index) const;
    
    void processNextMove();
    void changeCurrentPlayer();
    void checkGameEnd();
    
    void showMove(int index);

    void onCellClick(Ref* sender);
    void exit();
    
};

#endif // __NoughtAndCrossesScene_SCENE_H__
