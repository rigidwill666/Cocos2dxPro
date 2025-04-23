#pragma once
#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "Gamestate.h"
#include "Card.h"

// 前向声明，避免循环依赖
class GameScene;
class CardView;

/**
 * @class GameController
 * @brief 游戏控制器类，作为游戏流程的中枢管理类，负责协调游戏场景（GameScene）与游戏状态（GameState）的交互，
 *        包含游戏开始、卡牌操作、撤销等逻辑。采用单例模式，确保全局唯一实例。
 */
class GameController
{
public:
    /**
    * @brief 获取 GameController 单例对象
    * @return GameController* 单例指针
    */
    static GameController* getInstance();

    /**
     * @brief 销毁 GameController 单例对象
     */
    static void destroyInstance();

    /**
     * @brief 设置当前游戏场景对象（GameScene）
     * @param scene 指向游戏场景的指针
     */
    void setGameScene(GameScene* scene);

    /**
     * @brief 设置当前游戏状态对象（GameState）
     * @param state 指向游戏状态的指针
     */
    void setGameState(GameState* state);

    /**
     * @brief 开始新游戏并加载指定关卡配置
     * @param levelId 关卡标识符（例如 "level1"）
     * @return bool 是否成功开始游戏
     */
    bool startGame(const std::string& levelId);

    /**
     * @brief 玩家点击主牌堆抽取卡牌操作
     * @param clickedCard 被点击的卡牌对象指针
     * @param clickPosition 点击坐标（默认为 Vec2::ZERO）
     */
    void onDrawCard(Card* clickedCard, const cocos2d::Vec2& clickPosition = cocos2d::Vec2::ZERO);

    /**
     * @brief 玩家点击任意卡牌时的响应事件（用于选牌或操作）
     * @param card 被点击的卡牌对象指针
     * @param clickPosition 点击坐标（默认为 Vec2::ZERO）
     */
    void onCardClicked(Card* card, const cocos2d::Vec2& clickPosition = cocos2d::Vec2::ZERO);

    /**
     * @brief 判断某张卡牌是否可以放置到底牌堆中
     * @param card 待检查的卡牌对象指针
     * @return bool 是否可以放置
     */    
    bool canPlaceOnBottomPile(Card* card);

    /**
     * @brief 撤销上一步操作，恢复上一状态
     */
    void undo();

private:
    
    GameController();// 私有构造函数，禁止外部创建
    ~GameController();// 析构函数

    /**
    * @brief 加载指定关卡配置
    * @param levelFile 配置文件路径
    * @return bool 是否加载成功
    */
    bool _loadLevelConfig(const std::string& levelFile);

    /**
     * @brief 保存当前状态以支持撤销操作
     */
    void _saveGameStateForUndo();

    /**
    * @brief 将卡牌移动到底牌堆
    * @param card 要移动的卡牌
    */
    void _moveCardToBottomPile(Card* card);

private:
    static GameController* _instance; // GameController 单例对象
    GameScene* _gameScene;            // 当前游戏场景
    GameState* _gameState;            // 当前游戏状态
    std::string _currentLevelId;      // 当前关卡 ID
};

#endif /* __GAME_CONTROLLER_H__ */
