#include "Gamestate.h"
#include "Gameconfig.h"

USING_NS_CC;

GameState* GameState::create()
{
    GameState* state = new (std::nothrow) GameState();
    if (state && state->init()) {
        state->autorelease();
        return state;
    }
    CC_SAFE_DELETE(state);
    return nullptr;
}

GameState::GameState()
    : _bottomCard(nullptr)
{
}


GameState::~GameState()
{
    // 首先检查节点是否还在运行
    for (auto card : _mainPileCards) {
        if (card && card->isRunning()) {
            card->stopAllActions();  // 停止所有动作
            card->onExit();  // 手动调用onExit
        }
        CC_SAFE_RELEASE(card);
    }
    _mainPileCards.clear();

    if (_bottomCard) {
        if (_bottomCard->isRunning()) {
            _bottomCard->stopAllActions();
            _bottomCard->onExit();
        }
        CC_SAFE_RELEASE(_bottomCard);
        _bottomCard = nullptr;
    }

    for (auto card : _reservePileCards) {
        if (card && card->isRunning()) {
            card->stopAllActions();
            card->onExit();
        }
        CC_SAFE_RELEASE(card);
    }
    _reservePileCards.clear();

    // 清理撤销栈
    while (!_undoStack.empty()) {
        StateSnapshot* snapshot = _undoStack.top();
        _undoStack.pop();

        // 同样对快照中的卡牌进行安全释放
        for (auto card : snapshot->mainPileCards) {
            if (card && card->isRunning()) {
                card->stopAllActions();
                card->onExit();
            }
            CC_SAFE_RELEASE(card);
        }

        if (snapshot->bottomCard) {
            if (snapshot->bottomCard->isRunning()) {
                snapshot->bottomCard->stopAllActions();
                snapshot->bottomCard->onExit();
            }
            CC_SAFE_RELEASE(snapshot->bottomCard);
        }

        for (auto card : snapshot->reservePileCards) {
            if (card && card->isRunning()) {
                card->stopAllActions();
                card->onExit();
            }
            CC_SAFE_RELEASE(card);
        }

        delete snapshot;
    }
}

bool GameState::init()
{
    reset();
    return true;
}

void GameState::reset()
{
    for (auto card : _mainPileCards) {
        CC_SAFE_RELEASE(card);
    }
    _mainPileCards.clear();
    _mainPilePositions.clear();

    if (_bottomCard) {
        CC_SAFE_RELEASE(_bottomCard);
        _bottomCard = nullptr;
    }

    for (auto card : _reservePileCards) {
        CC_SAFE_RELEASE(card);
    }
    _reservePileCards.clear();

    while (!_undoStack.empty()) {
        StateSnapshot* snapshot = _undoStack.top();
        _undoStack.pop();

        for (auto card : snapshot->mainPileCards) {
            CC_SAFE_RELEASE(card);
        }
        if (snapshot->bottomCard) {
            CC_SAFE_RELEASE(snapshot->bottomCard);
        }
        for (auto card : snapshot->reservePileCards) {
            CC_SAFE_RELEASE(card);
        }

        delete snapshot;
    }
}

void GameState::addMainPileCard(Card* card, const cocos2d::Vec2& position)
{
    if (card) {
        card->retain();
        _mainPileCards.push_back(card);
        _mainPilePositions.push_back(position);
    }
}

void GameState::removeMainPileCard(Card* card)
{
    auto it = std::find(_mainPileCards.begin(), _mainPileCards.end(), card);
    if (it != _mainPileCards.end()) {
        size_t index = std::distance(_mainPileCards.begin(), it);
        _mainPilePositions.erase(_mainPilePositions.begin() + index);
        (*it)->release();
        _mainPileCards.erase(it);
    }
}

void GameState::updateMainPileCardPosition(Card* card, const cocos2d::Vec2& position)
{
    auto it = std::find(_mainPileCards.begin(), _mainPileCards.end(), card);
    if (it != _mainPileCards.end()) {
        size_t index = std::distance(_mainPileCards.begin(), it);
        _mainPilePositions[index] = position;
    }
}

int GameState::getMainPileCardCount() const
{
    return (int)_mainPileCards.size();
}

std::vector<Card*> GameState::getMainPileCards() const
{
    return _mainPileCards;
}

std::vector<cocos2d::Vec2> GameState::getMainPilePositions() const
{
    return _mainPilePositions;
}

Card* GameState::getBottomCard() const
{
    return _bottomCard;
}

Card* GameState::setBottomCard(Card* card)
{
    Card* oldCard = _bottomCard;

    _bottomCard = card;
    if (_bottomCard) {
        _bottomCard->retain();
    }

    return oldCard;
}

