#ifndef INC_GLOBAL_VARS_H_
#define INC_GLOBAL_VARS_H_

#define GET_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ============================================================
 *  ID CAN-СООБЩЕНИЙ (прямое чтение, из DBC MQB)
 * ============================================================ */
#define CAN_ID_MOTOR_07     0x640   /* Мотор: ОЖ, масло ДВС, впуск */
#define CAN_ID_GETRIEBE_14  0x3C8   /* АКПП: масло */
#define CAN_ID_KOMBI_02     0x6B7   /* Приборка: топливо, наружная */
#define CAN_ID_MOTOR_12     0x0A8   /* Мотор: обороты */
#define CAN_ID_ESP_21       0x0FD   /* ESP: скорость */
#define CAN_ID_MOTOR_20     0x121   /* Мотор: педаль газа */
#define CAN_ID_GATEWAY_74   0x3DD   /* Климат: вентилятор */
#define CAN_ID_BEM_05       0x365   /* Генератор: нагрузка */
#define CAN_ID_DIMMUNG_01   0x5F0   /* Яркость: дисплей */
#define CAN_ID_KLEMMEN      0x3C0   /* Клеммы: зажигание */

/* ============================================================
 *  СПИСОК ПАРАМЕТРОВ ДЛЯ ВЫВОДА
 *  Порядок = порядок на экране (слева-направо).
 *  Раскомментируй — появится на экране.
 * ============================================================ */

/* Основные (раскомментированы) */
#define PARAM_COOLANT       /* ОЖ */
#define PARAM_OIL_ENG       /* Масло ДВС */
#define PARAM_OIL_ATF       /* Масло АКПП */
#define PARAM_FUEL          /* Топливо */

/* Дополнительные (закомментированы) */
// #define PARAM_RPM        /* Обороты */
// #define PARAM_SPEED      /* Скорость */
// #define PARAM_AMBIENT    /* Наружная */
// #define PARAM_INTAKE     /* Впуск */
// #define PARAM_THROTTLE   /* Педаль газа */
// #define PARAM_FAN        /* Вентилятор печки */
// #define PARAM_LOAD       /* Нагрузка генератора */

/* ============================================================
 *  МАССИВ ПАРАМЕТРОВ (заполняется автоматически)
 * ============================================================ */
typedef enum {
#ifdef PARAM_COOLANT
    P_COOLANT,
#endif
#ifdef PARAM_OIL_ENG
    P_OIL_ENG,
#endif
#ifdef PARAM_OIL_ATF
    P_OIL_ATF,
#endif
#ifdef PARAM_FUEL
    P_FUEL,
#endif
#ifdef PARAM_RPM
    P_RPM,
#endif
#ifdef PARAM_SPEED
    P_SPEED,
#endif
#ifdef PARAM_AMBIENT
    P_AMBIENT,
#endif
#ifdef PARAM_INTAKE
    P_INTAKE,
#endif
#ifdef PARAM_THROTTLE
    P_THROTTLE,
#endif
#ifdef PARAM_FAN
    P_FAN,
#endif
#ifdef PARAM_LOAD
    P_LOAD,
#endif
    P_MAX
} param_id_t;

/* Подписи (заполняются в display.c) */
#define PARAM_LABELS { \
    "ОЖ", "МАСЛО", "АКПП", "ТОПЛ", \
    "ОБОР", "СКОР", "УЛИЦА", "ВПУСК", \
    "ГАЗ", "ВЕНТ", "ГЕН" \
}

typedef enum { FALSE, TRUE } Bool;

#endif
