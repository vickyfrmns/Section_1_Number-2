# PWM Control on STM32F4 Discovery
This project demonstrates how to generate a PWM signal on PC6 using Timer 3 (TIM3) on the STM32F4 Discovery board. The PWM duty cycle is varied from 0% to 100% in a loop, and the cycle repeats indefinitely. The project uses HAL library to configure and control the hardware peripherals.

## Requirements
- STM32F4 Discovery Board
- PlatformIO useing STM32Cube Framework
- Platform: STM32F4 series
- Clock: 84 MHz (default configuration for STM32F4)

## Features
- PWM Generation: The program uses Timer 3 (TIM3) to generate a PWM signal on pin PC6 of the STM32F4 Discovery.
- Duty Cycle Adjustment: The duty cycle is adjusted dynamically between 0% and 100% in increments of 1%.
- Looping Duty Cycle: The duty cycle increases from 0% to 100%, then decreases back to 0% in a continuous loop.
- System Clock: Configured to 84 MHz to work with the STM32F4 Discovery board.

## Pin Configuration
- Pin PC6 (STM32F4) is used for PWM output via Timer 3 Channel 1.
- Timer 3 is configured with a frequency of 1 kHz and can adjust the duty cycle from 0% to 100%.

## Setup and Configuration
- GPIO Initialization: Pin PC6 is configured for Alternate Function mode to map it to Timer 3 Channel 1 for PWM generation.
- Timer 3 Initialization: Timer 3 is configured for PWM mode with a frequency of 1 kHz, and the prescaler is set to 83.
- PWM Duty Cycle: The PWM duty cycle is dynamically updated by setting the compare value of the timer, which controls the width of the PWM pulse.

## How to Run
- Cloning a repository using the command
```bash
git clone --url_https/ssh
```
- Open the project in Visual Studio Code PlatformIO.
- Build and flash the program to your STM32F4 Discovery board.
- After programming, observe the PWM signal on pin PC6. The duty cycle will vary from 0% to 100% and back in a loop.
