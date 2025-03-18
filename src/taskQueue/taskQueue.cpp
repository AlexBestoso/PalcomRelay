#include <Arduino.h>
#include <cstdint>
#include <src/error/error.h>
#include "./taskQueue.h"

extern SemaphoreHandle_t queueSemaphore = NULL;
// Private
bool TaskQueue::lock(void){
	if(queueSemaphore == NULL)
		throw CoreException("Semaphore Null", ERR_TQ_SEMAPHORE);
	if(this->locked)
		return false;
	if(xSemaphoreTake(queueSemaphore, portMAX_DELAY) == pdTRUE){
		this->locked = true;
		return true;
	}
	return false;
}

void TaskQueue::unlock(void){
	xSemaphoreGive(queueSemaphore);
	this->locked = false;	
}

// Public
TaskQueue::TaskQueue(){
	queueSemaphore = xSemaphoreCreateBinary();
	this->unlock();
	this->taskCount = 0;
}

TaskQueue::~TaskQueue(){}


void TaskQueue::push(uint8_t to, uint8_t from, uint8_t instruction){
	this->push(this->buildTask(to, from, instruction));
}
void TaskQueue::push(struct task_queue_task task){
	if(!task.active)
		throw CoreException("Provided task inactive", ERR_TQ_POP);
	if(task.to <= 0)
		throw CoreException("To field empty", ERR_TQ_POP);
	if(task.from <= 0)
		throw CoreException("From field empty", ERR_TQ_POP);
	if(task.instruction <= 0)
		throw CoreException("Instruction field empty", ERR_TQ_POP);
	if(taskCount >= TASK_QUEUE_MAX)
		throw CoreException("Task count causes overflow", ERR_TQ_POP);

	while(!this->lock()){delay(10);}
	if(this->taskCount > 0){
		int i=(this->tasks[this->taskCount].active) ? 0 : this->taskCount;
		for(i=0; i<TASK_QUEUE_MAX; i++){
			if(!this->tasks[i].active){
				this->tasks[i] = task;
				this->taskCount++;
				break;
			}
		}
	}else{
		this->tasks[0] = task;
		this->taskCount++;
	}
	this->unlock();
}
void TaskQueue::push(struct task_queue_task *task){
	if(task == NULL){
		Serial.printf("Task is null.\n");
		throw CoreException("Task is null.\n", ERR_TASK_NULL);
	}
	if(!task->active)
		throw CoreException("Provided task inactive", ERR_TQ_POP);
	if(task->to <= 0)
		throw CoreException("To field empty", ERR_TQ_POP);
	if(task->from <= 0)
		throw CoreException("From field empty", ERR_TQ_POP);
	if(task->instruction <= 0)
		throw CoreException("Instruction field empty", ERR_TQ_POP);
	if(taskCount >= TASK_QUEUE_MAX)
		throw CoreException("Task count causes overflow", ERR_TQ_POP);

	while(!this->lock()){delay(10);}
	if(this->taskCount > 0){
		int i=(this->tasks[this->taskCount].active) ? 0 : this->taskCount;
		for(i=0; i<TASK_QUEUE_MAX; i++){
			if(!this->tasks[i].active){
        			this->tasks[i].active=true;
        			this->tasks[i].to=task->to; 
        			this->tasks[i].from=task->from;
        			this->tasks[i].instruction=task->instruction;
				for(int j=0; j<256; j++)
	        			this->tasks[i].msg[j] = task->msg[j];
  
				this->taskCount++;
				break;
			}
		}
	}else{
		this->tasks[0].active=true;
		this->tasks[0].to=task->to;
		this->tasks[0].from=task->from;
		this->tasks[0].instruction=task->instruction;
		for(int j=0; j<256; j++)
			this->tasks[0].msg[j] = task->msg[j];

		this->taskCount++;
	}
	this->unlock();
}
struct task_queue_task TaskQueue::pop(int spaceId){
	struct task_queue_task ret;
	
	while(!this->lock()){delay(10);}

	if(!this->mine(spaceId) || !this->tasks[0].active){
		this->unlock();
		return ret;
	}
	
	ret = this->tasks[0];

	for(int i=1;i<TASK_QUEUE_MAX; i++){
		tasks[i-1] = tasks[i];
	}
	this->taskCount -= (this->taskCount <= 0) ? 0 : 1;

	this->unlock();
	return ret;
}

bool TaskQueue::mine(int spaceID){
	return (this->tasks[0].to == spaceID);
}

struct task_queue_task TaskQueue::buildTask(uint8_t to, uint8_t from, uint8_t instruction){
	struct task_queue_task ret;
	ret.active = true;
	ret.to = to;
	ret.from = from;
	ret.instruction = instruction;
	return ret;
}

struct task_queue_task TaskQueue::buildTask(uint8_t to, uint8_t from, uint8_t instruction, uint8_t msg[256]){
        struct task_queue_task ret;
        ret.active = true;
        ret.to = to;
        ret.from = from;
        ret.instruction = instruction;
	for(int i=0; i<256; i++)
		ret.msg[i] = msg[i];
        return ret;
}
