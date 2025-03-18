struct palcom_partition{
	int mode = 0;
};
typedef struct palcom_partition palcom_partition_t;

class PalcomPartition{
	private:
		esp_partition_iterator_t _partitionIterator = NULL;
		const char *defualtPartition = "app1";
	public:
		esp_partition_t *partition = NULL;

		bool getAllPartitions(void);
		void freePartitions(void);
		bool fetchPartition(void);
		bool eraseRange(const esp_partition_t *partition, size_t offset, size_t eraseRange);
		bool readPartition(const esp_partition_t *partition, size_t offset, void *outBuf, size_t outBufSize);
		bool writePartition(const esp_partition_t *partition, size_t offset, const void *src, size_t srcSize);
		bool writeData(const esp_partition_t *partition, palcom_partition_t data);
		bool write(palcom_partition_t data);
		bool read(palcom_partition_t *ret);
		bool fetchPartitionByName(String name);
		bool readData(const esp_partition_t *partition, palcom_partition_t *ret);
};
