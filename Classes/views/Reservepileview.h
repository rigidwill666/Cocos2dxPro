#pragma once
#ifndef __RESERVE_PILE_VIEW_H__
#define __RESERVE_PILE_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Cardview.h"
#include <vector>

/**
 * @class CardClickInfo
 * @brief 卡牌点击信息，包含被点击的卡牌和点击时的世界坐标
 */
class CardClickInfo : public cocos2d::Ref {
public:
    Card* card;                  // 被点击的卡牌
    cocos2d::Vec2 worldPosition; // 卡牌在世界坐标系中的位置
};

/**
 * @class ReservePileView
 * @brief 备用牌堆视图，用于展示和管理备用卡牌的 UI
 */
class ReservePileView : public cocos2d::Node
{
public:
    /**
     * @brief 创建 ReservePileView 实例
     */
    CREATE_FUNC(ReservePileView);

    /**
     * @brief 初始化备用牌堆视图
     * @return 是否初始化成功
     */
    virtual bool init();

    virtual ~ReservePileView();
    virtual void onExit() override;

    /**
     * @brief 设置备用牌堆中的卡牌
     * @param cards 要显示的卡牌集合
     */
    void setCards(const std::vector<Card*>& cards);

    /**
    * @brief 添加卡牌点击监听回调
    * @param callback 回调函数（参数为点击信息对象）
    */
    void addClickListener(const std::function<void(cocos2d::Ref*)>& callback);

    /**
     * @brief 获取备用牌堆当前所有卡牌
     * @return 卡牌数组
     */
    const std::vector<Card*>& getCards() const;

    /**
     * @brief 清空所有卡牌和视图
     */
    void clearAllCards();

    /**
    * @brief 更新视图显示（根据当前数据刷新UI）
    */
    void updateView();

private:
    /**
     * @brief 触摸开始事件处理（用于检测点击）
     */
    bool _onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    cocos2d::Sprite* _pileBase = nullptr;                ///< 备用堆底座精灵
    std::vector<CardView*> _cardViews;                   ///< 卡牌视图容器
    std::vector<Card*> _cards;                           ///< 卡牌数据容器
    std::function<void(cocos2d::Ref*)> _clickCallback;   ///< 点击事件回调函数
};

#endif // __RESERVE_PILE_VIEW_H__
