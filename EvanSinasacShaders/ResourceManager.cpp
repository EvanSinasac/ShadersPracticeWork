#include "ResourceManager.h"

#include <fstream>

ResourceManager::ResourceManager(void)
{
	// do nothing.
	return;
}

ResourceManager::~ResourceManager(void)
{
	// do nothing.
	return;
}

void ResourceManager::StartUp(void)
{
	this->m_uiResourceID = 100;
	return;
}

void ResourceManager::ShutDown(void)
{

	return;
}

bool ResourceManager::OpenBinaryFile(std::string filename, unsigned int *id)
{
	Resource* rsrc = new Resource();
	rsrc->stream.open(filename, std::ios_base::binary);
	if (!rsrc->stream.is_open())
	{
		rsrc->stream.close();
		delete rsrc;
		return false;
	}

	rsrc->filename = filename;
	rsrc->bFileOpen = true;
	rsrc->id = this->m_uiResourceID;

	this->m_map_pResouces.insert(std::pair<unsigned int, Resource*>(rsrc->id, rsrc));

	this->m_uiResourceID++;

	*id = rsrc->id;

	return true;
}

void ResourceManager::Seek(unsigned int id, unsigned long pos)
{
	if (this->m_map_pResouces[id]->stream.is_open())
	{
		//this->m_map_pResouces[id]->stream.seekg(0, pos);
		this->m_map_pResouces[id]->stream.seekg(pos);
	}

	return;
}

void ResourceManager::CloseFile(unsigned int id)
{
	if (this->m_map_pResouces[id]->stream.is_open())
	{
		this->m_map_pResouces[id]->stream.close();
	}
	this->m_map_pResouces[id]->stream.clear();
	this->m_map_pResouces.erase(id);
}

char ResourceManager::ReadAByte(unsigned int id)
{
	char theByte = 0;

	this->m_map_pResouces[id]->stream.get(theByte);

	return theByte;
}

unsigned short ResourceManager::ReadUnsignedShort(unsigned int id)
{
	unsigned short theUShort = 0;

	//theUShort |= ReadAByte(id) << 8;
	//theUShort |= ReadAByte(id); // << 0;

	//this->ReadData(id, (char*)&theUShort, sizeof(unsigned short));

	return theUShort;
}

unsigned long ResourceManager::ReadUnsignedLong(unsigned int id)
{
	unsigned long theULong = 0;

	//theULong |= ReadAByte(id) << 24;
	//theULong |= ReadAByte(id) << 16;
	//theULong |= ReadAByte(id) << 8;
	//theULong |= ReadAByte(id); // << 0;

	//this->ReadData(id, (char*)&theULong, sizeof(unsigned long));

	return theULong;
}

void ResourceManager::ReadData(const unsigned int id, unsigned short* data)
{
	//this->m_map_pResouces[id]->stream.get((char*)data, sizeof(unsigned short) + 1);
	//printf("%c%c\n", ((char*)data)[0], ((char*)data)[1]);
	this->ReadData(id, (char*)data, sizeof(unsigned short));
}

void ResourceManager::ReadData(const unsigned int id, unsigned int* data)
{
	this->ReadData(id, (char*)data, sizeof(unsigned int));
	//this->ReadData(id, (char*)&data, sizeof(unsigned int));
}

void ResourceManager::ReadData(const unsigned int id, unsigned long* data)
{
	this->ReadData(id, (char*)data, sizeof(unsigned long));
	//this->ReadData(id, (char*)&data, sizeof(unsigned long));
}

void ResourceManager::ReadData(const unsigned int id, char* data, unsigned int count)
{
	this->m_map_pResouces[id]->stream.read(data, count);
}