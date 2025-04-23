#pragma once
#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

// 花色类型
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
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
    // 屏幕尺寸配置
    const float SCREEN_WIDTH = 1080.0f;
    const float MAIN_AREA_HEIGHT = 1500.0f;
    const float BOTTOM_AREA_HEIGHT = 580.0f;
    const float TOTAL_HEIGHT = MAIN_AREA_HEIGHT + BOTTOM_AREA_HEIGHT;

    // 卡牌尺寸配置
    const float CARD_WIDTH = 120.0f;
    const float CARD_HEIGHT = 180.0f;

    // 游戏规则配置
    const bool ALLOW_K_TO_A_WRAP = false;  // 是否允许K连接A（循环）

    // 分数配置
    const int SCORE_PER_CARD = 10;      // 每消除一张牌得分
}

#endif // __GAME_CONFIG_H__