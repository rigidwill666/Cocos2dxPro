#pragma once
#ifndef __RESERVE_PILE_VIEW_H__
#define __RESERVE_PILE_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Cardview.h"
#include <vector>

/**
 * @class CardClickInfo
 * @brief ���Ƶ����Ϣ������������Ŀ��ƺ͵��ʱ����������
 */
class CardClickInfo : public cocos2d::Ref {
public:
    Card* card;                  // ������Ŀ���
    cocos2d::Vec2 worldPosition; // ��������������ϵ�е�λ��
};

/**
 * @class ReservePileView
 * @brief �����ƶ���ͼ������չʾ�͹����ÿ��Ƶ� UI
 */
class ReservePileView : public cocos2d::Node
{
public:
    /**
     * @brief ���� ReservePileView ʵ��
     */
    CREATE_FUNC(ReservePileView);

    /**
     * @brief ��ʼ�������ƶ���ͼ
     * @return �Ƿ��ʼ���ɹ�
     */
    virtual bool init();

    virtual ~ReservePileView();
    virtual void onExit() override;

    /**
     * @brief ���ñ����ƶ��еĿ���
     * @param cards Ҫ��ʾ�Ŀ��Ƽ���
     */
    void setCards(const std::vector<Card*>& cards);

    /**
    * @brief ��ӿ��Ƶ�������ص�
    * @param callback �ص�����������Ϊ�����Ϣ����
    */
    void addClickListener(const std::function<void(cocos2d::Ref*)>& callback);

    /**
     * @brief ��ȡ�����ƶѵ�ǰ���п���
     * @return ��������
     */
    const std::vector<Card*>& getCards() const;

    /**
     * @brief ������п��ƺ���ͼ
     */
    void clearAllCards();

    /**
    * @brief ������ͼ��ʾ�����ݵ�ǰ����ˢ��UI��
    */
    void updateView();

private:
    /**
     * @brief ������ʼ�¼��������ڼ������
     */
    bool _onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    cocos2d::Sprite* _pileBase = nullptr;                ///< ���öѵ�������
    std::vector<CardView*> _cardViews;                   ///< ������ͼ����
    std::vector<Card*> _cards;                           ///< ������������
    std::function<void(cocos2d::Ref*)> _clickCallback;   ///< ����¼��ص�����
};

#endif // __RESERVE_PILE_VIEW_H__
