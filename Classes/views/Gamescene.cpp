#include "Gamescene.h"
#include "Gameconfig.h"
#include "json/document.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 初始化UI元素
    _initUI();

    // 初始化游戏状态
    _initGameState();

    // 初始化事件处理
    _initEvents();

    return true;
}

void GameScene::onExit()
{
    Scene::onExit();  // 调用父类的onExit

    // 清理所有UI组件
    if (_bottomPileView) {
        _bottomPileView->clearAllCards();
    }
    if (_mainPileView) {
        _mainPileView->clearAllCards();
    }
    if (_reservePileView) {
        _reservePileView->clearAllCards();
    }

    // 释放GameState
    if (_gameState) {
        _gameState->release();
        _gameState = nullptr;
    }
}

void GameScene::_initUI()
{
    // 获取屏幕尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建棕色背景
    auto background = LayerColor::create(Color4B(139, 69, 19, 255));
    this->addChild(background, 0);

    // 创建底部紫色区域
    auto bottomArea = LayerColor::create(Color4B(128, 0, 128, 255),
        visibleSize.width, GameConfig::BOTTOM_AREA_HEIGHT);
    bottomArea->setPosition(Vec2(origin.x, origin.y));
    this->addChild(bottomArea, 1);

    // 创建底牌区视图
    _bottomPileView = BottomPileView::create();
    _bottomPileView->setPosition(Vec2(visibleSize.width * 2.6 / 4 + origin.x,
        GameConfig::BOTTOM_AREA_HEIGHT / 2 + origin.y));
    this->addChild(_bottomPileView, 2);

    // 创建主牌区视图
    _mainPileView = MainPileView::create();
    _mainPileView->setPosition(Vec2(origin.x, GameConfig::BOTTOM_AREA_HEIGHT + origin.y));
    this->addChild(_mainPileView, 2);

    // 创建备用牌堆视图
    _reservePileView = ReservePileView::create();
    _reservePileView->setPosition(Vec2(visibleSize.width / 4 + origin.x,
        GameConfig::BOTTOM_AREA_HEIGHT / 2 + origin.y));
    this->addChild(_reservePileView, 2);

    auto undoLabel = Label::createWithSystemFont("Back", "fonts/SimHei.ttf", 60);
    undoLabel->setPosition(Vec2(visibleSize.width * 3.5 / 4 + origin.x, GameConfig::BOTTOM_AREA_HEIGHT / 2 + origin.y));
    CCLOG("Label position: (%f, %f)", undoLabel->getPositionX(), undoLabel->getPositionY());
    undoLabel->setTextColor(Color4B::WHITE);

    // 添加触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this, undoLabel](Touch* touch, Event* event) {
        auto bounds = undoLabel->getBoundingBox();
        if (bounds.containsPoint(touch->getLocation())) { //判断触摸点是否在undoLabel的边界框内
            this->_onUndoClicked(nullptr);
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, undoLabel);
    this->addChild(undoLabel, 3);
}

void GameScene::_onUndoClicked(cocos2d::Ref* sender)
{
    // 获取游戏控制器实例
    auto controller = GameController::getInstance();
    if (controller)
    {
        // 调用控制器的撤销方法
        controller->undo();
    }
}

void GameScene::_initGameState()
{
    // 创建游戏状态
    _gameState = GameState::create();
    _gameState->retain(); // 增加引用计数，保证不会被自动释放

    // 设置游戏控制器引用
    GameController::getInstance()->setGameScene(this);
    GameController::getInstance()->setGameState(_gameState);

    // 默认关卡
    GameController::getInstance()->startGame("level1");
    //GameScene::loadLevelConfig("level1");

}

void GameScene::_onReservePileClicked(cocos2d::Ref* sender)
{
    // 从Ref*转换为CardClickInfo*
    CardClickInfo* clickInfo = dynamic_cast<CardClickInfo*>(sender);
    if (!clickInfo) return;

    Card* clickedCard = clickInfo->card;
    cocos2d::Vec2 clickPosition = clickInfo->worldPosition;

    if (!clickedCard) return;

    // 获取游戏控制器
    auto controller = GameController::getInstance();
    if (controller)
    {
        // 调用控制器的抽牌方法，传递卡牌和点击位置
        controller->onDrawCard(clickedCard, clickPosition);
    }
}

void GameScene::_onMainPileClicked(cocos2d::Ref* sender)
{
    // 从Ref*转换为CardClickInfo*
    CardClickInfo* clickInfo = dynamic_cast<CardClickInfo*>(sender);
    if (!clickInfo) return;

    Card* clickedCard = clickInfo->card;
    cocos2d::Vec2 clickPosition = clickInfo->worldPosition;

    if (!clickedCard) return;

    // 获取游戏控制器
    auto controller = GameController::getInstance();
    if (controller)
    {
        // 调用控制器的卡牌点击方法，传递卡牌和点击位置
        controller->onCardClicked(clickedCard, clickPosition);
    }
}

void GameScene::_initEvents()
{
    // 备用牌堆点击事件
    _reservePileView->addClickListener(CC_CALLBACK_1(GameScene::_onReservePileClicked, this));
    // 添加主牌区点击事件
    _mainPileView->addClickListener(CC_CALLBACK_1(GameScene::_onMainPileClicked, this));
}

bool GameScene::loadLevelConfig(const std::string& levelFile)
{
    // 清空旧配置
    _levelConfig.clear();

    // 加载关卡文件
    std::string fullPath = "D:\\program_data\\cocos_work\\CardGame\\proj.win32\\level1.json";

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(fullPath);

    // 解析JSON
    rapidjson::Document document;
    document.Parse(jsonStr.c_str());

    if (!document.IsObject()) {
        return false;
    }

    //处理不同牌区
    std::vector<Card*> reserveCards;
    processPlayfieldCards(document);
    processStackCards(document, reserveCards);

    //更新UI
    _gameState->setReservePileCards(reserveCards);
    updateUI(false);

    return true;
}

