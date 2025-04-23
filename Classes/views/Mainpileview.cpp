#include "Mainpileview.h"
#include "Gamecontroller.h"
#include "Card.h"
#include "Gameconfig.h"

USING_NS_CC;

bool MainPileView::init()
{
    if (!Node::init()) {
        return false;
    }

    // ��ʼ���յĿ�����ͼ����
    _cardViews.reserve(20); // Ԥ���ռ䣬��������
    return true;
}

MainPileView::~MainPileView()
{
    clearAllCards();
}

void MainPileView::onExit()
{
    Node::onExit();
    clearAllCards();
}

void MainPileView::clearAllCards()
{
    for (auto cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();
}

void MainPileView::addCardView(CardView* cardView)
{
    if (cardView) {
        _cardViews.push_back(cardView);
        this->addChild(cardView); // ȷ����ӵ���ǰ��ͼ��

        // ���õ���ص�
        cardView->setClickCallback([this, cardView](Card* card) {
            if (card && card->isFaceUp() && !card->isBlocked() && _clickCallback) {
                // ����һ���������ƺ�λ�õ�CardClickInfo����
                CardClickInfo* info = new CardClickInfo();
                info->card = card;
                info->worldPosition = this->convertToWorldSpace(cardView->getPosition());
                info->autorelease(); // �Զ��ͷţ������ڴ�й©

                _clickCallback(info);
            }
            });
    }
}

void MainPileView::addClickListener(const std::function<void(cocos2d::Ref*)>& callback)
{
    _clickCallback = callback;
}

void MainPileView::updateView()
{
    // �������п�����ͼ��״̬
    for (auto cardView : _cardViews) {
        if (cardView) {
            cardView->updateView();
        }
    }
}

void MainPileView::removeCardView(CardView* cardView, bool animate)
{
    if (!cardView) return;

    // ���������Ƴ�
    auto it = std::find(_cardViews.begin(), _cardViews.end(), cardView);
    if (it != _cardViews.end()) {
        _cardViews.erase(it);
    }

    if (animate) {
        // �����ʧ����
        auto fadeOut = FadeOut::create(0.3f);
        auto removeFunc = CallFunc::create([cardView]() {
            cardView->removeFromParent();
            });
        cardView->runAction(Sequence::create(fadeOut, removeFunc, nullptr));
    }
    else {
        cardView->removeFromParent();
    }
}