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
int periodTime[3] = { 200, 500, 750 }; //Length of period
int delayTime[3] = { 100, 200, 300 };

//Structs
typedef struct {
    enum STATES state;
    enum COLOUR color;
    int counter;
    int delay;
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
void setLed(int ledIndex, int power);

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
            if (node->task.counter <= 0) {
                node->task.state = waiting;
                node->task.delay = delayTime[node->task.color];
                node->task.counter = periodTime[node->task.color];
            }
            else {
                node->task.counter--;
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
    head->task.color = color;
    head->task.delay = delayTime[color];
    head->task.counter = periodTime[color];
    head->task.pointerNaarFunc = setLed;
}

void runReadyTasks() {
    TaskHolder* node;
    node = head;
    while (node != NULL) {
        if (node->task.state == ready) {
            node->task.pointerNaarFunc(node->task.color, CONFIG_GPIO_LED_ON);
        }
        node = node->next;
    }
}

void setLed(int ledIndex, int power) {
    if (ledIndex == 0) {
        GPIO_write(CONFIG_GPIO_LED_GREEN, power);
    }
    if (ledIndex == 1) {
        GPIO_write(CONFIG_GPIO_LED_YELLOW, power);
    }
    if (ledIndex == 2) {
        GPIO_write(CONFIG_GPIO_LED_RED, power);
    }
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