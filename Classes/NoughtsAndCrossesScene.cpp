#include "NoughtsAndCrossesScene.h"
#include "SimpleAudioEngine.h"

#include <utility>
#include <functional>

#include "ui/CocosGUI.h"

#include "NoughtsAndCrosses/AI.h"
#include "MainMenuScene.h"

USING_NS_CC;

using std::pair;
using namespace std::placeholders;
using namespace ttt;

constexpr int kFieldOrder = 1;
constexpr int kUIOrder = 2;

Scene* NoughtsAndCrossesScene::createScene(bool crossesAI, bool noughtsAI)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = NoughtsAndCrossesScene::create(crossesAI, noughtsAI);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

NoughtsAndCrossesScene* NoughtsAndCrossesScene::create(bool crossesAI, bool noughtsAI)
{
    NoughtsAndCrossesScene* pRet = new (std::nothrow) NoughtsAndCrossesScene();
    if (pRet && pRet->init(crossesAI, noughtsAI))
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
bool NoughtsAndCrossesScene::init(bool crossesAI, bool noughtsAI)
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor( Color4B(21, 84, 82, 255) ))
    {
        return false;
    }
    
    _crossesAI = crossesAI;
    _noughtsAI = noughtsAI;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _screenCenter = origin + visibleSize/2;
    
    
    static const char exit_name[] = "res/exit_button.png";
    auto closeItem = MenuItemImage::create(
        exit_name,
        exit_name,
        [this](Ref*){ exit(); });

    closeItem->setPosition(Vec2(origin.x + 16, origin.y + 16) + closeItem->getContentSize()/2);

    _menu = Menu::create();
    _menu->setPosition(Vec2::ZERO);
    addChild(_menu, kFieldOrder);
    _menu->addChild(closeItem);
    
    initField();
    
    _field.onChanged = std::bind(&NoughtsAndCrossesScene::showMove, this, _1);
    
    processNextMove();
    
    return true;
}


void NoughtsAndCrossesScene::initField()
{
    //Grid
    static const pair<Vec2, Vec2> linesRelCoords[] =
    {
        {{-0.5f, -1.5f}, {-0.5f, 1.5}},
        {{0.5f, -1.5f}, {0.5, 1.5}},
        {{-1.5f, -0.5f}, {1.5f, -0.5f}},
        {{-1.5f, 0.5f}, {1.5f, 0.5f}},
    };
    
    constexpr float gridWidth = 10.0f;
    
    for(const auto& lineRel : linesRelCoords)
    {
        Rect rect;
        
        Vec2 pos(
                 lineRel.first.x * kCellSize - 5 + _screenCenter.x,
                 lineRel.first.y * kCellSize - 5 + _screenCenter.y
        );
        
        auto lineSprite = cocos2d::ui::Scale9Sprite::create("res/grid.png");
        lineSprite->setPosition(pos);
        lineSprite->setContentSize(Size(
            (lineRel.second.x - lineRel.first.x) * kCellSize + gridWidth,
            (lineRel.second.y - lineRel.first.y) * kCellSize + gridWidth
        ));
        
        lineSprite->setAnchorPoint(Vec2::ZERO);
        
        lineSprite->setScale9Enabled(true);
        
        addChild(lineSprite, kFieldOrder);
    }
    
    for(int i = 0; i< Field::kFieldSize; ++i)
    {
        auto button = MenuItem::create();
        button->setContentSize( {kCellSize, kCellSize} );
        button->setPosition(calcCellPosition(i));
        button->setTag(i);
        button->setCallback(std::bind(&NoughtsAndCrossesScene::onCellClick, this, _1));

        _menu->addChild(button);
    }
}

cocos2d::Vec2 NoughtsAndCrossesScene::calcCellPosition(int index) const
{
    static const Vec2 relativePos[9] =
    {
        { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
        { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f },
        { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }
    };

    return Vec2(relativePos[index] * static_cast<float>(kCellSize) + _screenCenter);
}


void NoughtsAndCrossesScene::onCellClick(Ref* sender)
{
    if( _sceneIsBusy )
        return;
    
    if(_gameEnded)
    {
        exit();
        return;
    }
        
    auto button = dynamic_cast<MenuItem*>(sender);
    if(!button)
        return;
    
    int index = button->getTag();

    if( _field.getMark(index) != Mark::kFree)
        return;
    
    _field.doMove(index, currentPlayer);

    changeCurrentPlayer();
}

void NoughtsAndCrossesScene::checkGameEnd()
{
    const char* message = nullptr;
    
    if(_field.checkWin(Mark::kCross))
    {
        _gameEnded = true;
        message = "Crosses win!";
    }
    else if(_field.checkWin(Mark::kNought))
    {
        _gameEnded = true;
        message = "Noughts win!";
        
    }
    else if(!_field.isAnyMovies())
    {
        _gameEnded = true;
        message = "Tie!";
    }
    
    if(_gameEnded && message)
    {
        auto endGameLabel = Label::createWithTTF(message, "fonts/arial.ttf", 80);
        endGameLabel->setColor(Color3B::WHITE);
        endGameLabel->enableOutline(Color4B::BLACK, 3);
        endGameLabel->setPosition(_screenCenter);

        addChild(endGameLabel, kUIOrder);
        
        _sceneIsBusy = true;

        constexpr float duration1 = 0.3f;
        constexpr float duration2 = 0.2f;
        
        endGameLabel->runAction(Sequence::create(
            EaseSineOut::create(ScaleTo::create(duration1, 1.2f)),
            EaseSineIn::create(ScaleTo::create(duration2, 1.0)), nullptr));
        
        endGameLabel->runAction(Sequence::create(FadeIn::create(duration1+duration2), CallFunc::create([this](){_sceneIsBusy = false;}), nullptr ));
    }
}


void NoughtsAndCrossesScene::processNextMove()
{
    checkGameEnd();
    if(_gameEnded)
        return;
    
    if((!_crossesAI && currentPlayer == Mark::kCross) || (!_noughtsAI && currentPlayer == Mark::kNought))
        return;
    
    ai::makeMove(_field, currentPlayer);
    changeCurrentPlayer();
    
}

void NoughtsAndCrossesScene::showMove(int index)
{
    const char *filename = currentPlayer == Mark::kCross ? "res/cross.png" : "res/nought.png";
    Sprite* mark = Sprite::create(filename);
    
    auto button = _menu->getChildByTag(index);
    //place mark to center of a button
    mark->setPosition(button->getContentSize()/2);
    mark->setScale(0.33f);
    mark->setOpacity(0);
    
    _sceneIsBusy = true;
    constexpr float duration = 0.3f;
    mark->runAction(EaseBackOut::create(ScaleTo::create(duration, 1.0f)));
    mark->runAction(Sequence::create(FadeIn::create(duration), CallFunc::create([this]()
    {
        _sceneIsBusy = false;
        processNextMove();
    }), nullptr ));
    
    button->addChild(mark);
}

void NoughtsAndCrossesScene::changeCurrentPlayer()
{
    currentPlayer = currentPlayer == Mark::kCross ? Mark::kNought : Mark::kCross;
}

void NoughtsAndCrossesScene::exit()
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}
