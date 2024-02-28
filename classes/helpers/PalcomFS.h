const char *pfs_relayAuth = "/auth.txt";

#define __GLOBAL_BUFFER_SIZE 1000
unsigned char fileData[__GLOBAL_BUFFER_SIZE] = {0};
char compBuffer[__GLOBAL_BUFFER_SIZE] = {0};
char fileNameBuffer[128];

class PalcomFS{
  private:
  
  public:
  File fd;

	void mkDir(String target){
		if(!SD.exists(target.c_str())){
			SD.mkdir(target.c_str());
		}
	}

	bool exists(String target){
		Serial.printf("Testing if %s exists.\n", target.c_str());
		if(!SD.exists(target.c_str()))
			return false;
		return true;
	}

	AuthFile fetchAuthFile(void){
		this->clearFileBuffer();
		AuthFile ret;
		this->fd = SD.open(pfs_relayAuth);
		size_t s = this->fd.size();
		fd.read(fileData, s);
		this->close();
		int context = 0;
		for(int i=0; i<s; i++){
			switch(context){
				case 0:
					if(fileData[i] == '\n'){
						context = 1;
						break;
					}
					ret.ssid += fileData[i];
					break;
				case 1:
					if(fileData[i] == '\n'){
                                                context = 2;
                                                break;
                                        }
                                        ret.ssidPassword += fileData[i];
					break;
				case 2:
                                        if(fileData[i] == '\n'){
                                                context = 3;
                                                break;
                                        }
                                        ret.relayUsername += fileData[i];
                                        break;
				case 3:
                                        if(fileData[i] == '\n'){
                                                context = 4;
						i = s+0;
                                                break;
                                        }
                                        ret.relayPassword += fileData[i];
                                        break;
			}
		}
		this->clearFileBuffer();
		return ret;
	}

  void rm(const char *target){
    if(!SD.exists(target))
      return;
      File root = SD.open(target);
      if(root.isDirectory()){
        while(true){
          File node = root.openNextFile();
          if(!node)
            break;
          String name;
          name = target;
          if(name != "/")
            name += "/";
          name += node.name();
          if(node.isDirectory()){
            this->rm(name.c_str());
            node.close();
            if(!SD.rmdir(name.c_str()))
              Serial.printf("Failed to remove directory %s\n", name.c_str());
          }else{
            node.close();
            SD.remove(name.c_str());
          }
        }
        root.close();
        if(!SD.rmdir(target))
          Serial.printf("Failed to remove directory %s\n", (const char *)target);
      }else{
        root.close();
        SD.remove(target);
      }
  }

  const char *getFilenameByPos(int id, const char *targetDir){
    if(SD.exists(targetDir)){
      int buttonCount = 0;
      File root = SD.open(targetDir);
      while(true){
        if(buttonCount > 25)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(buttonCount == id){
          const char *ret = node.name();
          node.close();
          root.close();
         
          return ret;
        }

        node.close(); 
      }
      root.close();
    }
    return NULL;
  }

  void close(void){
    if(fd)
      fd.close();
  }

  void write(unsigned char *buf, size_t size){
    fd.write(buf, size);
  }
  void clearFileBuffer(void){
    for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
      fileData[i] = 0;
  }
  void clearCompBuffer(void){
    for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
      compBuffer[i] = 0;
  }

  void clearAllBuffers(void){
    for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
      compBuffer[i] = 0;
      fileData[i] = 0;
    }
  }

  size_t fileAppend(const char *target, uint8_t * buf, size_t bufSize){
    File targetFile;
    size_t targetSize = 0;
    clearFileBuffer();

    if(!SD.exists(target)){
      targetFile = SD.open(target, FILE_WRITE);
    }else{
      targetFile = SD.open(target, FILE_READ);
      targetSize = targetFile.size();
      targetFile.read(fileData, targetSize);
      targetFile.close();
      targetFile = SD.open(target, FILE_WRITE, O_TRUNC);
    }

    if(targetSize > 0)
      targetFile.write(fileData, targetSize);
    
    targetFile.write(buf, bufSize);
    targetFile.close();
    targetFile = SD.open(target, FILE_READ);
    targetSize = targetFile.size();
    targetFile.close();

    return targetSize;
  }

};
