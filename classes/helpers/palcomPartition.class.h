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

		bool getAllPartitions(void){
			this->_partitionIterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
			if(this->_partitionIterator == NULL){
				return false;
			}
			return true;
		}

		void freePartitions(void){
			if(this->_partitionIterator != NULL){
				esp_partition_iterator_release(this->_partitionIterator);
				this->_partitionIterator = NULL;
			}
			if(this->partition != NULL){
				this->partition = NULL;
			}
		}

		bool fetchPartition(void){
			if(this->_partitionIterator == NULL)
				return false;
			this->partition = (esp_partition_t*)esp_partition_get(this->_partitionIterator);
			this->_partitionIterator = esp_partition_next(this->_partitionIterator);
			return true;

		}

		bool eraseRange(const esp_partition_t *partition, size_t offset, size_t eraseRange){
			if(partition == NULL)
				return false;
			esp_err_t err = esp_partition_erase_range(partition, offset, eraseRange);
			if(err == ESP_OK){
				return true;
			}else if(err == ESP_ERR_INVALID_ARG){
				return false;
			}else if(err == ESP_ERR_INVALID_SIZE){
				return false;
			}else{
				return false;
			}
		}

		bool readPartition(const esp_partition_t *partition, size_t offset, void *outBuf, size_t outBufSize){
			if(partition == NULL)
				return false;
			
			esp_err_t err = esp_partition_read(partition, offset, outBuf, outBufSize);
			if(err == ESP_OK){
				return true;
			}else if(err == ESP_ERR_INVALID_ARG){
				return false;
			}else if(err == ESP_ERR_INVALID_SIZE){
				return false;
			}else{
				return false;
			}
		}

		bool writePartition(const esp_partition_t *partition, size_t offset, const void *src, size_t srcSize){
			if(partition == NULL)
				return false;
			esp_err_t err = esp_partition_write(partition, offset, src, srcSize);
			if(err == ESP_OK){
				return true;
			}else if(err == ESP_ERR_INVALID_ARG){
				return false;
			}else if(err == ESP_ERR_INVALID_SIZE){
				return false;
			}else{
				return false;
			}
		}

		bool writeData(const esp_partition_t *partition, palcom_partition_t data){
			if(!this->eraseRange(partition, 0, partition->size))
				return false;
			return this->writePartition(partition, 0, (const void *)&data, sizeof(palcom_partition_t));
		}

		bool write(palcom_partition_t data){
			if(!fetchPartitionByName(defualtPartition)){
				return false;
			}

			bool ret = writeData((const esp_partition_t *)partition, data);
			freePartitions();
			return ret;
		}

		bool read(palcom_partition_t *ret){
			if(!fetchPartitionByName(defualtPartition)){
				return false;
                        }
			
			bool retu = readData((const esp_partition_t *)partition, ret);
			freePartitions();
			return retu;
		}

		/*
		 * This function will free all partitions if it fails to find one.
		 * */
		bool fetchPartitionByName(String name){
			this->getAllPartitions();
			while(this->fetchPartition()){
				String comp = (const char *)this->partition->label;
				if(comp == name){
					return true;
				}
			}

			this->freePartitions();
			return false;
		}

		bool readData(const esp_partition_t *partition, palcom_partition_t *ret){
			return this->readPartition(partition, 0, (void *)ret, sizeof(palcom_partition_t));
		}
};
