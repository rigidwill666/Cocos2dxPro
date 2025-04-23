#pragma once
#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Gameconfig.h"
#include <string>

/**
 * @class Card
 * @brief 卡牌数据模型类，包含面值、花色、正反面状态、是否阻塞等信息。
 *        不直接负责显示，由 CardView 控制视图。
 */
class Card : public cocos2d::Node
{
private:
    CardFaceType _face;   // 卡牌面值
    CardSuitType _suit;   // 卡牌花色
    bool _isFaceUp;       // 卡牌是否正面朝上
    bool _isBlocked;      // 卡牌是否被阻塞

public:
    /**
     * @brief 构造函数
     * @param face 面值
     * @param suit 花色
     * @param isFaceUp 是否正面朝上
     * @param isBlocked 是否被阻挡
     */
    Card(CardFaceType face, CardSuitType suit, bool isFaceUp = true, bool isBlocked = false);

    /**
     * @brief 初始化函数
     * @return 初始化是否成功
     */
    virtual bool init() override;

    /**
     * @brief 静态创建方法
     * @param face 面值
     * @param suit 花色
     * @param isFaceUp 是否正面朝上
     * @param isBlocked 是否被阻挡
     * @return 创建的卡牌对象
     */
    static Card* create(CardFaceType face, CardSuitType suit, bool isFaceUp = true, bool isBlocked = false);

    // Getter 方法
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
    bool isFaceUp() const { return _isFaceUp; }
    bool isBlocked() const { return _isBlocked; }

    /**
     * @brief 翻转卡牌（正面 <-> 背面）
     */
    void flip();

    /**
     * @brief 设置卡牌是否被阻挡
     * @param blocked 是否阻挡
     */
    void setBlocked(bool blocked);

    /**
    * @brief 获取卡牌的游戏值（A=1, J=11, Q=12, K=13）
    * @return 卡牌对应数值
    */
    int getValue() const;

    /**
     * @brief 判断是否是红色花色（红桃或方片）
     * @return 是红色返回 true，否则 false
     */
    bool isRed() const;

    /**
     * @brief 克隆当前卡牌（深拷贝一个副本）
     * @return 新的卡牌实例
     */
    Card* clone() const;
};

#endif // __CARD_H__
