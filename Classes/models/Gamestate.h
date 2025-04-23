#pragma once
#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <stack>
#include <string>
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

// 前向声明
class GameController;

/**
 * @class GameState
 * @brief 游戏状态类，负责存储当前游戏的数据状态
 *
 * 该类属于 Model 层，只处理数据的保存和变更，不处理 UI 和复杂业务逻辑。
 * 提供序列化接口用于游戏存档、恢复，并支持撤销操作。
 */
class GameState : public cocos2d::Ref
{
public:
    /**
     * @struct StateSnapshot
     * @brief 存储某一时刻的游戏状态，用于撤销操作
     */
    struct StateSnapshot {
        std::vector<Card*> mainPileCards;          // 中央牌堆的卡牌列表
        std::vector<cocos2d::Vec2> mainPilePositions; // 中央牌堆的卡牌位置
        Card* bottomCard;                          // 当前底牌
        std::vector<Card*> reservePileCards;       // 备用牌堆中的卡牌
    };

    /**
    * @brief 创建并初始化 GameState 对象
    */
    static GameState* create();
    GameState();
    virtual ~GameState();

    /**
     * @brief 初始化 GameState
     * @return 初始化是否成功
     */
    virtual bool init();

    /**
     * @brief 重置游戏状态，清除所有数据
     */
    void reset();

    // ========================= 主牌区操作 =========================
    void addMainPileCard(Card* card, const cocos2d::Vec2& position);   // 添加卡牌到主牌区
    void removeMainPileCard(Card* card);                                // 从主牌区移除指定卡牌
    void updateMainPileCardPosition(Card* card, const cocos2d::Vec2& position); // 更新主牌区中某张卡牌的位置

    int getMainPileCardCount() const;                                   // 获取主牌区卡牌数量
    std::vector<Card*> getMainPileCards() const;                        // 获取主牌区所有卡牌
    std::vector<cocos2d::Vec2> getMainPilePositions() const;            // 获取主牌区所有卡牌位置

    // ========================= 底牌区操作 =========================
    Card* getBottomCard() const;    // 获取当前底牌
    Card* setBottomCard(Card* card);    // 设置底牌
    void addCardToBottomPile(Card* card);    // 添加卡牌到底牌区（辅助操作）

    // ========================= 备用牌堆操作 =========================

    void setReservePileCards(const std::vector<Card*>& cards);         // 设置备用牌堆卡牌列表
    std::vector<Card*> getReservePileCards() const;                    // 获取备用牌堆卡牌
    Card* drawCardFromReservePile();                                    // 从备用牌堆抽取卡牌
    int getReservePileCardCount() const;                                // 获取备用牌堆卡牌数量

    void replaceReserveCard(Card* oldCard, Card* newCard);             // 替换备用牌堆中的一张卡牌

    // ========================= 撤销机制 =========================
    void saveStateForUndo();                                            // 保存当前状态以供撤销
    bool undo();                                                        // 执行一次撤销操作
    bool canUndo() const;                                               // 是否可以撤销

    // ========================= 序列化存档 =========================

    std::string serialize() const;                                      // 序列化当前状态为字符串
    bool deserialize(const std::string& data);                          // 从序列化数据还原状态
    rapidjson::Value serializeCard(Card* card, rapidjson::Document::AllocatorType& allocator) const; // 序列化单张卡牌为 JSON 对象
    rapidjson::Value serializeCardPile(const std::vector<Card*>& cards,rapidjson::Document::AllocatorType& allocator) const;           // 序列化卡牌堆（无位置信息）为 JSON 数组
    rapidjson::Value serializeCardPile(const std::vector<Card*>& cards,const std::vector<cocos2d::Vec2>& positions,rapidjson::Document::AllocatorType& allocator) const;           // 序列化卡牌堆（带位置信息）为 JSON 数组
    Card* deserializeCard(const rapidjson::Value& cardObj);             // 从 JSON 对象反序列化一张卡牌
    void deserializeMainPile(const rapidjson::Document& document);     // 从 JSON 文档中反序列化主牌堆
    void deserializeBottomCard(const rapidjson::Document& document);    // 从 JSON 文档中反序列化底牌
    void deserializeReservePile(const rapidjson::Document& document);   // 从 JSON 文档中反序列化备用牌堆

    bool saveGame(const std::string& filename) const;                   // 保存游戏到本地文件
    bool loadGame(const std::string& filename);                         // 从本地文件加载游戏

private:
    // 中央牌区数据
    std::vector<Card*> _mainPileCards;             // 中央牌区卡牌
    std::vector<cocos2d::Vec2> _mainPilePositions; // 对应卡牌位置（用于动画展示）

    // 底牌区数据
    Card* _bottomCard;                             // 当前底牌，仅允许一张

    // 备用牌堆
    std::vector<Card*> _reservePileCards;          // 储备牌堆中的卡牌

    // 撤销操作支持
    std::stack<StateSnapshot*> _undoStack;         // 撤销状态栈
    static const int MAX_UNDO_STEPS = 20;          // 最大允许的撤销次数

    // 友元类，允许其访问私有成员
    friend class GameController;
    friend class GameScene;
};

#endif // __GAME_STATE_H__