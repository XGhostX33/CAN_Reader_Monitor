# CAN Reader — Monitor для VW MQB

Прямое чтение **CAN Comfort** для **VW Tiguan 2 (2023, MQB)**.

![CAN Reader](docs/screenshot.jpg)

## 📊 Что читает

| Параметр | ID CAN | Байт | Формула | Статус |
|----------|--------|------|---------|--------|
| **ОЖ** | `0x640` | 3 | `D[3] * 0.75 - 48` | ✅ |
| **Масло ДВС** | `0x640` | 2 | `D[2] - 60` | ✅ |
| **Масло АКПП** | `0x3C8` | 7 | `D[7] - 58` | ✅ |
| **Топливо** | `0x6B7` | 5 | `D[5] & 0x7F` | ✅ |
| **Обороты** | `0x0A8` | 6-7 | `((D[6]<<8) \| D[7]) * 0.25` | ⚙️ опция |
| **Скорость** | `0x0FD` | 4-5 | `((D[4]<<8) \| D[5]) * 0.01` | ⚙️ опция |
| **Наружная** | `0x6B7` | 7 | `D[7] * 0.5 - 50` | ⚙️ опция |
| **Впуск** | `0x640` | 1 | `D[1] * 0.75 - 48` | ⚙️ опция |
| **Педаль газа** | `0x121` | 1 | `D[1] * 0.4` | ⚙️ опция |
| **Вентилятор** | `0x3DD` | 5 | `D[5]` | ⚙️ опция |
| **Нагрузка ген.** | `0x365` | 4 | `D[4] * 3.225 + 0.025` | ⚙️ опция |

⚙️ **Опция** — раскомментируй в `global_vars.h` → параметр появится на экране.

## 🔌 Железо

| Компонент | Назначение | Цена |
|-----------|-----------|------|
| **STM32F103C8T6** (Blue Pill) | Микроконтроллер | ~200 ₽ |
| **SH1122** 256x64 OLED (SPI) | Дисплей | ~800 ₽ |
| **SN65HVD230** | CAN-трансивер 3.3V | ~100 ₽ |
| **Mini-360** | DC-DC понижающий | ~100 ₽ |
| **ST-Link V2** | Программатор | ~300 ₽ |

**Итого:** ~1500 ₽

## 🛠 Схема подключения
┌──────────────┐ ┌──────────────┐
│ STM32F103 │ │ SH1122 │
│ │ │ 256x64 │
│ PA4 ────────┼─────────┤ CS │
│ PA8 ────────┼─────────┤ DC │
│ PB1 ────────┼─────────┤ RESET │
│ PB13 ───────┼─────────┤ SCK │
│ PB15 ───────┼─────────┤ MOSI │
│ │ │ │
│ PA11 ───────┼────┐ │ VCC ─ 3.3V │
│ PA12 ───────┼──┐ │ │ GND ─ GND │
└──────────────┘ │ │ └──────────────┘
│ │
┌─────┴─┴─────┐
│ SN65HVD230 │
│ TXD ─ PA12 │
│ RXD ─ PA11 │
│ CANH ──────┼──→ CAN-H (OBD2 pin 6)
│ CANL ──────┼──→ CAN-L (OBD2 pin 14)
│ VCC ─ 3.3V │
│ GND ─ GND │
└─────────────┘


## 🔗 Подключение к машине

**Quadlock за магнитолой** (VW MQB):
- **Pin 1** → **CAN-H**
- **Pin 2** → **CAN-L**
- **Pin 17** → **GND**

**Или OBD2 разъём** (если есть доступ к Comfort):
- **Pin 6** → **CAN-H**
- **Pin 14** → **CAN-L**
- **Pin 4/5** → **GND**

⚠️ **OBD2 разъём идёт через Gateway** — он **фильтрует** трафик. Для **прямого чтения** нужен **Quadlock** или **блок климата**.

## 🛠 Сборка

### 1. Клонировать репозиторий
```bash
git clone https://github.com/XGhostX33/CAN_Reader_Monitor.git

 Открыть в STM32CubeIDE
File → Import → General → Existing Projects into Workspace

Указать папку CAN_Reader_Monitor

3. Собрать
Project → Build All

4. Прошить
Run → Debug (или Run → Run)

ST-Link подключён к SWD

📁 Структура проекта
text
CAN_Reader_Monitor/
├── Core/
│   ├── Inc/
│   │   ├── main.h              # Пины дисплея
│   │   ├── global_vars.h       # ID CAN и список параметров
│   │   ├── can_reader.h        # Прототипы
│   │   └── display.h           # Прототипы
│   └── Src/
│       ├── main.c              # Главный цикл, CAN-фильтр
│       ├── can_reader.c        # Обработка CAN
│       └── display.c           # Отрисовка
├── Drivers/                    # HAL библиотеки
├── CAN_Reader_Monitor.ioc      # CubeMX конфигурация
├── .cproject                   # Настройки проекта
├── .project                    # Настройки проекта
└── README.md
🎯 Как настроить вывод
Файл: Core/Inc/global_vars.h

Основные параметры (раскомментированы):

c
#define PARAM_COOLANT       /* ОЖ */
#define PARAM_OIL_ENG       /* Масло ДВС */
#define PARAM_OIL_ATF       /* Масло АКПП */
#define PARAM_FUEL          /* Топливо */
Дополнительные (закомментированы):

c
// #define PARAM_RPM        /* Обороты */
// #define PARAM_SPEED      /* Скорость */
// #define PARAM_AMBIENT    /* Наружная */
// #define PARAM_INTAKE     /* Впуск */
// #define PARAM_THROTTLE   /* Педаль газа */
// #define PARAM_FAN        /* Вентилятор */
// #define PARAM_LOAD       /* Нагрузка генератора */
Раскомментируй → параметр появится на экране автоматически.

📚 Источники
DBC MQB: Vaggysag/opendbc

mqbcan: jrjoaoramos/mqbcan

can-utils: linux-can/can-utils

Cangaroo: normaldotcom/cangaroo

SavvyCAN: collin80/SavvyCAN

📄 Лицензия
MIT License — используйте свободно.

