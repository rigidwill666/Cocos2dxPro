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

// ǰ������
class GameController;

/**
 * @class GameState
 * @brief ��Ϸ״̬�࣬����洢��ǰ��Ϸ������״̬
 *
 * �������� Model �㣬ֻ�������ݵı���ͱ���������� UI �͸���ҵ���߼���
 * �ṩ���л��ӿ�������Ϸ�浵���ָ�����֧�ֳ���������
 */
class GameState : public cocos2d::Ref
{
public:
    /**
     * @struct StateSnapshot
     * @brief �洢ĳһʱ�̵���Ϸ״̬�����ڳ�������
     */
    struct StateSnapshot {
        std::vector<Card*> mainPileCards;          // �����ƶѵĿ����б�
        std::vector<cocos2d::Vec2> mainPilePositions; // �����ƶѵĿ���λ��
        Card* bottomCard;                          // ��ǰ����
        std::vector<Card*> reservePileCards;       // �����ƶ��еĿ���
    };

    /**
    * @brief ��������ʼ�� GameState ����
    */
    static GameState* create();
    GameState();
    virtual ~GameState();

    /**
     * @brief ��ʼ�� GameState
     * @return ��ʼ���Ƿ�ɹ�
     */
    virtual bool init();

    /**
     * @brief ������Ϸ״̬�������������
     */
    void reset();

    // ========================= ���������� =========================
    void addMainPileCard(Card* card, const cocos2d::Vec2& position);   // ��ӿ��Ƶ�������
    void removeMainPileCard(Card* card);                                // ���������Ƴ�ָ������
    void updateMainPileCardPosition(Card* card, const cocos2d::Vec2& position); // ������������ĳ�ſ��Ƶ�λ��

    int getMainPileCardCount() const;                                   // ��ȡ��������������
    std::vector<Card*> getMainPileCards() const;                        // ��ȡ���������п���
    std::vector<cocos2d::Vec2> getMainPilePositions() const;            // ��ȡ���������п���λ��

    // ========================= ���������� =========================
    Card* getBottomCard() const;    // ��ȡ��ǰ����
    Card* setBottomCard(Card* card);    // ���õ���
    void addCardToBottomPile(Card* card);    // ��ӿ��Ƶ�������������������

    // ========================= �����ƶѲ��� =========================

    void setReservePileCards(const std::vector<Card*>& cards);         // ���ñ����ƶѿ����б�
    std::vector<Card*> getReservePileCards() const;                    // ��ȡ�����ƶѿ���
    Card* drawCardFromReservePile();                                    // �ӱ����ƶѳ�ȡ����
    int getReservePileCardCount() const;                                // ��ȡ�����ƶѿ�������

    void replaceReserveCard(Card* oldCard, Card* newCard);             // �滻�����ƶ��е�һ�ſ���

    // ========================= �������� =========================
    void saveStateForUndo();                                            // ���浱ǰ״̬�Թ�����
    bool undo();                                                        // ִ��һ�γ�������
    bool canUndo() const;                                               // �Ƿ���Գ���

    // ========================= ���л��浵 =========================

    std::string serialize() const;                                      // ���л���ǰ״̬Ϊ�ַ���
    bool deserialize(const std::string& data);                          // �����л����ݻ�ԭ״̬
    rapidjson::Value serializeCard(Card* card, rapidjson::Document::AllocatorType& allocator) const; // ���л����ſ���Ϊ JSON ����
    rapidjson::Value serializeCardPile(const std::vector<Card*>& cards,rapidjson::Document::AllocatorType& allocator) const;           // ���л����ƶѣ���λ����Ϣ��Ϊ JSON ����
    rapidjson::Value serializeCardPile(const std::vector<Card*>& cards,const std::vector<cocos2d::Vec2>& positions,rapidjson::Document::AllocatorType& allocator) const;           // ���л����ƶѣ���λ����Ϣ��Ϊ JSON ����
    Card* deserializeCard(const rapidjson::Value& cardObj);             // �� JSON �������л�һ�ſ���
    void deserializeMainPile(const rapidjson::Document& document);     // �� JSON �ĵ��з����л����ƶ�
    void deserializeBottomCard(const rapidjson::Document& document);    // �� JSON �ĵ��з����л�����
    void deserializeReservePile(const rapidjson::Document& document);   // �� JSON �ĵ��з����л������ƶ�

    bool saveGame(const std::string& filename) const;                   // ������Ϸ�������ļ�
    bool loadGame(const std::string& filename);                         // �ӱ����ļ�������Ϸ

private:
    // ������������
    std::vector<Card*> _mainPileCards;             // ������������
    std::vector<cocos2d::Vec2> _mainPilePositions; // ��Ӧ����λ�ã����ڶ���չʾ��

    // ����������
    Card* _bottomCard;                             // ��ǰ���ƣ�������һ��

    // �����ƶ�
    std::vector<Card*> _reservePileCards;          // �����ƶ��еĿ���

    // ��������֧��
    std::stack<StateSnapshot*> _undoStack;         // ����״̬ջ
    static const int MAX_UNDO_STEPS = 20;          // �������ĳ�������

    // ��Ԫ�࣬���������˽�г�Ա
    friend class GameController;
    friend class GameScene;
};

#endif // __GAME_STATE_H__