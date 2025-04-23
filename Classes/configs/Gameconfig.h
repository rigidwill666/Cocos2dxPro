#pragma once
#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

// ��ɫ����
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // ÷��
    CST_DIAMONDS,   // ����
    CST_HEARTS,     // ����
    CST_SPADES,     // ����
    CST_NUM_CARD_SUIT_TYPES
};

// ��������
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

namespace GameConfig {
    // ��Ļ�ߴ�����
    const float SCREEN_WIDTH = 1080.0f;
    const float MAIN_AREA_HEIGHT = 1500.0f;
    const float BOTTOM_AREA_HEIGHT = 580.0f;
    const float TOTAL_HEIGHT = MAIN_AREA_HEIGHT + BOTTOM_AREA_HEIGHT;

    // ���Ƴߴ�����
    const float CARD_WIDTH = 120.0f;
    const float CARD_HEIGHT = 180.0f;

    // ��Ϸ��������
    const bool ALLOW_K_TO_A_WRAP = false;  // �Ƿ�����K����A��ѭ����

    // ��������
    const int SCORE_PER_CARD = 10;      // ÿ����һ���Ƶ÷�
}

#endif // __GAME_CONFIG_H__