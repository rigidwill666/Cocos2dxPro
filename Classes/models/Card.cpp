#include "Card.h"

USING_NS_CC;

Card::Card(CardFaceType face, CardSuitType suit, bool isFaceUp, bool isBlocked)
    : _face(face)
    , _suit(suit)
    , _isFaceUp(isFaceUp)
    , _isBlocked(isBlocked)
{
    // 构造函数中只初始化变量
}

bool Card::init()
{
    if (!Node::init())
    {
        return false;
    }
    // 这里不进行渲染相关操作，只处理数据逻辑
    return true;
}

Card* Card::create(CardFaceType face, CardSuitType suit, bool isFaceUp, bool isBlocked)
{
    Card* card = new (std::nothrow) Card(face, suit, isFaceUp, isBlocked);
    if (card && card->init())
    {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

void Card::flip()
{
    _isFaceUp = !_isFaceUp;
    // 不调用渲染更新，由 CardView 处理
}

void Card::setBlocked(bool blocked)
{
    _isBlocked = blocked;
}

int Card::getValue() const
{
    return static_cast<int>(_face) + 1;
}

bool Card::isRed() const
{
    return _suit == CST_HEARTS || _suit == CST_DIAMONDS;
}

Card* Card::clone() const
{
    Card* newCard = new (std::nothrow) Card(_face, _suit, _isFaceUp, _isBlocked);
    if (newCard && newCard->init()) {
        newCard->autorelease();
        return newCard;
    }
    CC_SAFE_DELETE(newCard);
    return nullptr;
}