void GameState::addCardToBottomPile(Card* card)
{
    // 确保卡牌正面朝上
    //card->_isFaceUp=true;
    if (card) {
        card->retain(); // 增加引用计数

        // 释放旧的底牌（如果存在）
        if (_bottomCard) {
            _bottomCard->release();
        }

        // 设置新的底牌
        _bottomCard = card;
    }
}

void GameState::setReservePileCards(const std::vector<Card*>& cards)
{
    for (auto card : _reservePileCards) {
        CC_SAFE_RELEASE(card);
    }
    _reservePileCards.clear();

    for (auto card : cards) {
        if (card) {
            card->retain();
            _reservePileCards.push_back(card);
        }
    }
}

std::vector<Card*> GameState::getReservePileCards() const
{
    return _reservePileCards;
}

Card* GameState::drawCardFromReservePile()
{
    if (_reservePileCards.empty()) {
        return nullptr;
    }
    Card* card = _reservePileCards.front();

    _reservePileCards.erase(_reservePileCards.begin());

    return card;
}

int GameState::getReservePileCardCount() const
{
    return (int)_reservePileCards.size();
}

void GameState::replaceReserveCard(Card* oldCard, Card* newCard)
{
    // 找到要移除的卡牌
    auto it = std::find(_reservePileCards.begin(), _reservePileCards.end(), oldCard);
    if (it != _reservePileCards.end())
    {
        // 移除找到的卡牌（不需要release，因为它会成为底牌）
        _reservePileCards.erase(it);

        // 如果有新卡牌要添加（旧的底牌）
        if (newCard != nullptr)
        {
            _reservePileCards.push_back(newCard);
        }
    }
}

void GameState::saveStateForUndo()
{
    if (_undoStack.size() >= MAX_UNDO_STEPS) {
        StateSnapshot* oldestSnapshot = _undoStack.top();
        _undoStack.pop();

        for (auto card : oldestSnapshot->mainPileCards) {
            CC_SAFE_RELEASE(card);
        }
        if (oldestSnapshot->bottomCard) {
            CC_SAFE_RELEASE(oldestSnapshot->bottomCard);
        }
        for (auto card : oldestSnapshot->reservePileCards) {
            CC_SAFE_RELEASE(card);
        }

        delete oldestSnapshot;
    }

    StateSnapshot* snapshot = new StateSnapshot();
    snapshot->mainPilePositions = _mainPilePositions;
    for (auto card : _mainPileCards) {
        Card* cardCopy = card->clone();
        cardCopy->retain(); 
        snapshot->mainPileCards.push_back(cardCopy);
    }
    if (_bottomCard) {
        snapshot->bottomCard = _bottomCard->clone();
        snapshot->bottomCard->retain(); 
    }
    else {
        snapshot->bottomCard = nullptr;
    }

    for (auto card : _reservePileCards) {
        Card* cardCopy = card->clone();
        cardCopy->retain(); 
        snapshot->reservePileCards.push_back(cardCopy);
    }

    _undoStack.push(snapshot);
}

bool GameState::canUndo() const
{
    return !_undoStack.empty();
}

bool GameState::undo()
{
    if (_undoStack.empty()) {
        return false; // 无法回退
    }

    StateSnapshot* snapshot = _undoStack.top();
    _undoStack.pop();

    for (auto card : _mainPileCards) {
        CC_SAFE_RELEASE(card);
    }
    _mainPileCards.clear();
    _mainPilePositions.clear();

    if (_bottomCard) {
        CC_SAFE_RELEASE(_bottomCard);
        _bottomCard = nullptr;
    }

    for (auto card : _reservePileCards) {
        CC_SAFE_RELEASE(card);
    }
    _reservePileCards.clear();

    _mainPilePositions = snapshot->mainPilePositions;
    for (auto card : snapshot->mainPileCards) {
        Card* cardCopy = card->clone();
        cardCopy->retain();
        _mainPileCards.push_back(cardCopy);
    }

    if (snapshot->bottomCard) {
        _bottomCard = snapshot->bottomCard->clone();
        _bottomCard->retain(); 
    }

    for (auto card : snapshot->reservePileCards) {
        Card* cardCopy = card->clone();
        cardCopy->retain();
        _reservePileCards.push_back(cardCopy);
    }
    for (auto card : snapshot->mainPileCards) {
        CC_SAFE_RELEASE(card);
    }
    if (snapshot->bottomCard) {
        CC_SAFE_RELEASE(snapshot->bottomCard);
    }
    for (auto card : snapshot->reservePileCards) {
        CC_SAFE_RELEASE(card);
    }
    delete snapshot;

    return true;
}

