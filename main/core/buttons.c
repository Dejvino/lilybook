
#include <stdlib.h>
#include "driver/gpio.h"
#include "buttons.h"

#define BTN_PIN_PLUS ((gpio_num_t)39)
#define BTN_PIN_OK ((gpio_num_t)37)
#define BTN_PIN_MINUS ((gpio_num_t)38)

void buttons_init()
{
    gpio_config_t buttons_config;
    buttons_config.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
    buttons_config.mode = GPIO_MODE_INPUT;
    buttons_config.pull_up_en = (gpio_pullup_t)1;
    buttons_config.pull_down_en = (gpio_pulldown_t)0;
    buttons_config.pin_bit_mask = (1LL << BTN_PIN_MINUS);
    gpio_config(&buttons_config);
    buttons_config.pin_bit_mask = (1LL << BTN_PIN_OK);
    gpio_config(&buttons_config);
    buttons_config.pin_bit_mask = (1LL << BTN_PIN_PLUS);
    gpio_config(&buttons_config);
}

bool buttons_pressed_ok() { return gpio_get_level(BTN_PIN_OK) == 0; }
bool buttons_pressed_plus() { return gpio_get_level(BTN_PIN_PLUS) == 0; }
bool buttons_pressed_minus() { return gpio_get_level(BTN_PIN_MINUS) == 0; }
