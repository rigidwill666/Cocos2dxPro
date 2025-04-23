#pragma once
#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Gameconfig.h"
#include <string>

/**
 * @class Card
 * @brief ��������ģ���࣬�̳��� cocos2d::Node
 */
class Card : public cocos2d::Node
{
private:
    CardFaceType _face;   // ������ֵ
    CardSuitType _suit;   // ���ƻ�ɫ
    bool _isFaceUp;       // �����Ƿ����泯��
    bool _isBlocked;      // �����Ƿ�����

public:
    /**
     * ���캯��
     */
    Card(CardFaceType face, CardSuitType suit, bool isFaceUp = true, bool isBlocked = false);

    /**
     * ��ʼ������
     */
    virtual bool init() override;

    /**
     * ��������ʵ��
     */
    static Card* create(CardFaceType face, CardSuitType suit, bool isFaceUp = true, bool isBlocked = false);

    // Getter ����
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
    bool isFaceUp() const { return _isFaceUp; }
    bool isBlocked() const { return _isBlocked; }

    // ��Ϸ�߼�����
    void flip();
    void setBlocked(bool blocked);
    int getValue() const;
    bool isRed() const;
    bool canStackOn(const Card* other) const;
    Card* clone() const;
    std::string getDisplayName() const;
};

#endif // __CARD_H__