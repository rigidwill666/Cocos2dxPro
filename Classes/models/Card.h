#pragma once
#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Gameconfig.h"
#include <string>

/**
 * @class Card
 * @brief 卡牌数据模型类，继承自 cocos2d::Node
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
     * 构造函数
     */
    Card(CardFaceType face, CardSuitType suit, bool isFaceUp = true, bool isBlocked = false);

    /**
     * 初始化函数
     */
    virtual bool init() override;

    /**
     * 创建卡牌实例
     */
    static Card* create(CardFaceType face, CardSuitType suit, bool isFaceUp = true, bool isBlocked = false);

    // Getter 方法
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
    bool isFaceUp() const { return _isFaceUp; }
    bool isBlocked() const { return _isBlocked; }

    // 游戏逻辑方法
    void flip();
    void setBlocked(bool blocked);
    int getValue() const;
    bool isRed() const;
    bool canStackOn(const Card* other) const;
    Card* clone() const;
    std::string getDisplayName() const;
};

#endif // __CARD_H__