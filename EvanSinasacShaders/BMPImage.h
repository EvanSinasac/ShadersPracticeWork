#ifndef BMPImage_HG
#define BMPImage_HG

#include <string>

class BMPImage
{
public:
	BMPImage(const std::string &filename);
	virtual ~BMPImage(void);

	bool IsLoaded(void);
	unsigned long GetFileSize(void);
	unsigned long GetImageWidth(void);
	unsigned long GetImageHeight(void);
	unsigned long GetImageSize(void);
	char* GetData(void);

private:
	BMPImage(void);
	bool LoadBMPFromFile(const std::string &filename);

	bool m_bIsLoaded;
	unsigned long m_FileSize;
	unsigned long m_ImageWidth;
	unsigned long m_ImageHeight;
	unsigned long m_ImageSize;
	char* m_pData;
};

#endif
