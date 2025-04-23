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

    // 初始化空的卡牌视图数组
    _cardViews.reserve(20); // 预留空间，提升性能
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
        this->addChild(cardView); // 确保添加到当前视图中

        // 设置点击回调
        cardView->setClickCallback([this, cardView](Card* card) {
            if (card && card->isFaceUp() && !card->isBlocked() && _clickCallback) {
                // 创建一个包含卡牌和位置的CardClickInfo对象
                CardClickInfo* info = new CardClickInfo();
                info->card = card;
                info->worldPosition = this->convertToWorldSpace(cardView->getPosition());
                info->autorelease(); // 自动释放，避免内存泄漏

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
    // 更新所有卡牌视图的状态
    for (auto cardView : _cardViews) {
        if (cardView) {
            cardView->updateView();
        }
    }
}

void MainPileView::removeCardView(CardView* cardView, bool animate)
{
    if (!cardView) return;

    // 从数组中移除
    auto it = std::find(_cardViews.begin(), _cardViews.end(), cardView);
    if (it != _cardViews.end()) {
        _cardViews.erase(it);
    }

    if (animate) {
        // 添加消失动画
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