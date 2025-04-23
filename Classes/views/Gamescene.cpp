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

    // ��ʼ��UIԪ��
    _initUI();

    // ��ʼ����Ϸ״̬
    _initGameState();

    // ��ʼ���¼�����
    _initEvents();

    return true;
}

void GameScene::onExit()
{
    Scene::onExit();  // ���ø����onExit

    // ��������UI���
    if (_bottomPileView) {
        _bottomPileView->clearAllCards();
    }
    if (_mainPileView) {
        _mainPileView->clearAllCards();
    }
    if (_reservePileView) {
        _reservePileView->clearAllCards();
    }

    // �ͷ�GameState
    if (_gameState) {
        _gameState->release();
        _gameState = nullptr;
    }
}

void GameScene::_initUI()
{
    // ��ȡ��Ļ�ߴ�
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ������ɫ����
    auto background = LayerColor::create(Color4B(139, 69, 19, 255));
    this->addChild(background, 0);

    // �����ײ���ɫ����
    auto bottomArea = LayerColor::create(Color4B(128, 0, 128, 255),
        visibleSize.width, GameConfig::BOTTOM_AREA_HEIGHT);
    bottomArea->setPosition(Vec2(origin.x, origin.y));
    this->addChild(bottomArea, 1);

    // ������������ͼ
    _bottomPileView = BottomPileView::create();
    _bottomPileView->setPosition(Vec2(visibleSize.width * 2.6 / 4 + origin.x,
        GameConfig::BOTTOM_AREA_HEIGHT / 2 + origin.y));
    this->addChild(_bottomPileView, 2);

    // ������������ͼ
    _mainPileView = MainPileView::create();
    _mainPileView->setPosition(Vec2(origin.x, GameConfig::BOTTOM_AREA_HEIGHT + origin.y));
    this->addChild(_mainPileView, 2);

    // ���������ƶ���ͼ
    _reservePileView = ReservePileView::create();
    _reservePileView->setPosition(Vec2(visibleSize.width / 4 + origin.x,
        GameConfig::BOTTOM_AREA_HEIGHT / 2 + origin.y));
    this->addChild(_reservePileView, 2);

    auto undoLabel = Label::createWithSystemFont("Back", "fonts/SimHei.ttf", 60);
    undoLabel->setPosition(Vec2(visibleSize.width * 3.5 / 4 + origin.x, GameConfig::BOTTOM_AREA_HEIGHT / 2 + origin.y));
    CCLOG("Label position: (%f, %f)", undoLabel->getPositionX(), undoLabel->getPositionY());
    undoLabel->setTextColor(Color4B::WHITE);

    // ��Ӵ����¼�
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this, undoLabel](Touch* touch, Event* event) {
        auto bounds = undoLabel->getBoundingBox();
        if (bounds.containsPoint(touch->getLocation())) { //�жϴ������Ƿ���undoLabel�ı߽����
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
    // ��ȡ��Ϸ������ʵ��
    auto controller = GameController::getInstance();
    if (controller)
    {
        // ���ÿ������ĳ�������
        controller->undo();
    }
}

void GameScene::_initGameState()
{
    // ������Ϸ״̬
    _gameState = GameState::create();
    _gameState->retain(); // �������ü�������֤���ᱻ�Զ��ͷ�

    // ������Ϸ����������
    GameController::getInstance()->setGameScene(this);
    GameController::getInstance()->setGameState(_gameState);

    // Ĭ�Ϲؿ�
    GameController::getInstance()->startGame("level1");
    //GameScene::loadLevelConfig("level1");

}

void GameScene::_onReservePileClicked(cocos2d::Ref* sender)
{
    // ��Ref*ת��ΪCardClickInfo*
    CardClickInfo* clickInfo = dynamic_cast<CardClickInfo*>(sender);
    if (!clickInfo) return;

    Card* clickedCard = clickInfo->card;
    cocos2d::Vec2 clickPosition = clickInfo->worldPosition;

    if (!clickedCard) return;

    // ��ȡ��Ϸ������
    auto controller = GameController::getInstance();
    if (controller)
    {
        // ���ÿ������ĳ��Ʒ��������ݿ��ƺ͵��λ��
        controller->onDrawCard(clickedCard, clickPosition);
    }
}

void GameScene::_onMainPileClicked(cocos2d::Ref* sender)
{
    // ��Ref*ת��ΪCardClickInfo*
    CardClickInfo* clickInfo = dynamic_cast<CardClickInfo*>(sender);
    if (!clickInfo) return;

    Card* clickedCard = clickInfo->card;
    cocos2d::Vec2 clickPosition = clickInfo->worldPosition;

    if (!clickedCard) return;

    // ��ȡ��Ϸ������
    auto controller = GameController::getInstance();
    if (controller)
    {
        // ���ÿ������Ŀ��Ƶ�����������ݿ��ƺ͵��λ��
        controller->onCardClicked(clickedCard, clickPosition);
    }
}

void GameScene::_initEvents()
{
    // �����ƶѵ���¼�
    _reservePileView->addClickListener(CC_CALLBACK_1(GameScene::_onReservePileClicked, this));
    // �������������¼�
    _mainPileView->addClickListener(CC_CALLBACK_1(GameScene::_onMainPileClicked, this));
}

bool GameScene::loadLevelConfig(const std::string& levelFile)
{
    // ��վ�����
    _levelConfig.clear();

    // ���عؿ��ļ�
    std::string fullPath = "D:\\program_data\\cocos_work\\CardGame\\proj.win32\\level1.json";

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(fullPath);

    // ����JSON
    rapidjson::Document document;
    document.Parse(jsonStr.c_str());

    if (!document.IsObject()) {
        return false;
    }

    //����ͬ����
    std::vector<Card*> reserveCards;
    processPlayfieldCards(document);
    processStackCards(document, reserveCards);

    //����UI
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

    // ��ȡģ������
    Card* bottomCard = _gameState->getBottomCard();
    std::vector<Card*> mainPileCards = _gameState->getMainPileCards();
    std::vector<cocos2d::Vec2> mainPilePositions = _gameState->getMainPilePositions();
    std::vector<Card*> reservePileCards = _gameState->getReservePileCards();

    // ���³�����ť״̬
    bool canUndo = _gameState->canUndo();

    if (withAnimation && startPosition != cocos2d::Vec2::ZERO) {
        // ����һ����ǣ���ʾ�����Ƿ��Ѿ�����
        bool animationStarted = false;

        // ������ɺ�Ĳ���
        auto finalUpdateCallback = CallFunc::create([this, bottomCard, mainPileCards, mainPilePositions, reservePileCards]() {
            // ���µ�����
            _bottomPileView->clearAllCards();
            if (bottomCard) {
                _bottomPileView->setCurrentCard(bottomCard);
            }

            // ����������
            _mainPileView->clearAllCards();
            if (mainPileCards.size() == mainPilePositions.size()) {
                for (size_t i = 0; i < mainPileCards.size(); ++i) {
                    CardView* cardView = CardView::create(mainPileCards[i]);
                    cardView->setPosition(mainPilePositions[i]);
                    _mainPileView->addCardView(cardView);
                }
            }

            // ���±����ƶ�
            _reservePileView->setCards(reservePileCards);
            });

        // ���������ƶ�����
        if (bottomCard) {
            // �����µĵ�����ͼ���ڶ���
            CardView* newCardView = CardView::create(bottomCard);

            CCLOG("newcard suit:%d", bottomCard->getValue());


            newCardView->updateView();


            // ������ʼλ��
            cocos2d::Vec2 animStartPos = this->convertToNodeSpace(startPosition);

            // ��ӵ�����
            this->addChild(newCardView, 100); // �ǳ��ߵ�Z˳��ȷ����ʾ��������
            newCardView->setPosition(animStartPos);

            // �ƶ���������
            cocos2d::Vec2 targetPos = _bottomPileView->getPosition();
            auto moveTo = cocos2d::MoveTo::create(0.3f, targetPos);
            auto cleanupAction = cocos2d::CallFunc::create([newCardView]() {
                newCardView->removeFromParent();
                });

            newCardView->runAction(cocos2d::Sequence::create(moveTo, cleanupAction, finalUpdateCallback, nullptr));
            animationStarted = true;
        }


        // ���û�������κζ�����������Ҫ����UI
        if (!animationStarted) {
            finalUpdateCallback->execute();
        }
    }
    else {
        // ���µ����� - ��������
        _bottomPileView->clearAllCards();
        if (bottomCard) {
            _bottomPileView->setCurrentCard(bottomCard);
        }

        // ���������� - ��������
        _mainPileView->clearAllCards();
        if (mainPileCards.size() == mainPilePositions.size()) {
            for (size_t i = 0; i < mainPileCards.size(); ++i) {
                CardView* cardView = CardView::create(mainPileCards[i]);
                cardView->setPosition(mainPilePositions[i]);
                _mainPileView->addCardView(cardView);
            }
        }
        // ���±����ƶ�
        _reservePileView->setCards(reservePileCards);
    }
}