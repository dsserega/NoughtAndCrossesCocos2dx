//
//  MainMenuScene.cpp
//  NoughtAndCrossesCocos2dx
//
//  Created by Volodymyr Dyachenko on 4/9/17.
//
//

#include "MainMenuScene.h"

#include "NoughtsAndCrossesScene.h"
#include "SimpleAudioEngine.h"

#include "ui/CocosGUI.h"

#include <functional>

USING_NS_CC;

using namespace std::placeholders;

static const char defaultFont[] = "fonts/arial.ttf";

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

MainMenuScene* MainMenuScene::create()
{
    MainMenuScene* pRet = new (std::nothrow) MainMenuScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    if ( !LayerColor::initWithColor( Color4B(21, 84, 82, 255) ))
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 screenCenter = origin + visibleSize/2;
    
    auto title = Label::createWithTTF("Noughts and crosses", defaultFont, 60);
    title->setColor(Color3B::YELLOW);
    title->setPosition(screenCenter + Vec2(0.0f, visibleSize.height/4.0f));
    addChild(title);

    auto subTitle = Label::createWithTTF("(Tic tac toe)", defaultFont, 30);
    subTitle->setColor(Color3B::YELLOW);
    subTitle->setPosition(screenCenter + Vec2(0.0f, visibleSize.height/8.0f));
    addChild(subTitle);
    
    
    
    _menu = Menu::create();
    _menu->setPosition(Vec2::ZERO);
    addChild(_menu);

    static const char* exit_name = "res/exit_button.png";
    auto closeItem = MenuItemImage::create(
                                           exit_name,
                                           exit_name,
                                           std::bind(&MainMenuScene::onClose, this, _1));
    
    closeItem->setPosition(Vec2(origin.x + 16, origin.y + 16));
    closeItem->setAnchorPoint(Vec2(0.0f, 0.0f));
    
    _menu->addChild(closeItem);
    
    static const char* play_name = "res/play_button.png";
    auto playItem = MenuItemImage::create(
                                           play_name,
                                           play_name,
                                           std::bind(&MainMenuScene::onPlay, this, _1));
    
    playItem->setPosition(Vec2(origin.x + visibleSize.width - 16, origin.y + 16));
    playItem->setAnchorPoint(Vec2(1.0f, 0.0f));
    
    _menu->addChild(playItem);
    
    // Player / AI
    
    _crossesPlayerLabel = createPlayerSelection(
        Vec2(screenCenter.x-visibleSize.width/4, screenCenter.y-visibleSize.height/16),
        "Crosses:",
        Color3B::GREEN,
        _crossesAI
    );

    _noughtsPlayerLabel = createPlayerSelection(
        Vec2(screenCenter.x+visibleSize.width/4, screenCenter.y-visibleSize.height/16),
        "Nougths:",
        Color3B::RED,
        _noughtsAI
    );

    updateTexts();
    
    return true;
}

Label* MainMenuScene::createPlayerSelection(Vec2 playerPos, const char* markText, Color3B color, bool& aiFlag)
{
    auto markLabel = Label::createWithTTF(markText, defaultFont, 30);
    markLabel->setPosition(playerPos);
    markLabel->setColor(color);
    
    addChild(markLabel);
    
    playerPos.y -= markLabel->getContentSize().height * 1.2f;
    
    auto playerLabel = Label::createWithTTF("", defaultFont, 30);
    playerLabel->setPosition(playerPos);
    playerLabel->setColor(color);
    
    addChild(playerLabel);

    const char buttonFilename[] = "res/change_button.png";
    playerPos.y -= markLabel->getContentSize().height * 1.1f;
    auto playerButton = MenuItemImage::create(
        buttonFilename,
        buttonFilename,
        [this, &aiFlag](Ref*)
        {
            aiFlag = !aiFlag;
            updateTexts();
        }
    );
    
    playerButton->setPosition(playerPos);
    playerButton->setAnchorPoint(Vec2(0.5f, 1.0f));
    
    _menu->addChild(playerButton);
    
    return playerLabel;
}

void MainMenuScene::updateTexts()
{
    _crossesPlayerLabel->setString(getTextForAIFlag(_crossesAI));
    _noughtsPlayerLabel->setString(getTextForAIFlag(_noughtsAI));
}

const char* MainMenuScene::getTextForAIFlag(bool flag) const
{
    return flag ? "AI" : "Player";
}

void MainMenuScene::onPlay(cocos2d::Ref* sender)
{
    auto scene = NoughtsAndCrossesScene::createScene(_crossesAI, _noughtsAI);
    Director::getInstance()->replaceScene(scene);
}


void MainMenuScene::onClose(Ref* sender)
{
    //exit
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
