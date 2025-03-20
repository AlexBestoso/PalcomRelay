#include <Arduino.h>
#include <cstdint>
#include <ds3231.h> 
#include <src/error/error.h>
#include <src/taskQueue/taskQueue.h>
                
#include "./comms.h"
                
extern OLED_CLASS_OBJ display;
extern TaskQueue taskQueue;
                
CoreComms::CoreComms(void){
        this->taskType = TASK_SPACE_COMMS;
}

bool CoreComms::pop(void){
        struct task_queue_task t = taskQueue.pop(this->taskType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool CoreComms::fetchTask(void){
        return this->pop();
}

bool CoreComms::runTask(void){
        switch(this->task.instruction){
                case COMMS_INSTR_RECV:{
                        display.clear();
                        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "RECEIVED!\n");
                        display.display();
                }
                return true;
                //default: Do nothing.
        }
        return false;
}
