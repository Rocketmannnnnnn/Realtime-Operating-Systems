
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include<stdio.h>
#include <NoRTOS.h>
#include <ti/drivers/GPIO.h>

#include "ti_drivers_config.h"
#include <ti/devices/cc32xx/driverlib/systick.h>

//Enums & constants
enum STATE { waiting, ready, finished };
enum COLOR { red, yellow, green };
int periods[3] = { 200, 500, 750 };
int delays[3] = { 100, 200, 300 };

//Structs
typedef struct {
    enum STATE state;
    enum COLOR color;
    int counter;
    int delay;
    void (*pointerNaarFunc)(int, int);
} Task;

typedef struct {
    void *prev;
    void *next;
    Task task;
} TaskHolder;

//Globals
static TaskHolder *head = NULL;

//Function defenitions;
void addTask(enum COLOR color);
TaskHolder* getTask(int i);
int taskCount();
void runReadyTasks();
void setLed(int ledIndex, int power);

void INT(){
    TaskHolder* node;
    node = head;

    while (1)
    {
        if (!node)
            return;

        switch(node->task.state){
            case waiting:
                if(node->task.delay <= 0){
                    node->task.state = ready;
                }
                else{
                    node->task.delay--;
                }
                break;
            case ready:
                if(node->task.counter <= 0){
                    node->task.state = finished;
                }
                else{
                    node->task.counter--;
                }
                break;
            default:
                break;
        }
        node = node->next;
    }
}

void addTask(enum COLOR color){
    TaskHolder *t;
    t = (TaskHolder*) malloc(sizeof(TaskHolder));
    if(head == NULL){
        head = t;
        head->next = NULL;
    }
    else
    {
        t->next = head;
        head->prev = t;
        head = t;
    }
    head->prev = NULL;
    head->task.state = waiting;
    head->task.color = color;
    head->task.delay = delays[color];
    head->task.counter = periods[color];
    head->task.pointerNaarFunc = setLed;
}

TaskHolder* getTask(int i){
    int iterator = 0;
    TaskHolder* node;
    node = head;

    while (1)
    {
        if (!node)
            return NULL;

        if (iterator == i)
            return node;
        else
            iterator++;

        node = node->next;
    }
}

int taskCount(){
    int iterator = 0;
    TaskHolder* node;
    node = head;

    while (1)
    {
        if (!node)
            return iterator;

        node = node->next;
        iterator++;
    }
}

void runReadyTasks(){
    TaskHolder* node;
        node = head;

        while (1)
        {
            if (!node)
                return;

            if(node->task.state == ready){
                node->task.pointerNaarFunc(node->task.color, CONFIG_GPIO_LED_ON);
            }
            node = node->next;
        }
}

void setLed(int ledIndex, int power){
    if(ledIndex == 0){
        GPIO_write(CONFIG_LED_0_GPIO, power);
    }
    if(ledIndex == 1){
        GPIO_write(CONFIG_LED_1_GPIO, power);
    }
    if(ledIndex == 2){
        GPIO_write(CONFIG_LED_2_GPIO, power);
    }
}


int main(){
    Board_init();
    NoRTOS_start();

    GPIO_init();

    //set period. 1 milli second is 80000. Multiply by 1000 and get a second
    SysTickPeriodSet(80000);
    //enable
    SysTickEnable();
    //Register interrupt
    SysTickIntRegister(INT);

    //ADD STRUCTS
    addTask(red);
    addTask(red);
    addTask(red);
    addTask(yellow);
    addTask(yellow);
    addTask(yellow);
    addTask(green);
    addTask(green);

    while(1){
        //SET ALL LEDS OFF
        int i;
        for(i = 0; i < 3; i++){
            setLed(i, CONFIG_GPIO_LED_OFF);
        }
        runReadyTasks();
        PRCMSleepEnter();
    }
}
