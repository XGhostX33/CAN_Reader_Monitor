/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : CAN Reader — читалка
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "can_reader.h"
#include "display.h"
#include "global_vars.h"
#include <string.h>
#include <stdio.h>

/* ===================== USER CODE BEGIN PV ===================== */
CAN_HandleTypeDef hcan;
SPI_HandleTypeDef hspi2;
u8g2_t u8g2;
/* ===================== USER CODE END PV ===================== */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_SPI2_Init(void);

/* ===================== USER CODE BEGIN 0 ===================== */
uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8; (void)arg_ptr;
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: HAL_Delay(1); break;
        case U8X8_MSG_DELAY_MILLI:         HAL_Delay(arg_int); break;
        case U8X8_MSG_GPIO_CS:             HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, arg_int); break;
        case U8X8_MSG_GPIO_DC:             HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int); break;
        case U8X8_MSG_GPIO_RESET:          HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, arg_int); break;
        default: return 0;
    }
    return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8;
    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            HAL_SPI_Transmit(&hspi2, (uint8_t *)arg_ptr, arg_int, 10000);
            break;
        case U8X8_MSG_BYTE_INIT:
            break;
        case U8X8_MSG_BYTE_SET_DC:
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
            break;
        default: return 0;
    }
    return 1;
}
/* ===================== USER CODE END 0 ===================== */

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_CAN_Init();
    MX_SPI2_Init();

    /* ===================== USER CODE BEGIN 2 ===================== */

    u8g2_Setup_sh1122_256x64_f(&u8g2, U8G2_R0,
        u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    display_Init(&u8g2);
    can_reader_Init();

    /* Без заставки — сразу данные */
    for (uint8_t i = 0; i < P_MAX; i++) {
        display_UpdateElement(i, 0);
    }

    /* CAN-фильтр */
    CAN_FilterTypeDef f;

    f.FilterBank = 0;
    f.FilterMode = CAN_FILTERMODE_IDLIST;
    f.FilterScale = CAN_FILTERSCALE_16BIT;
    f.FilterIdHigh = (CAN_ID_MOTOR_07 << 5);
    f.FilterIdLow  = 0x0000;
    f.FilterMaskIdHigh = 0xFFFF;
    f.FilterMaskIdLow  = 0x0000;
    f.FilterFIFOAssignment = CAN_RX_FIFO0;
    f.FilterActivation = ENABLE;
    f.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 1;
    f.FilterIdHigh = (CAN_ID_GETRIEBE_14 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 2;
    f.FilterIdHigh = (CAN_ID_KOMBI_02 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 3;
    f.FilterIdHigh = (CAN_ID_MOTOR_12 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 4;
    f.FilterIdHigh = (CAN_ID_ESP_21 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 5;
    f.FilterIdHigh = (CAN_ID_MOTOR_20 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 6;
    f.FilterIdHigh = (CAN_ID_GATEWAY_74 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    f.FilterBank = 7;
    f.FilterIdHigh = (CAN_ID_BEM_05 << 5);
    f.FilterIdLow  = 0x0000;
    HAL_CAN_ConfigFilter(&hcan, &f);

    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

    /* ===================== USER CODE END 2 ===================== */

    while (1)
    {
        /* ===================== USER CODE BEGIN 3 ===================== */
        HAL_Delay(10);
        /* ===================== USER CODE END 3 ===================== */
    }
}

/* ===================== USER CODE BEGIN 4 ===================== */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan_ptr)
{
    uint8_t RxData[8];
    CAN_RxHeaderTypeDef RxHeader;
    HAL_CAN_GetRxMessage(hcan_ptr, CAN_RX_FIFO0, &RxHeader, RxData);

    can_reader_Handler(RxHeader.StdId, RxData);
}
/* ===================== USER CODE END 4 ===================== */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) Error_Handler();
}

static void MX_CAN_Init(void)
{
    hcan.Instance = CAN1;
    hcan.Init.Prescaler = 9;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_3TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = ENABLE;
    if (HAL_CAN_Init(&hcan) != HAL_OK) Error_Handler();
}

static void MX_SPI2_Init(void)
{
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi2) != HAL_OK) Error_Handler();
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = OLED_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_CS_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_DC_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_DC_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_RES_Pin;
    HAL_GPIO_Init(OLED_RES_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
