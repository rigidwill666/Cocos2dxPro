#include "Gamecontroller.h"
#include "Gamescene.h"
#include "json/document.h"
#include "Gameconfig.h"
#include "Cardview.h"
#include "Card.h"

USING_NS_CC;

// 初始化静态实例指针
GameController* GameController::_instance = nullptr;

GameController* GameController::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new GameController();
    }
    return _instance;
}

void GameController::destroyInstance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

GameController::GameController()
    : _gameScene(nullptr)
    , _gameState(nullptr)
    , _currentLevelId("")
{
    // 构造函数
}

GameController::~GameController()
{
    // 析构函数
}

void GameController::setGameScene(GameScene* scene)
{
    _gameScene = scene;
}

void GameController::setGameState(GameState* state)
{
    _gameState = state;
}

bool GameController::startGame(const std::string& levelId)
{
    if (_gameScene == nullptr || _gameState == nullptr)
    {
        CCLOG("GameController::startGame - Game scene or state not set");
        return false;
    }

    _currentLevelId = levelId;
    std::string levelFile = levelId + ".json";

    // 加载关卡配置
    if (!_loadLevelConfig(levelFile))
    {
        CCLOG("Failed to load level: %s", levelId.c_str());
        return false;
    }

    return true;
}

bool GameController::_loadLevelConfig(const std::string& levelFile)
{
    return _gameScene->loadLevelConfig(levelFile);
}


void GameController::onDrawCard(Card* clickedCard, const cocos2d::Vec2& clickPosition)
{
    if (_gameState == nullptr || clickedCard == nullptr)
        return;

    // 保存当前状态以便撤销
    _saveGameStateForUndo();

    // 获取当前底牌
    Card* currentBottomCard = _gameState->getBottomCard();

    // 1. 从备用牌堆中移除被点击的牌
    _gameState->replaceReserveCard(clickedCard, currentBottomCard);

    // 2. 设置新的底牌
    _gameState->setBottomCard(clickedCard);

    // 3. 确保卡牌正面朝上
    if (!clickedCard->isFaceUp()) {
        clickedCard->flip();
    }

    // 4. 更新界面（通过参数true指示需要播放动画）
    _gameScene->updateUI(true, clickPosition);
}

bool GameController::canPlaceOnBottomPile(Card* card)
{
    Card* bottomCard = _gameState->getBottomCard();

    if (!bottomCard) return true;
    // 检查点数差是否为1（无花色要求）
    int cardValue = card->getValue();
    int bottomValue = bottomCard->getValue();

    return std::abs(cardValue - bottomValue) == 1;
}

void GameController::onCardClicked(Card* card, const cocos2d::Vec2& clickPosition)
{
    if (_gameState == nullptr || card == nullptr)
        return;

    // 如果牌是背面朝上或被阻塞，则不能点击
    if (!card->isFaceUp() || card->isBlocked())
        return;

    // 保存当前状态以便撤销
    _saveGameStateForUndo();

    // 检查点击的牌是在主牌区(桌面牌)
    bool isMainPileCard = false;
    for (auto mainCard : _gameState->getMainPileCards())
    {
        if (mainCard == card)
        {
            isMainPileCard = true;
            break;
        }
    }

    if (isMainPileCard)
    {
        // 需求：桌面牌和底牌区顶部牌匹配
        // 桌面牌点击 - 检查是否可以与底牌匹配
        if (canPlaceOnBottomPile(card))
        {
            card->retain();
            // 从主牌区移除卡牌
            _gameState->removeMainPileCard(card);

            // 将卡牌添加到底牌区（作为新的顶部卡牌）
            _gameState->addCardToBottomPile(card);

            card->release();

            // 更新UI，包括动画（从点击位置到底牌区）
            _gameScene->updateUI(true, clickPosition);

            return;
        }
    }
}

void GameController::_moveCardToBottomPile(Card* card)
{
    if (_gameState == nullptr || card == nullptr)
        return;

    // 保存当前底牌
    Card* oldBottomCard = _gameState->getBottomCard();

    // 从主牌区移除该卡牌
    _gameState->removeMainPileCard(card);

    // 设置为新的底牌
    _gameState->setBottomCard(card);

    // 如果有旧底牌，将其放入主牌区
    if (oldBottomCard)
    {
        float x = GameConfig::SCREEN_WIDTH / 2 + (rand() % 200 - 100);
        float y = GameConfig::BOTTOM_AREA_HEIGHT + GameConfig::MAIN_AREA_HEIGHT / 2 + (rand() % 200 - 100);
        _gameState->addMainPileCard(oldBottomCard, Vec2(x, y));
    }
}

void GameController::undo()
{
    if (_gameState == nullptr)
        return;

    if (_gameState->undo())
    {
        // 撤销成功，更新UI（带动画）
        _gameScene->updateUI(true);
    }
}

void GameController::_saveGameStateForUndo()
{
    if (_gameState == nullptr)
        return;

    // 创建当前游戏状态的副本并保存到撤销栈
    _gameState->saveStateForUndo();
}