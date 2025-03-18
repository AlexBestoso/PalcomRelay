class CoreGraphics{
	private:
		int taskType;
		struct task_queue_task task;

		bool pop(void);
	public:
		CoreGraphics(void);

		bool fetchTask(void);
		bool runTask(void);
		
};
