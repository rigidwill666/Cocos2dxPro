#pragma once
#ifndef __MAIN_PILE_VIEW_H__
#define __MAIN_PILE_VIEW_H__

#include "cocos2d.h"
#include "Cardview.h"
#include "Card.h"
#include <vector>
#include "Reservepileview.h"

/**
 * @class MainPileView
 * @brief ���ƶ���ͼ�࣬���ڹ������Ⱦ���ƶ��ϵĿ�����ͼ��
 *        ����̳��� cocos2d::Node�����𽫶�� CardView ��ӵ����ƶѲ������Ƶ�����϶������µ���Ϊ��
 */
class MainPileView : public cocos2d::Node {
public:
    /**
     * @brief ���� MainPileView ʵ�����궨�� CREATE_FUNC ���ɣ�
     */
    CREATE_FUNC(MainPileView);

    /**
    * @brief ��ʼ���������� cocos2d-x �����ڶ������ĳ�ʼ������
    * @return bool ��ʼ���Ƿ�ɹ�
    */
    virtual bool init() override;

    /**
     * @brief �����������ͷ���Դ
     */
    virtual ~MainPileView();

    /**
    * @brief �����˳�ʱ�Ļص���������������
    */
    virtual void onExit() override;

    /**
    * @brief ������п�����ͼ
    */
    void clearAllCards();

    /**
     * @brief �����ƶ����һ�ſ�����ͼ
     * @param cardView Ҫ��ӵĿ�����ͼָ��
     */
    void addCardView(CardView* cardView);

    /**
 * @brief ������ͼ���л���ʽ�����統�������������仯��
 */
    void updateView();

    /**
     * @brief ���ÿ��Ƶ���¼��ļ�����
     * @param callback �ص�����������Ϊ����� Ref* ����
     */
    void addClickListener(const std::function<void(cocos2d::Ref*)>& callback);

    /**
     * @brief ���ƶ����Ƴ�һ�ſ�����ͼ����ѡ������
     * @param cardView Ҫ�Ƴ�����ͼ
     * @param animate �Ƿ񲥷��Ƴ�������Ĭ�� true��
     */
    void removeCardView(CardView* cardView, bool animate = true);
private:
    std::vector<CardView*> _cardViews; // ��ǰ���ƶ������п�����ͼ

    std::function<void(cocos2d::Ref*)> _clickCallback; // ���Ƶ���ص�����
};

#endif // __MAIN_PILE_VIEW_H__
#pragma once
