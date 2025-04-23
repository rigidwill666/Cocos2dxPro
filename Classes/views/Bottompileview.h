#pragma once
#ifndef __BOTTOM_PILE_VIEW_H__
#define __BOTTOM_PILE_VIEW_H__

#include "cocos2d.h"
#include "Cardview.h"
#include "Card.h"

/**
 * @class BottomPileView
 * @brief 底牌堆视图组件，用于展示玩家目标收集的卡牌堆。
 */
class BottomPileView : public cocos2d::Node {
public:
    /**
     * @brief 创建 BottomPileView 对象
     */
    CREATE_FUNC(BottomPileView);

    /**
     * @brief 初始化视图对象
     * @return 是否初始化成功
     */
    virtual bool init() override;

    virtual ~BottomPileView();

    /**
     * @brief 离开场景时自动清理资源
     */
    virtual void onExit() override;

    /**
     * @brief 清空当前底牌视图
     */
    void clearAllCards();

    /**
     * @brief 设置当前显示的卡牌（无动画）
     * @param card 要设置的卡牌数据对象
     */
    void setCurrentCard(Card* card);

    /**
      * @brief 判断底牌区是否为空
      * @return 是否没有卡牌
      */
    //bool isEmpty() const { return _currentCardView == nullptr; }

    /**
     * @brief 添加新卡牌（从其他区域移动而来，附带动画）
     * @param card 要添加的卡牌
     * @param startPos 起始位置（用于移动动画起点）
     */
    void addNewCard(Card* card, const cocos2d::Vec2& startPos);

    /**
     * @brief 添加卡牌到底牌堆（可选择是否立即更新视图）
     * @param card 要添加的卡牌
     * @param updateView 是否立刻刷新视图
     */
    void addCardToStack(Card* card, bool updateView);

    /**
     * @brief 刷新当前底牌区域的显示状态
     */
    void updateView();
private:
    CardView* _currentCardView = nullptr;      // 当前展示的底牌视图
    cocos2d::Sprite* _pileBase = nullptr;      // 底牌区的底座背景
};

#endif // __BOTTOM_PILE_VIEW_H__
