#include "BottompileView.h"
#include "Cardview.h"

USING_NS_CC;

bool BottomPileView::init()
{
    if (!Node::init()) {
        return false;
    }

    // 创建底牌区底座，请修改自己本地绝对路径
    _pileBase = Sprite::create("D:\\program_data\\cocos_work\\test\\proj.win32\\card_general.png");
    if (_pileBase == nullptr) {
        // 如果加载失败，使用默认的纯色背景
        _pileBase = Sprite::create();
        if (_pileBase) {
            _pileBase->setTextureRect(Rect(0, 0, 100, 140)); // 设置大小为标准扑克牌尺寸
            _pileBase->setColor(Color3B(50, 50, 50)); // 设置深灰色背景
        }
        else {
            CCLOG("Failed to create bottom pile base sprite");
            return false;
        }
    }

    _pileBase->setPosition(Vec2::ZERO);
    this->addChild(_pileBase);

    _currentCardView = nullptr; // 初始时没有卡牌

    return true;
}

BottomPileView::~BottomPileView()
{
    // 清理资源
    clearAllCards();
}

void BottomPileView::onExit()
{
    Node::onExit();  // 确保调用父类的onExit
    clearAllCards();  // 清理所有卡牌
}


void BottomPileView::clearAllCards()
{
    if (_currentCardView) {
        _currentCardView->removeFromParent();
        _currentCardView = nullptr;  // 清空当前底牌
    }
}

void BottomPileView::setCurrentCard(Card* card)
{
    clearAllCards();  // 先清除旧卡牌

    if (card) {
        // 创建新的卡牌视图
        _currentCardView = CardView::create(card);
        if (_currentCardView) {
            _currentCardView->setPosition(cocos2d::Vec2::ZERO);  // 默认位置是底座中心
            this->addChild(_currentCardView);  // 添加到当前节点

            // 确保数字和花色显示正确
            _currentCardView->updateView();
        }
    }
}

void BottomPileView::addNewCard(Card* card, const cocos2d::Vec2& startPos)
{
    if (!card) return;

    // 创建新的卡牌视图
    CardView* newCardView = CardView::create(card);
    newCardView->setPosition(this->convertToNodeSpace(startPos));  // 转换为相对节点的位置
    this->addChild(newCardView);

    // 确保卡牌视图初始化时就显示正确的数字和花色
    newCardView->updateView();

    // 移动新卡到底牌区中心
    auto moveTo = cocos2d::MoveTo::create(0.3f, cocos2d::Vec2::ZERO);  // 0.3秒动画移动
    auto callback = cocos2d::CallFunc::create([this, newCardView]() {
        if (_currentCardView && _currentCardView != newCardView) {
            _currentCardView->removeFromParent();  // 移除旧卡
        }
        _currentCardView = newCardView;  // 更新为新卡

        // 确保更新后再次调用updateView确保显示正确
        _currentCardView->updateView();
        });

    newCardView->runAction(cocos2d::Sequence::create(moveTo, callback, nullptr));  // 动画完成后执行更新
}


void BottomPileView::addCardToStack(Card* card, bool updateView)
{
    if (!card) return;

    // 清除当前显示的卡牌
    if (_currentCardView) {
        _currentCardView->removeFromParent();
        _currentCardView = nullptr;
    }

    // 创建新卡牌视图 - 确保包含完整的卡牌信息（数字和花色）
    _currentCardView = CardView::create(card);
    _currentCardView->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(_currentCardView);

    // 确保卡牌视图更新，显示正确的数字和花色
    if (updateView) {
        _currentCardView->updateView();
    }
}

void BottomPileView::updateView()
{
    if (_currentCardView) {
        _currentCardView->updateView();  // 更新底牌区视图
    }
}
