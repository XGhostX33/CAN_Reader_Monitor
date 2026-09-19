#include "display.h"
#include "global_vars.h"
#include <stdio.h>

/* Максимум 8 колонок */
#define MAX_COLUMNS  8

/* Центры колонок (256px) */
static const uint8_t X_CENTER[MAX_COLUMNS] = {
    16, 48, 80, 112, 144, 176, 208, 240
};

#define Y_VALUE     36
#define Y_LABEL     58

int16_t stored_values[MAX_COLUMNS] = {0};

/* Подписи — все возможные */
const char *all_labels[] = PARAM_LABELS;

static u8g2_t *g_u8g2 = NULL;

static void draw_centered(uint8_t x_center, uint8_t y, const char *str)
{
    uint8_t w = u8g2_GetUTF8Width(g_u8g2, str);
    u8g2_DrawUTF8(g_u8g2, x_center - w/2, y, str);
}

void display_Init(u8g2_t *u8g2)
{
    g_u8g2 = u8g2;
}

void display_UpdateElement(uint8_t index, int16_t value)
{
    if (index >= MAX_COLUMNS) return;
    stored_values[index] = value;

    u8g2_ClearBuffer(g_u8g2);
    u8g2_SetDrawColor(g_u8g2, 1);

    char str[10];

    /* ===== ЗНАЧЕНИЯ ===== */
    u8g2_SetFont(g_u8g2, u8g2_font_ncenB14_tr);

    for (uint8_t i = 0; i < P_MAX; i++) {
        sprintf(str, "%d", stored_values[i]);
        draw_centered(X_CENTER[i], Y_VALUE, str);
    }

    /* ===== ПОДПИСИ ===== */
    u8g2_SetFont(g_u8g2, u8g2_font_6x12_t_cyrillic);

    for (uint8_t i = 0; i < P_MAX; i++) {
        draw_centered(X_CENTER[i], Y_LABEL, all_labels[i]);
    }

    u8g2_SendBuffer(g_u8g2);
}
