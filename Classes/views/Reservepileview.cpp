#include "Reservepileview.h"

USING_NS_CC;

bool ReservePileView::init()
{
    if (!Node::init()) {
        return false;
    }

    // 创建备用牌堆底座，请修改自己本地绝对路径
    _pileBase = Sprite::create("D:\\program_data\\cocos_work\\CardGame\\proj.win32\\card_general.png");
    _pileBase->setPosition(Vec2::ZERO);
    this->addChild(_pileBase);

    // 创建点击区域
    auto touchArea = EventListenerTouchOneByOne::create();
    touchArea->onTouchBegan = CC_CALLBACK_2(ReservePileView::_onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchArea, this);

    _cardViews.clear();
    return true;
}

ReservePileView::~ReservePileView()
{
    clearAllCards();
}

void ReservePileView::onExit()
{
    Node::onExit();
    clearAllCards();
}


void ReservePileView::setCards(const std::vector<Card*>& cards)
{
    _cards = cards;
    updateView();
}

void ReservePileView::addClickListener(const std::function<void(Ref*)>& callback)
{
    _clickCallback = callback;
}

const std::vector<Card*>& ReservePileView::getCards() const
{
    return _cards;
}

void ReservePileView::clearAllCards()
{
    // 移除所有卡牌视图
    for (auto cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();
    _cards.clear();
}

void ReservePileView::updateView()
{
    // 清除所有卡牌视图
    for (auto cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();

    // 显示所有牌，设置偏移量叠加显示
    float xOffset = 132.0f;
    int showCount = std::min(2, (int)_cards.size());

    for (int i = 0; i < showCount; ++i) {
        Card* card = _cards[_cards.size() - showCount + i];
        CardView* cardView = CardView::create(card);

        // 计算卡牌位置
        Vec2 cardPos = Vec2(i * xOffset, 0);
        cardView->setPosition(cardPos);
        cardView->setLocalZOrder(i); // 确保后面的卡显示在前面卡之上

        // 设置点击回调，传递卡牌和位置信息
        cardView->setClickCallback([this, card, cardPos](Card* clickedCard) {
            if (_clickCallback) {
                // 创建一个包含卡牌和位置的CardClickInfo对象
                CardClickInfo* info = new CardClickInfo();
                info->card = card;
                info->worldPosition = this->convertToWorldSpace(cardPos);
                info->autorelease(); // 自动释放，避免内存泄漏

                _clickCallback(info);
            }
            });

        this->addChild(cardView);
        _cardViews.push_back(cardView);
    }
}

bool ReservePileView::_onTouchBegan(Touch* touch, Event* event)
{
    return false;
}