void GameScene::processPlayfieldCards(const rapidjson::Document& document)
{
    if (!document.HasMember("Playfield") || !document["Playfield"].IsArray()) {
        return;
    }

    const rapidjson::Value& playfield = document["Playfield"];
    for (rapidjson::SizeType i = 0; i < playfield.Size(); ++i) {
        const auto& cardData = playfield[i];
        if (isValidCardData(cardData)) {
            Card* card = createCardFromData(cardData);
            float x = cardData["Position"]["x"].GetFloat();
            float y = cardData["Position"]["y"].GetFloat();
            _gameState->addMainPileCard(card, Vec2(x, y));
        }
    }
}

void GameScene::processStackCards(const rapidjson::Document& document, std::vector<Card*>& reserveCards)
{
    if (!document.HasMember("Stack") || !document["Stack"].IsArray()) {
        return;
    }

    const rapidjson::Value& stack = document["Stack"];
    int total = static_cast<int>(stack.Size());

    for (int i = 0; i < total; ++i) {
        const auto& cardData = stack[i];
        if (cardData.HasMember("CardFace") && cardData.HasMember("CardSuit")) {
            Card* card = createCardFromData(cardData);
            if (i < total - 1) {
                reserveCards.push_back(card);
            }
            else {
                _gameState->setBottomCard(card);
            }
        }
    }
}

bool GameScene::isValidCardData(const rapidjson::Value& cardData)
{
    return cardData.HasMember("CardFace") &&
        cardData.HasMember("CardSuit") &&
        cardData.HasMember("Position") &&
        cardData["Position"].IsObject();
}

Card* GameScene::createCardFromData(const rapidjson::Value& cardData)
{
    CardFaceType face = static_cast<CardFaceType>(cardData["CardFace"].GetInt());
    CardSuitType suit = static_cast<CardSuitType>(cardData["CardSuit"].GetInt());
    Card* card = new Card(face, suit, true);
    card->autorelease();
    return card;
}


void GameScene::updateUI(bool withAnimation, const cocos2d::Vec2& startPosition) {
    if (_gameState == nullptr)
        return;

    // 获取模型数据
    Card* bottomCard = _gameState->getBottomCard();
    std::vector<Card*> mainPileCards = _gameState->getMainPileCards();
    std::vector<cocos2d::Vec2> mainPilePositions = _gameState->getMainPilePositions();
    std::vector<Card*> reservePileCards = _gameState->getReservePileCards();

    // 更新撤销按钮状态
    bool canUndo = _gameState->canUndo();

    if (withAnimation && startPosition != cocos2d::Vec2::ZERO) {
        // 创建一个标记，表示动画是否已经启动
        bool animationStarted = false;

        // 动画完成后的操作
        auto finalUpdateCallback = CallFunc::create([this, bottomCard, mainPileCards, mainPilePositions, reservePileCards]() {
            // 更新底牌区
            _bottomPileView->clearAllCards();
            if (bottomCard) {
                _bottomPileView->setCurrentCard(bottomCard);
            }

            // 更新主牌区
            _mainPileView->clearAllCards();
            if (mainPileCards.size() == mainPilePositions.size()) {
                for (size_t i = 0; i < mainPileCards.size(); ++i) {
                    CardView* cardView = CardView::create(mainPileCards[i]);
                    cardView->setPosition(mainPilePositions[i]);
                    _mainPileView->addCardView(cardView);
                }
            }

            // 更新备用牌堆
            _reservePileView->setCards(reservePileCards);
            });

        // 创建卡牌移动动画
        if (bottomCard) {
            // 创建新的底牌视图用于动画
            CardView* newCardView = CardView::create(bottomCard);

            CCLOG("newcard suit:%d", bottomCard->getValue());


            newCardView->updateView();


            // 计算起始位置
            cocos2d::Vec2 animStartPos = this->convertToNodeSpace(startPosition);

            // 添加到场景
            this->addChild(newCardView, 100); // 非常高的Z顺序确保显示在最上面
            newCardView->setPosition(animStartPos);

            // 移动到底牌区
            cocos2d::Vec2 targetPos = _bottomPileView->getPosition();
            auto moveTo = cocos2d::MoveTo::create(0.3f, targetPos);
            auto cleanupAction = cocos2d::CallFunc::create([newCardView]() {
                newCardView->removeFromParent();
                });

            newCardView->runAction(cocos2d::Sequence::create(moveTo, cleanupAction, finalUpdateCallback, nullptr));
            animationStarted = true;
        }


        // 如果没有启动任何动画，还是需要更新UI
        if (!animationStarted) {
            finalUpdateCallback->execute();
        }
    }
    else {
        // 更新底牌区 - 不带动画
        _bottomPileView->clearAllCards();
        if (bottomCard) {
            _bottomPileView->setCurrentCard(bottomCard);
        }

        // 更新主牌区 - 不带动画
        _mainPileView->clearAllCards();
        if (mainPileCards.size() == mainPilePositions.size()) {
            for (size_t i = 0; i < mainPileCards.size(); ++i) {
                CardView* cardView = CardView::create(mainPileCards[i]);
                cardView->setPosition(mainPilePositions[i]);
                _mainPileView->addCardView(cardView);
            }
        }
        // 更新备用牌堆
        _reservePileView->setCards(reservePileCards);
    }
}