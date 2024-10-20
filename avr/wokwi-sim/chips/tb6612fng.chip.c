// Source: unknown (https://wokwi.com/projects/394149080650977281)

// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023

#include "../wokwi-api.h"
// #include <stdio.h>

// stdlib in this environment/setup requires WASM random_get which wokwi sim does not implement
// #include <stdlib.h>

// Pull these in from walloc.c instead.
void *malloc(size_t size);
// void free(void *p);

// make the linker happy until I figure out how to pass --no-entry via zig
int main() {};

typedef struct
{
    pin_t pin_PWMA; // Pin de control de velocidad del motor A (PWM)
    pin_t pin_AIN1; // Pin de dirección del motor A
    pin_t pin_AIN2; // Pin de dirección del motor A
    pin_t pin_PWMB; // Pin de control de velocidad del motor B (PWM)
    pin_t pin_BIN1; // Pin de dirección del motor B
    pin_t pin_BIN2; // Pin de dirección del motor B
    pin_t pin_STBY; // Pin de habilitación/deshabilitación del puente H (STBY)
    pin_t pin_AO1;
    pin_t pin_AO2;
    pin_t pin_BO1;
    pin_t pin_BO2;
    pin_t pin_VM;
} chip_state_t;

static void chip_pin_change(void *user_data, pin_t pin, uint32_t value);

void chip_init(void)
{
    chip_state_t *chip = malloc(sizeof(chip_state_t));
    chip->pin_PWMA = pin_init("PWMA", ANALOG);
    chip->pin_AIN1 = pin_init("AIN1", INPUT);
    chip->pin_AIN2 = pin_init("AIN2", INPUT);
    chip->pin_PWMB = pin_init("PWMB", ANALOG);
    chip->pin_BIN1 = pin_init("BIN1", INPUT);
    chip->pin_BIN2 = pin_init("BIN2", INPUT);
    chip->pin_STBY = pin_init("STBY", INPUT);
    chip->pin_VM = pin_init("VM", INPUT);
    chip->pin_AO1 = pin_init("AO1", ANALOG);
    chip->pin_AO2 = pin_init("AO2", ANALOG);
    chip->pin_BO1 = pin_init("BO1", ANALOG);
    chip->pin_BO2 = pin_init("BO2", ANALOG);

    const pin_watch_config_t watch_config = {
        .edge = BOTH,
        .pin_change = chip_pin_change,
        .user_data = chip,
    };

    pin_watch(chip->pin_PWMA, &watch_config);
    pin_watch(chip->pin_PWMB, &watch_config);
    pin_watch(chip->pin_AIN1, &watch_config);
    pin_watch(chip->pin_AIN2, &watch_config);
    pin_watch(chip->pin_BIN1, &watch_config);
    pin_watch(chip->pin_BIN2, &watch_config);
    pin_watch(chip->pin_STBY, &watch_config);
    pin_watch(chip->pin_VM, &watch_config);
}

void chip_pin_change(void *user_data, pin_t pin, uint32_t value)
{
    chip_state_t *chip = (chip_state_t *)user_data;
    float PWMA = pin_adc_read(chip->pin_PWMA);
    float PWMB = pin_adc_read(chip->pin_PWMB);
    int AIN1 = pin_read(chip->pin_AIN1);
    int AIN2 = pin_read(chip->pin_AIN2);
    int BIN1 = pin_read(chip->pin_BIN1);
    int BIN2 = pin_read(chip->pin_BIN2);
    int VM = pin_adc_read(chip->pin_VM);

    // Control del motor A
    if (AIN1)
        // pin_dac_write(chip->pin_AO1, PWMA);
        pin_write(chip->pin_AO1, 1);
    else
        pin_write(chip->pin_AO1, 0);
    if (AIN2)
        // pin_dac_write(chip->pin_AO2, PWMA);
        pin_write(chip->pin_AO2, 1);
    else
        pin_write(chip->pin_AO2, 0);
    // Control del motor B
    if (BIN1)
        // pin_dac_write(chip->pin_BO1, PWMB);
        pin_write(chip->pin_BO1, 1);
    else
        pin_write(chip->pin_BO1, 0);
    if (BIN2)
        // pin_dac_write(chip->pin_BO2, PWMB);
        pin_write(chip->pin_BO2, 1);
    else
        pin_write(chip->pin_BO2, 0);
}
