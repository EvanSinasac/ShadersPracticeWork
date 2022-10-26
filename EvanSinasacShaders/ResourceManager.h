#ifndef ResourceManager_HG
#define ResourceManager_HG

#include <map>
#include <fstream>

class Resource
{
public:
	std::ifstream stream;
	unsigned long filesize;
	std::string filename;
	std::string name;
	bool bFileOpen;
	unsigned int id;
};

class ResourceManager
{
public:
	ResourceManager(void);
	virtual ~ResourceManager(void);

	void StartUp(void);
	void ShutDown(void);

	bool OpenBinaryFile(std::string filename, unsigned int *id);
	void CloseFile(unsigned int id);

	// Reading information
	char ReadAByte(unsigned int rsrcID);
	unsigned short ReadUnsignedShort(unsigned int id);
	unsigned long ReadUnsignedLong(unsigned int id);

	void Seek(const unsigned int id, unsigned long pos);

	void ReadData(const unsigned int id, unsigned short* data);
	void ReadData(const unsigned int id, unsigned int* data);
	void ReadData(const unsigned int id, unsigned long* data);
	void ReadData(const unsigned int id, char* data, unsigned int count);
private:
	unsigned int m_uiResourceID;
	std::map<unsigned int, Resource*> m_map_pResouces;
};

#endif
