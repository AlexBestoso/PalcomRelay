#include <Arduino.h>
#include <cstdint>
#include <ds3231.h>
#include <src/error/error.h>
#include <src/taskQueue/taskQueue.h>

#include "./graphics.h"

extern OLED_CLASS_OBJ display;
extern TaskQueue taskQueue;

CoreGraphics::CoreGraphics(void){
	this->taskType = TASK_SPACE_GRAPHICS;
}

bool CoreGraphics::pop(void){
        struct task_queue_task t = taskQueue.pop(this->taskType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool CoreGraphics::fetchTask(void){
	return this->pop();
}

bool CoreGraphics::runTask(void){
	switch(this->task.instruction){
		case GRAPHICS_INSTR_HOMEPAGE:{
			display.clear();
        		display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Palnode\n");
        		display.display();
		}
		return true;
		//default: Do nothing.
	}
	return false;
}
