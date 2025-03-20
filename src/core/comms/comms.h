class CoreComms{
        private:
                int taskType;
                struct task_queue_task task;

                bool pop(void);
        public:
                CoreComms(void);

                bool fetchTask(void);
                bool runTask(void);
                
};
