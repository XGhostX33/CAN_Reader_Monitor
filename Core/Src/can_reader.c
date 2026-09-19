#include "can_reader.h"
#include "display.h"
#include "global_vars.h"

/* ============================================================
 *  ЗНАЧЕНИЯ
 * ============================================================ */
int16_t temp_coolant  = -40;   /* ОЖ */
int16_t temp_oil_eng  = -60;   /* Масло ДВС */
int16_t temp_oil_atf  = -58;   /* Масло АКПП */
int16_t fuel_level    = 0;     /* Топливо */
int16_t rpm           = 0;     /* Обороты */
int16_t speed         = 0;     /* Скорость */
int16_t temp_ambient  = -50;   /* Наружная */
int16_t temp_intake   = -48;   /* Впуск */
int16_t throttle      = 0;     /* Педаль газа */
int16_t fan_speed     = 0;     /* Вентилятор */
int16_t gen_load      = 0;     /* Нагрузка генератора */

/* ============================================================
 *  ИНДЕКСЫ (позиция в списке вывода)
 * ============================================================ */
static int idx_coolant = -1, idx_oil_eng = -1, idx_oil_atf = -1, idx_fuel = -1;
static int idx_rpm = -1, idx_speed = -1, idx_ambient = -1, idx_intake = -1;
static int idx_throttle = -1, idx_fan = -1, idx_load = -1;

/* ============================================================
 *  ИНИЦИАЛИЗАЦИЯ ИНДЕКСОВ
 * ============================================================ */
void can_reader_Init(void)
{
    int n = 0;
#ifdef PARAM_COOLANT
    idx_coolant = n++;
#endif
#ifdef PARAM_OIL_ENG
    idx_oil_eng = n++;
#endif
#ifdef PARAM_OIL_ATF
    idx_oil_atf = n++;
#endif
#ifdef PARAM_FUEL
    idx_fuel = n++;
#endif
#ifdef PARAM_RPM
    idx_rpm = n++;
#endif
#ifdef PARAM_SPEED
    idx_speed = n++;
#endif
#ifdef PARAM_AMBIENT
    idx_ambient = n++;
#endif
#ifdef PARAM_INTAKE
    idx_intake = n++;
#endif
#ifdef PARAM_THROTTLE
    idx_throttle = n++;
#endif
#ifdef PARAM_FAN
    idx_fan = n++;
#endif
#ifdef PARAM_LOAD
    idx_load = n++;
#endif
}

/* ============================================================
 *  ОБРАБОТКА CAN
 * ============================================================ */
void can_reader_Handler(uint32_t id, uint8_t *data)
{
    switch (id) {

    /* ========================================================
     *  Motor_07 (0x640)
     *  Байт 1: впуск  → D[1] * 0.75 - 48
     *  Байт 2: масло  → D[2] - 60
     *  Байт 3: ОЖ     → D[3] * 0.75 - 48
     * ======================================================== */
    case CAN_ID_MOTOR_07:
        temp_intake  = (int16_t)(data[1] * 0.75f) - 48;
        temp_oil_eng = (int16_t)data[2] - 60;
        temp_coolant = (int16_t)(data[3] * 0.75f) - 48;

        if (idx_coolant >= 0) display_UpdateElement(idx_coolant, temp_coolant);
        if (idx_oil_eng >= 0) display_UpdateElement(idx_oil_eng, temp_oil_eng);
        if (idx_intake  >= 0) display_UpdateElement(idx_intake,  temp_intake);
        break;

    /* ========================================================
     *  Getriebe_14 (0x3C8)
     *  Байт 7: масло АКПП → D[7] - 58
     * ======================================================== */
    case CAN_ID_GETRIEBE_14:
        temp_oil_atf = (int16_t)data[7] - 58;
        if (idx_oil_atf >= 0) display_UpdateElement(idx_oil_atf, temp_oil_atf);
        break;

    /* ========================================================
     *  Kombi_02 (0x6B7)
     *  Байт 5: топливо → D[5] & 0x7F
     *  Байт 7: наружная → D[7] * 0.5 - 50
     * ======================================================== */
    case CAN_ID_KOMBI_02:
        fuel_level   = data[5] & 0x7F;
        temp_ambient = (int16_t)(data[7] * 0.5f) - 50;

        if (idx_fuel    >= 0) display_UpdateElement(idx_fuel,    fuel_level);
        if (idx_ambient >= 0) display_UpdateElement(idx_ambient, temp_ambient);
        break;

    /* ========================================================
     *  Motor_12 (0x0A8) — обороты
     *  Байт 6-7: → ((D[6] << 8) | D[7]) * 0.25
     * ======================================================== */
    case CAN_ID_MOTOR_12:
        rpm = (int16_t)(((uint16_t)data[6] << 8 | data[7]) * 0.25f);
        if (idx_rpm >= 0) display_UpdateElement(idx_rpm, rpm);
        break;

    /* ========================================================
     *  ESP_21 (0x0FD) — скорость
     *  Байт 4-5: → ((D[4] << 8) | D[5]) * 0.01
     * ======================================================== */
    case CAN_ID_ESP_21:
        speed = (int16_t)(((uint16_t)data[4] << 8 | data[5]) * 0.01f);
        if (idx_speed >= 0) display_UpdateElement(idx_speed, speed);
        break;

    /* ========================================================
     *  Motor_20 (0x121) — педаль газа
     *  Байт 1: → D[1] * 0.4
     * ======================================================== */
    case CAN_ID_MOTOR_20:
        throttle = (int16_t)(data[1] * 0.4f);
        if (idx_throttle >= 0) display_UpdateElement(idx_throttle, throttle);
        break;

    /* ========================================================
     *  Gateway_74 (0x3DD) — вентилятор
     *  Байт 5: → D[5]
     * ======================================================== */
    case CAN_ID_GATEWAY_74:
        fan_speed = (int16_t)data[5];
        if (idx_fan >= 0) display_UpdateElement(idx_fan, fan_speed);
        break;

    /* ========================================================
     *  BEM_05 (0x365) — нагрузка генератора
     *  Байт 4: → D[4] * 3.225 + 0.025
     * ======================================================== */
    case CAN_ID_BEM_05:
        gen_load = (int16_t)(data[4] * 3.225f + 0.025f);
        if (idx_load >= 0) display_UpdateElement(idx_load, gen_load);
        break;

    default:
        break;
    }
}
