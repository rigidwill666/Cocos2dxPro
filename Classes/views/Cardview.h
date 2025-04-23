#pragma once
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Card.h"

/**
 * @class CardView
 * @brief 卡牌视图组件，用于将 Card 模型以图形方式展示在界面上。
 *
 * CardView 负责绑定 Card 模型对象，渲染卡牌的图案与数值，
 * 并处理用户的点击或拖动等交互行为。
 */
class CardView : public cocos2d::Node {
public:
    /**
     * @brief 创建一个新的卡牌视图对象
     * @param card 绑定的数据模型对象
     * @return 创建的 CardView 实例指针
     */
    static CardView* create(Card* card);

    /**
     * @brief 初始化视图对象
     * @param card 绑定的数据模型对象
     * @return 是否初始化成功
     */
    virtual bool init(Card* card);

    virtual ~CardView();

    /**
     * @brief 根据模型更新当前视图显示内容
     */
    void updateView();

    /**
     * @brief 清理当前卡牌上所有精灵（背景、点数、花色等）
     *
     * 用于重新绘制时清空旧精灵，避免资源叠加。
     */
    void cleanupSprites();

    /**
     * @brief 更新正面朝上的卡牌视图内容
     */
    void updateFaceUpCard();

    /**
     * @brief 更新背面朝上的卡牌视图内容
     */
    void updateFaceDownCard();

    /**
     * @brief 创建数字精灵（显示卡牌点数），并添加到视图中
     */
    void createNumberSprites();

    /**
     * @brief 获取当前卡牌的字母表示
     */
    std::string getCardValueText();

    /**
    * @brief 创建花色精灵（黑桃、红心等），并添加到视图中
    */
    void createSuitSprite();

    /**
     * @brief 根据卡牌花色返回对应花色图标路径
     *
     * @return std::string 精灵图路径
     */
    std::string getSuitFilePath();
    /**
 * @brief 进入场景时触发（注册监听等）
 */
    virtual void onEnter() override;

    /**
     * @brief 离开场景时触发（取消监听等）
     */
    virtual void onExit() override;

    /**
     * @brief 获取当前绑定的模型
     * @return Card* 数据模型
     */
    Card* getCard() const;

    /**
     * @brief 设置卡牌点击事件的回调函数
     * @param callback 回调函数，参数为当前 Card*
     */
    void setClickCallback(const std::function<void(Card*)>& callback);

private:
    Card* _card;                         // 绑定的卡牌数据模型
    cocos2d::Sprite* _background;        // 卡牌背景图片
    cocos2d::Sprite* _numberSprite;      // 显示点数的精灵
    cocos2d::Sprite* _suitSprite;        // 显示花色的精灵
    std::function<void(Card*)> _clickCallback; // 点击回调函数

};

#endif // __CARD_VIEW_H__#pragma once
