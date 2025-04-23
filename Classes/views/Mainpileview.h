#pragma once
#ifndef __MAIN_PILE_VIEW_H__
#define __MAIN_PILE_VIEW_H__

#include "cocos2d.h"
#include "Cardview.h"
#include "Card.h"
#include <vector>
#include "Reservepileview.h"

/**
 * @class MainPileView
 * @brief 主牌堆视图类，用于管理和渲染主牌堆上的卡牌视图。
 *        该类继承自 cocos2d::Node，负责将多个 CardView 添加到主牌堆并处理卡牌点击、拖动、更新等行为。
 */
class MainPileView : public cocos2d::Node {
public:
    /**
     * @brief 创建 MainPileView 实例（宏定义 CREATE_FUNC 生成）
     */
    CREATE_FUNC(MainPileView);

    /**
    * @brief 初始化函数，在 cocos2d-x 中用于对象构造后的初始化工作
    * @return bool 初始化是否成功
    */
    virtual bool init() override;

    /**
     * @brief 析构函数，释放资源
     */
    virtual ~MainPileView();

    /**
    * @brief 场景退出时的回调函数，用于清理
    */
    virtual void onExit() override;

    /**
    * @brief 清除所有卡牌视图
    */
    void clearAllCards();

    /**
     * @brief 向主牌堆添加一张卡牌视图
     * @param cardView 要添加的卡牌视图指针
     */
    void addCardView(CardView* cardView);

    /**
 * @brief 更新视图排列或样式（例如当卡牌数量发生变化）
 */
    void updateView();

    /**
     * @brief 设置卡牌点击事件的监听器
     * @param callback 回调函数（参数为点击的 Ref* 对象）
     */
    void addClickListener(const std::function<void(cocos2d::Ref*)>& callback);

    /**
     * @brief 从牌堆中移除一张卡牌视图（可选动画）
     * @param cardView 要移除的视图
     * @param animate 是否播放移除动画（默认 true）
     */
    void removeCardView(CardView* cardView, bool animate = true);
private:
    std::vector<CardView*> _cardViews; // 当前主牌堆中所有卡牌视图

    std::function<void(cocos2d::Ref*)> _clickCallback; // 卡牌点击回调函数
};

#endif // __MAIN_PILE_VIEW_H__
#pragma once
