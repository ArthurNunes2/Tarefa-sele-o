//includes
#include <esp_wifi_types.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
bool bot=false,lastbot=false;
int y=0,a=2,count=0,x=0;
/*
 *  TASK 
 */
void led(void *pvParamenters)
{
    while(1){
        lastbot=bot;
        bot=gpio_get_level(GPIO_NUM_25);
        if(bot==true && lastbot==false)
            y^=1;
        gpio_set_level(GPIO_NUM_32, y);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
/*
 *  CONFIGURAÇÕES DO LED
 */
void configled()
{
    ledc_timer_config_t tim;
    tim.duty_resolution = LEDC_TIMER_8_BIT; // resolution of PWM duty
    tim.freq_hz = 120;                      // frequency of PWM signal
    tim.speed_mode = LEDC_LOW_SPEED_MODE;           // timer mode
    tim.timer_num = LEDC_TIMER_1;            // timer index
    tim.clk_cfg = LEDC_AUTO_CLK;              // Auto select the source clock
    ledc_timer_config(&tim);
    
    ledc_channel_config_t canal;
    canal.channel    = LEDC_CHANNEL_0;
    canal.duty       = 0;
    canal.gpio_num   = 27;
    canal.speed_mode = LEDC_LOW_SPEED_MODE;
    canal.hpoint     = 1;
    canal.timer_sel  =  LEDC_TIMER_1;
    ledc_channel_config(&canal);
    }
void inicia()
{
  
    printf("Hello world!\n");
    printf("digite 3 para continuar o programa\n");
    while(a!=3)
        {
         vTaskDelay(1000 / portTICK_PERIOD_MS);
         scanf("\n %d ",&a);
        }
    printf("o programa irá continuar...\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_direction(GPIO_NUM_32,GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_25,GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_25, GPIO_PULLDOWN_ONLY);
   
   
}
void iniciawifi()
{
     //ISSO TUDO É WIFI
    nvs_flash_init();
    wifi_init_config_t Dcfg= WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&Dcfg);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "NetVirtua53",
            .password = "10067990",
        },
    };
   
    esp_wifi_set_mode(WIFI_MODE_STA) ;
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) ;
    esp_wifi_start() ;
    esp_wifi_connect();
    //ISSO TUDO É WIFI
}

//void app_main
void app_main(void)
{   
    inicia();
    configled();
    while(1){
        //subindo
        if(x==0){
            count++;
            if(count==255)
                x=1;
        }
        else if(x==1){
            count--;
            if(count==0){
                x=0;
                break;
                }
                
        }
        ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,count); 
        ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
            }  
    iniciawifi();
    xTaskCreatePinnedToCore(led,"nome",4098,NULL,2,NULL,1);
    
    while(1){
        //subindo
        if(x==0&&y==1){
            count++;
            if(count==255)
                x=1;
        }
        else if(x==1&&y==1){
            count--;
            if(count==0)
                x=0;
        }
        else {
            x=0;
            count=0;
        }
        ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,count); 
        ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
            }
}


