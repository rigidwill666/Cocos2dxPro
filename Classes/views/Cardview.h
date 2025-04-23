#pragma once
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Card.h"

/**
 * @class CardView
 * @brief ������ͼ��������ڽ� Card ģ����ͼ�η�ʽչʾ�ڽ����ϡ�
 *
 * CardView ����� Card ģ�Ͷ�����Ⱦ���Ƶ�ͼ������ֵ��
 * �������û��ĵ�����϶��Ƚ�����Ϊ��
 */
class CardView : public cocos2d::Node {
public:
    /**
     * @brief ����һ���µĿ�����ͼ����
     * @param card �󶨵�����ģ�Ͷ���
     * @return ������ CardView ʵ��ָ��
     */
    static CardView* create(Card* card);

    /**
     * @brief ��ʼ����ͼ����
     * @param card �󶨵�����ģ�Ͷ���
     * @return �Ƿ��ʼ���ɹ�
     */
    virtual bool init(Card* card);

    virtual ~CardView();

    /**
     * @brief ����ģ�͸��µ�ǰ��ͼ��ʾ����
     */
    void updateView();

    /**
     * @brief ����ǰ���������о��飨��������������ɫ�ȣ�
     *
     * �������»���ʱ��վɾ��飬������Դ���ӡ�
     */
    void cleanupSprites();

    /**
     * @brief �������泯�ϵĿ�����ͼ����
     */
    void updateFaceUpCard();

    /**
     * @brief ���±��泯�ϵĿ�����ͼ����
     */
    void updateFaceDownCard();

    /**
     * @brief �������־��飨��ʾ���Ƶ�����������ӵ���ͼ��
     */
    void createNumberSprites();

    /**
     * @brief ��ȡ��ǰ���Ƶ���ĸ��ʾ
     */
    std::string getCardValueText();

    /**
    * @brief ������ɫ���飨���ҡ����ĵȣ�������ӵ���ͼ��
    */
    void createSuitSprite();

    /**
     * @brief ���ݿ��ƻ�ɫ���ض�Ӧ��ɫͼ��·��
     *
     * @return std::string ����ͼ·��
     */
    std::string getSuitFilePath();
    /**
 * @brief ���볡��ʱ������ע������ȣ�
 */
    virtual void onEnter() override;

    /**
     * @brief �뿪����ʱ������ȡ�������ȣ�
     */
    virtual void onExit() override;

    /**
     * @brief ��ȡ��ǰ�󶨵�ģ��
     * @return Card* ����ģ��
     */
    Card* getCard() const;

    /**
     * @brief ���ÿ��Ƶ���¼��Ļص�����
     * @param callback �ص�����������Ϊ��ǰ Card*
     */
    void setClickCallback(const std::function<void(Card*)>& callback);

private:
    Card* _card;                         // �󶨵Ŀ�������ģ��
    cocos2d::Sprite* _background;        // ���Ʊ���ͼƬ
    cocos2d::Sprite* _numberSprite;      // ��ʾ�����ľ���
    cocos2d::Sprite* _suitSprite;        // ��ʾ��ɫ�ľ���
    std::function<void(Card*)> _clickCallback; // ����ص�����

};

#endif // __CARD_VIEW_H__#pragma once
