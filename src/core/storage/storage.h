class CoreStorage{
        private:
                int taskType;
                struct task_queue_task task;

                bool pop(void);
        public:
                CoreStorage(void);

                bool fetchTask(void);
                bool runTask(void);
                
};

