//
//  MainMenuScene.hpp
//  NoughtAndCrossesCocos2dx
//
//  Created by Volodymyr Dyachenko on 4/9/17.
//
//

#ifndef MainMenuScene_hpp
#define MainMenuScene_hpp

#include "cocos2d.h"


class MainMenuScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    static MainMenuScene* create();
private:
    bool _crossesAI = false;
    bool _noughtsAI = true;

    cocos2d::Menu* _menu;
    cocos2d::Label* _crossesPlayerLabel;
    cocos2d::Label* _noughtsPlayerLabel;
    
    cocos2d::Label* createPlayerSelection(cocos2d::Vec2 playerPos, const char* markText, cocos2d::Color3B color, bool& aiFlag);
    void updateTexts();
    const char* getTextForAIFlag(bool flag) const;
    
    // a selector callback
    void onClose(cocos2d::Ref* sender);
    void onPlay(cocos2d::Ref* sender);

};

#endif /* MainMenuScene_hpp */
