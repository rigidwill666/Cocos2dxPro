#pragma once
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Bottompileview.h"
#include "Mainpileview.h"
#include "Reservepileview.h"
#include "Gamestate.h"
#include "Card.h"
#include "Gamecontroller.h"
#include <string>
#include <vector>
#include <map>

/**
 * @class GameScene
 * @brief 游戏主场景类，负责展示界面与处理交互逻辑。
 *
 * 游戏流程的入口点，负责加载关卡配置、初始化 UI、设置交互事件、
 * 并协调控制器与状态之间的数据更新。
 */
class GameScene : public cocos2d::Scene
{
public:
    /**
     * @brief 创建并返回一个完整的游戏场景对象
     * @return cocos2d::Scene* 游戏场景指针
     */
    static cocos2d::Scene* createScene();

    /**
     * @brief cocos2d 宏定义：创建当前类实例
     */
    CREATE_FUNC(GameScene);

    /**
     * @brief 初始化游戏场景
     * @return bool 初始化是否成功
     */
    virtual bool init() override;

    /**
     * @brief 离开场景时执行清理逻辑
     */
    virtual void onExit() override;

    /**
     * @brief 加载指定关卡的配置文件
     * @param levelFile 配置文件路径
     * @return bool 加载是否成功
     */
    bool loadLevelConfig(const std::string& levelFile);

    /**
     * @brief 加载主牌设置
     * @param document 配置文件解析格式结果
     */
    void processPlayfieldCards(const rapidjson::Document& document);

    /**
    * @brief 加载底牌备用牌设置
    * @param document 配置文件解析格式结果 reserveCards 备用牌存储结构
    */
    void GameScene::processStackCards(const rapidjson::Document& document, std::vector<Card*>& reserveCards);

    /**
     * @brief 判断配置文件数据有效性
     * @param cardData rapidjson::Value 引用，包含单张卡牌的属性数据
     * @return bool 是否有效
     */
    bool GameScene::isValidCardData(const rapidjson::Value& cardData);

    /**
     * @brief 从 JSON 数据创建一张卡牌对象（Card）
     * @param cardData rapidjson::Value 引用，包含单张卡牌的属性数据
     * @return Card* 创建好的卡牌对象
     */
    Card* GameScene::createCardFromData(const rapidjson::Value& cardData);

    /**
     * @brief 更新游戏 UI，包括卡牌状态、分数等
     * @param withAnimation 是否带动画
     * @param startPosition 动画起始坐标（可选）
     */
    void updateUI(bool withAnimation = false, const cocos2d::Vec2& startPosition = cocos2d::Vec2::ZERO);

private:
    // -------------------- UI 组件 --------------------
    BottomPileView* _bottomPileView;         // 底牌区视图
    MainPileView* _mainPileView;             // 主牌区视图
    ReservePileView* _reservePileView;       // 备用牌堆视图
    cocos2d::ui::Button* _undoButton;        // 撤销按钮

    // -------------------- 数据模型 --------------------
    GameState* _gameState;                   // 当前游戏状态指针
    std::map<std::string, cocos2d::Value> _levelConfig;  // 关卡配置数据

    // -------------------- 私有方法 --------------------
    /**
     * @brief 初始化 UI 控件（按钮、分数等）
     */
    void _initUI();

    /**
     * @brief 初始化游戏状态（如牌堆布局）
     */
    void _initGameState();

    /**
    * @brief 绑定各个 UI 的事件监听
    */
    void _initEvents();

    /**
     * @brief 撤销按钮点击响应
     * @param sender 事件触发者指针
     */
    void _onUndoClicked(cocos2d::Ref* sender);

    /**
    * @brief 备用牌堆点击事件响应
    * @param sender 事件触发者指针
    */
    void _onReservePileClicked(cocos2d::Ref* sender);

    /**
    * @brief 主牌堆点击事件响应
    * @param sender 事件触发者指针
    */
    void _onMainPileClicked(cocos2d::Ref* sender);
};

#endif // __GAME_SCENE_H__