std::string GameState::serialize() const
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // Serialize main pile cards
    document.AddMember("mainPile", serializeCardPile(_mainPileCards, _mainPilePositions, allocator), allocator);

    // Serialize bottom card
    document.AddMember("bottomCard", _bottomCard ? serializeCard(_bottomCard, allocator) :
        rapidjson::Value(rapidjson::kNullType), allocator);

    // Serialize reserve pile cards
    document.AddMember("reservePile", serializeCardPile(_reservePileCards, allocator), allocator);

    // Convert to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

bool GameState::deserialize(const std::string& data)
{
    reset();
    rapidjson::Document document;
    document.Parse(data.c_str());

    if (document.HasParseError() || !document.IsObject()) {
        return false;
    }
    // Deserialize main pile
    deserializeMainPile(document);

    // Deserialize bottom card
    deserializeBottomCard(document);

    // Deserialize reserve pile
    deserializeReservePile(document);

    return true;
}

rapidjson::Value GameState::serializeCard(Card* card, rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value cardObj(rapidjson::kObjectType);
    cardObj.AddMember("face", static_cast<int>(card->getFace()), allocator);
    cardObj.AddMember("suit", static_cast<int>(card->getSuit()), allocator);
    cardObj.AddMember("faceUp", card->isFaceUp(), allocator);
    cardObj.AddMember("blocked", card->isBlocked(), allocator);

    return cardObj;
}

rapidjson::Value GameState::serializeCardPile(const std::vector<Card*>& cards,
    rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value cardArray(rapidjson::kArrayType);

    for (auto card : cards) {
        cardArray.PushBack(serializeCard(card, allocator), allocator);
    }

    return cardArray;
}

rapidjson::Value GameState::serializeCardPile(const std::vector<Card*>& cards,
    const std::vector<Vec2>& positions,
    rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value cardArray(rapidjson::kArrayType);

    for (size_t i = 0; i < cards.size(); i++) {
        rapidjson::Value cardObj = serializeCard(cards[i], allocator);
        cardObj.AddMember("x", positions[i].x, allocator);
        cardObj.AddMember("y", positions[i].y, allocator);
        cardArray.PushBack(cardObj, allocator);
    }

    return cardArray;
}

Card* GameState::deserializeCard(const rapidjson::Value& cardObj)
{
    if (!cardObj.IsObject() ||
        !cardObj.HasMember("face") || !cardObj.HasMember("suit") ||
        !cardObj.HasMember("faceUp") || !cardObj.HasMember("blocked")) {
        return nullptr;
    }

    CardFaceType face = static_cast<CardFaceType>(cardObj["face"].GetInt());
    CardSuitType suit = static_cast<CardSuitType>(cardObj["suit"].GetInt());
    bool faceUp = cardObj["faceUp"].GetBool();
    bool blocked = cardObj["blocked"].GetBool();

    Card* card = new Card(face, suit, faceUp, blocked);
    card->autorelease();
    return card;
}

void GameState::deserializeMainPile(const rapidjson::Document& document)
{
    if (!document.HasMember("mainPile") || !document["mainPile"].IsArray()) {
        return;
    }

    const rapidjson::Value& mainPileArray = document["mainPile"];
    for (rapidjson::SizeType i = 0; i < mainPileArray.Size(); i++) {
        const rapidjson::Value& cardObj = mainPileArray[i];

        Card* card = deserializeCard(cardObj);
        if (card && cardObj.HasMember("x") && cardObj.HasMember("y")) {
            float x = cardObj["x"].GetFloat();
            float y = cardObj["y"].GetFloat();
            addMainPileCard(card, Vec2(x, y));
        }
    }
}

void GameState::deserializeBottomCard(const rapidjson::Document& document)
{
    if (!document.HasMember("bottomCard") || !document["bottomCard"].IsObject()) {
        return;
    }

    Card* card = deserializeCard(document["bottomCard"]);
    if (card) {
        setBottomCard(card);
    }
}

void GameState::deserializeReservePile(const rapidjson::Document& document)
{
    if (!document.HasMember("reservePile") || !document["reservePile"].IsArray()) {
        return;
    }

    const rapidjson::Value& reservePileArray = document["reservePile"];
    std::vector<Card*> reserveCards;

    for (rapidjson::SizeType i = 0; i < reservePileArray.Size(); i++) {
        Card* card = deserializeCard(reservePileArray[i]);
        if (card) {
            reserveCards.push_back(card);
        }
    }

    setReservePileCards(reserveCards);
}

bool GameState::saveGame(const std::string& filename) const
{
    std::string gameData = serialize();
    return FileUtils::getInstance()->writeStringToFile(gameData, filename);
}

bool GameState::loadGame(const std::string& filename)
{
    if (!FileUtils::getInstance()->isFileExist(filename)) {
        return false;
    }

    std::string gameData = FileUtils::getInstance()->getStringFromFile(filename);
    if (gameData.empty()) {
        return false;
    }
    return deserialize(gameData);
}
