#include "Reservepileview.h"

USING_NS_CC;

bool ReservePileView::init()
{
    if (!Node::init()) {
        return false;
    }

    // ���������ƶѵ��������޸��Լ����ؾ���·��
    _pileBase = Sprite::create("D:\\program_data\\cocos_work\\CardGame\\proj.win32\\card_general.png");
    _pileBase->setPosition(Vec2::ZERO);
    this->addChild(_pileBase);

    // �����������
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
    // �Ƴ����п�����ͼ
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
    // ������п�����ͼ
    for (auto cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();

    // ��ʾ�����ƣ�����ƫ����������ʾ
    float xOffset = 132.0f;
    int showCount = std::min(2, (int)_cards.size());

    for (int i = 0; i < showCount; ++i) {
        Card* card = _cards[_cards.size() - showCount + i];
        CardView* cardView = CardView::create(card);

        // ���㿨��λ��
        Vec2 cardPos = Vec2(i * xOffset, 0);
        cardView->setPosition(cardPos);
        cardView->setLocalZOrder(i); // ȷ������Ŀ���ʾ��ǰ�濨֮��

        // ���õ���ص������ݿ��ƺ�λ����Ϣ
        cardView->setClickCallback([this, card, cardPos](Card* clickedCard) {
            if (_clickCallback) {
                // ����һ���������ƺ�λ�õ�CardClickInfo����
                CardClickInfo* info = new CardClickInfo();
                info->card = card;
                info->worldPosition = this->convertToWorldSpace(cardPos);
                info->autorelease(); // �Զ��ͷţ������ڴ�й©

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