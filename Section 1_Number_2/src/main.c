#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

// Handler timer
TIM_HandleTypeDef htim3;

void SystemClock_Config();
void GPIO_Init();
void TIM3_Init();
void PWM_SetDutyCycle(uint8_t dutyCycle);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    TIM3_Init();

    // Start PWM on TIM3 Channel 1 (PC6)
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    while (1)
    {
        // Change the duty cycle from 0% to 100%
        for (int dutyCycle = 0; dutyCycle <= 100; dutyCycle++)
        {
            PWM_SetDutyCycle(dutyCycle);
            HAL_Delay(100);
        }

        // Change the duty cycle back from 100% to 0%
        for (int dutyCycle = 100; dutyCycle >= 0; dutyCycle--)
        {
            PWM_SetDutyCycle(dutyCycle);
            HAL_Delay(100);
        }
    }
}

// Function to set the duty cycle (0-100%)
void PWM_SetDutyCycle(uint8_t dutyCycle)
{
    // command used so that the maximum duty cycle is at 100
    if (dutyCycle > 100)
    {
        dutyCycle = 100;
    }

    // Calculate the new compare value based on the duty cycle
    uint32_t compareValue = (__HAL_TIM_GET_AUTORELOAD(&htim3) + 1) * dutyCycle / 100;

    // Set compare value on TIM3 Channel 1
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, compareValue);
}

// Initialize Timer 3 for PWM because GPIOC6 use TIM3_CH1
void TIM3_Init()
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 83; // Prescaler to clock timer 1 MHz (84 MHz / (Prescaler + 1))
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 999; // Period for 1 kHz PWM frequency
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim3);

    HAL_TIM_PWM_Init(&htim3);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0; // Initial duty cycle value 0%
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
}

// Initialize GPIO PC6 for alternate function
void GPIO_Init()
{
    __HAL_RCC_GPIOC_CLK_ENABLE(); /// Activation of GPIO C on STM32F4

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6;       // Pin PC6 on STM32F4
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Swicth to mode Alternate Function to execute PWM
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3; // Alternate Function TIM3 using AF2
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// System clock configuration (84 MHz for STM32F4 Discovery)
void SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}