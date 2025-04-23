#pragma once
#ifndef __BOTTOM_PILE_VIEW_H__
#define __BOTTOM_PILE_VIEW_H__

#include "cocos2d.h"
#include "Cardview.h"
#include "Card.h"

/**
 * @class BottomPileView
 * @brief ���ƶ���ͼ���������չʾ���Ŀ���ռ��Ŀ��ƶѡ�
 */
class BottomPileView : public cocos2d::Node {
public:
    /**
     * @brief ���� BottomPileView ����
     */
    CREATE_FUNC(BottomPileView);

    /**
     * @brief ��ʼ����ͼ����
     * @return �Ƿ��ʼ���ɹ�
     */
    virtual bool init() override;

    virtual ~BottomPileView();

    /**
     * @brief �뿪����ʱ�Զ�������Դ
     */
    virtual void onExit() override;

    /**
     * @brief ��յ�ǰ������ͼ
     */
    void clearAllCards();

    /**
     * @brief ���õ�ǰ��ʾ�Ŀ��ƣ��޶�����
     * @param card Ҫ���õĿ������ݶ���
     */
    void setCurrentCard(Card* card);

    /**
      * @brief �жϵ������Ƿ�Ϊ��
      * @return �Ƿ�û�п���
      */
    //bool isEmpty() const { return _currentCardView == nullptr; }

    /**
     * @brief ����¿��ƣ������������ƶ�����������������
     * @param card Ҫ��ӵĿ���
     * @param startPos ��ʼλ�ã������ƶ�������㣩
     */
    void addNewCard(Card* card, const cocos2d::Vec2& startPos);

    /**
     * @brief ��ӿ��Ƶ����ƶѣ���ѡ���Ƿ�����������ͼ��
     * @param card Ҫ��ӵĿ���
     * @param updateView �Ƿ�����ˢ����ͼ
     */
    void addCardToStack(Card* card, bool updateView);

    /**
     * @brief ˢ�µ�ǰ�����������ʾ״̬
     */
    void updateView();
private:
    CardView* _currentCardView = nullptr;      // ��ǰչʾ�ĵ�����ͼ
    cocos2d::Sprite* _pileBase = nullptr;      // �������ĵ�������
};

#endif // __BOTTOM_PILE_VIEW_H__
