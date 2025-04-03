class CoreComms{
        private:
                int taskType;
                struct task_queue_task task;

                bool pop(void);
		
		unsigned char iv[16];
		unsigned char inKey[32];
		unsigned char outKey[32];
		unsigned char padding[32];
		unsigned char*CORE_ROUTING_KEY;
		size_t CORE_ROUTING_KEY_SIZE;
		Cryptography *cryptography;

		void deriveRoutingIv(void);
		void deriveRoutingRecvKey(void);
        	void deriveRoutingSendKey(void);
		void deriveRoutingPadding(void);

		bool validatePadding(unsigned char *data, size_t dataSize);
        public:
                CoreComms(void);

		void init(Cryptography *c, unsigned char*CORE_ROUTING_KEY, size_t CORE_ROUTING_KEY_SIZE);

                bool fetchTask(void);
                bool runTask(void);
                
};
