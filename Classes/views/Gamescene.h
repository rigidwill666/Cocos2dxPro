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
 * @brief ��Ϸ�������࣬����չʾ�����봦�����߼���
 *
 * ��Ϸ���̵���ڵ㣬������عؿ����á���ʼ�� UI�����ý����¼���
 * ��Э����������״̬֮������ݸ��¡�
 */
class GameScene : public cocos2d::Scene
{
public:
    /**
     * @brief ����������һ����������Ϸ��������
     * @return cocos2d::Scene* ��Ϸ����ָ��
     */
    static cocos2d::Scene* createScene();

    /**
     * @brief cocos2d �궨�壺������ǰ��ʵ��
     */
    CREATE_FUNC(GameScene);

    /**
     * @brief ��ʼ����Ϸ����
     * @return bool ��ʼ���Ƿ�ɹ�
     */
    virtual bool init() override;

    /**
     * @brief �뿪����ʱִ�������߼�
     */
    virtual void onExit() override;

    /**
     * @brief ����ָ���ؿ��������ļ�
     * @param levelFile �����ļ�·��
     * @return bool �����Ƿ�ɹ�
     */
    bool loadLevelConfig(const std::string& levelFile);

    /**
     * @brief ������������
     * @param document �����ļ�������ʽ���
     */
    void processPlayfieldCards(const rapidjson::Document& document);

    /**
    * @brief ���ص��Ʊ���������
    * @param document �����ļ�������ʽ��� reserveCards �����ƴ洢�ṹ
    */
    void GameScene::processStackCards(const rapidjson::Document& document, std::vector<Card*>& reserveCards);

    /**
     * @brief �ж������ļ�������Ч��
     * @param cardData rapidjson::Value ���ã��������ſ��Ƶ���������
     * @return bool �Ƿ���Ч
     */
    bool GameScene::isValidCardData(const rapidjson::Value& cardData);

    /**
     * @brief �� JSON ���ݴ���һ�ſ��ƶ���Card��
     * @param cardData rapidjson::Value ���ã��������ſ��Ƶ���������
     * @return Card* �����õĿ��ƶ���
     */
    Card* GameScene::createCardFromData(const rapidjson::Value& cardData);

    /**
     * @brief ������Ϸ UI����������״̬��������
     * @param withAnimation �Ƿ������
     * @param startPosition ������ʼ���꣨��ѡ��
     */
    void updateUI(bool withAnimation = false, const cocos2d::Vec2& startPosition = cocos2d::Vec2::ZERO);

private:
    // -------------------- UI ��� --------------------
    BottomPileView* _bottomPileView;         // ��������ͼ
    MainPileView* _mainPileView;             // ��������ͼ
    ReservePileView* _reservePileView;       // �����ƶ���ͼ
    cocos2d::ui::Button* _undoButton;        // ������ť

    // -------------------- ����ģ�� --------------------
    GameState* _gameState;                   // ��ǰ��Ϸ״ָ̬��
    std::map<std::string, cocos2d::Value> _levelConfig;  // �ؿ���������

    // -------------------- ˽�з��� --------------------
    /**
     * @brief ��ʼ�� UI �ؼ�����ť�������ȣ�
     */
    void _initUI();

    /**
     * @brief ��ʼ����Ϸ״̬�����ƶѲ��֣�
     */
    void _initGameState();

    /**
    * @brief �󶨸��� UI ���¼�����
    */
    void _initEvents();

    /**
     * @brief ������ť�����Ӧ
     * @param sender �¼�������ָ��
     */
    void _onUndoClicked(cocos2d::Ref* sender);

    /**
    * @brief �����ƶѵ���¼���Ӧ
    * @param sender �¼�������ָ��
    */
    void _onReservePileClicked(cocos2d::Ref* sender);

    /**
    * @brief ���ƶѵ���¼���Ӧ
    * @param sender �¼�������ָ��
    */
    void _onMainPileClicked(cocos2d::Ref* sender);
};

#endif // __GAME_SCENE_H__