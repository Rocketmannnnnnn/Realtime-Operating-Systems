#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include<stdio.h>
#include <NoRTOS.h>
#include <ti/drivers/GPIO.h>

#include "ti_drivers_config.h"
#include <ti/devices/cc32xx/driverlib/systick.h>

//Enums & constants
enum STATES { waiting, ready };
enum COLOUR { red, yellow, green };

//Structs
typedef struct {
    enum STATES state;
    int periodCounter;
    int totalPeriod;
    int delay;
    int totalDelay;
    void (*pointerNaarFunc)(int, int);
} Task;

typedef struct {
    void* next;
    Task task;
} TaskHolder;

//Globals
static TaskHolder* head = NULL;

//Function definitions;
void addTask(enum COLOUR color);
void runReadyTasks();
void setRed(int power);
void setYellow(int power);
void setGreen(int power);

void INT() {
    TaskHolder* node;
    node = head;

    while (node != NULL) {
        switch (node->task.state) {
        case waiting:
            if (node->task.delay <= 0) {
                node->task.state = ready;
            }
            else {
                node->task.delay--;
            }
            break;
        case ready:
            if (node->task.periodCounter <= 0) {
                node->task.state = waiting;
                node->task.delay = node->task.totalDelay;
                node->task.periodCounter = node->task.totalPeriod;
            }
            else {
                node->task.periodCounter--;
            }
            break;
        default:
            break;
        }
        node = node->next;
    }
}

void addTask(enum COLOUR color) {
    TaskHolder* t;
    t = (TaskHolder*)malloc(sizeof(TaskHolder));
    if (head == NULL) {
        head = t;
        head->next = NULL;
    }
    else {
        t->next = head;
        head = t;
    }
    head->task.state = waiting;

    if(color == red){
        head->task.delay = 200;
        head->task.totalDelay = 200;
        head->task.periodCounter = 100;
        head->task.totalPeriod = 100;
        head->task.pointerNaarFunc = setRed;
    }else if (color == yellow){
        head->task.delay = 500;
        head->task.totalDelay = 500;
        head->task.periodCounter = 200;
        head->task.totalPeriod = 200;
        head->task.pointerNaarFunc = setYellow;
    }else if (color == green){
        head->task.delay = 750;
        head->task.totalDelay = 750;
        head->task.periodCounter = 300;
        head->task.totalPeriod = 300;
        head->task.pointerNaarFunc = setGreen;
    }

}

void runReadyTasks() {
    TaskHolder* node;
    node = head;
    while (node != NULL) {
        if (node->task.state == ready) {
            node->task.pointerNaarFunc(node->task.periodCounter, CONFIG_GPIO_LED_ON);
        }
        node = node->next;
    }
}

void setRed(int power){
    GPIO_write(CONFIG_GPIO_LED_RED, power);
}

void setYellow(int power){
    GPIO_write(CONFIG_GPIO_LED_YELLOW, power);
}

void setGreen(int power){
    GPIO_write(CONFIG_GPIO_LED_GREEN, power);
}

int main() {
    Board_init();
    NoRTOS_start();
    GPIO_init();

    //set period. 1 milli second is 80000. Multiply by 1000 and get a second
    SysTickPeriodSet(80000);
    //Register interrupt
    SysTickIntRegister(INT);
    //enable
    SysTickEnable();

    //ADD STRUCTS
    addTask(red);
    addTask(yellow);
    addTask(green);

    while (1) {
        //SET ALL LEDS OFF
        for (int i = 0; i < 3; i++) {
            setLed(i, CONFIG_GPIO_LED_OFF);
        }
        runReadyTasks();
        PRCMSleepEnter();
    }
}
