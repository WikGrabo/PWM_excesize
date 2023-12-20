/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <math.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "iot_button.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO_R         (23) // Define the 1st output GPIO
#define LEDC_OUTPUT_IO_G         (22) // Define the 2nd output GPIO
#define LEDC_OUTPUT_IO_B         (21) // Define the 3rd output GPIO
#define LEDC_CHANNEL_R          LEDC_CHANNEL_0
#define LEDC_CHANNEL_G          LEDC_CHANNEL_1
#define LEDC_CHANNEL_B          LEDC_CHANNEL_2
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz
#define BUTTON_PIN              (5)

bool button_pressed;

void example_ledc_init_R(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_R,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO_R,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void example_ledc_init_G(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_G,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO_G,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void example_ledc_init_B(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_B,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO_B,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void press (void *arg,void *usr_data)
    {
    button_pressed = !button_pressed;
    printf("Button pressed!");
    }

void button()
{
    //gpio_set_level(BUTTON_PIN, 1);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT_OUTPUT);

    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = BUTTON_PIN,
            .active_level = 0,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);

    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, press,NULL);
}

void app_main(void)
{
    button();
    example_ledc_init_R();
    example_ledc_init_G();
    example_ledc_init_B();
    
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, LEDC_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R));

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, LEDC_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G));

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, LEDC_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B));

    int dutyR = 4000;
    int dutyG = 4000;
    int dutyB = 4000;
    float r = -15;
    float g = 0;
    float b = 15;
    while(1)
    {

        if (button_pressed == true) 
        {
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, dutyR);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);
        dutyR = 10*(200*sin(0.1*r)+201);
        r = r+1;

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, dutyG);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);
        dutyG = 10*(200*sin(0.1*g)+201);
        g = g+1;

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, dutyB);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);
        dutyB = 10*(200*sin(0.1*b)+201);
        b = b+1;

        vTaskDelay(200/portTICK_PERIOD_MS);

        ESP_LOGW("INFO", "R=%d G=%d B=%d", dutyR, dutyG, dutyB);
        }

        else
        {
         ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, 0);
         ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);

         ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, 0);
         ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);

         ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, 0);
         ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);
        }
    }
        

}

