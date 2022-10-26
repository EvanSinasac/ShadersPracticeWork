#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>
#include <list>

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
//struct sVertex
//{
//	float x, y, z;		
//	float r, g, b;
//};

//#include "sVertex_XYZ_RGB.h"
#include "sVertex_types.h"


struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;	// Is also the file name (i.e. what we search for)

	std::string fileNameWithPath;	// What we actual load from

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex_XYZW_RGBA_N_UV_T_B* pVertices;	//  = 0;
//	sVertex_XYZW_RGBA* pVertices;	//  = 0;
//	sVertex_XYZ_RGB* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	sVertex_XYZW_RGBA_N_UV_T_B maxValues;
	sVertex_XYZW_RGBA_N_UV_T_B minValues;
	//	scale = 5.0/maxExtent;		-> 5x5x5
	sVertex_XYZW_RGBA_N_UV_T_B maxExtents;	
	float maxExtent;

	void calculateExtents(void);

	// This is for the threading loading, etc.
	bool bReadFromFile;		// Is false if it's still pending a read
	bool bLoadedIntoVAO;	// Is false until it's loaded into the GPU
};


class cVAOManager
{
public:

	cVAOManager();

	bool LoadModelIntoVAO(std::string meshName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// Same as above, but doesn't load into the VAO
	// i.e. you'd call these in two steps, passing the filename in the drawInfo.meshName
	// Assumes the full file and path are also loaded.
	bool LoadPLYModelFromFile(sModelDrawInfo& drawInfo);
	// Uses a populated drawInfo structure, with the filename in the drawInfo.meshName
	bool LoadModelIntoVAO(sModelDrawInfo& drawInfo, unsigned int shaderProgramID);

	// Unfinished example of reading the entire file in one call...
	bool LoadPLYModelFromFile_SupahFastSpeedyWayLikeABoss(std::string fileName, sModelDrawInfo& drawInfo);


	// You could write this is you didn't want the sModelDrawInfo to return by ref
	bool LoadModelIntoVAO(std::string fileName, 
						  unsigned int shaderProgramID);

	// We don't want to return an int, likely
	// UPDATED: if it DOESN'T find the model, then it returns a "pending model loading" model
	// (We used a teapot for this)
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo,
								 bool bReturnPendingLoadingModelIfNotFound = true);

	std::string getLastError(bool bAndClear = true);

	void setFilePath(std::string filePath);
	std::string getFilePath(void);

	// This is modified from GenerateSphericalTextureCoords.cpp file,
	//	specifically that it's using the sModelDrawInfo structure
	enum enumTEXCOORDBIAS {
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};
	void GenerateSphericalTextureCoords(
		enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias,
		sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast);
	// Defaults to POSITIVE_X, POSITIVE_Y
	void GenerateSphericalTextureCoords(
		sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast);

	// Added November 24, 2021: To handle the cylindrical projection UV mapping for applying planet textures to spheres
	// This projects on a cylinder that has an axis along the y-axis.
	void GenerateCylindricalTextureCoords_FromNormals(sModelDrawInfo& drawInfo);

	// Added November 24, 2021: To save the planet model so we can reload it
	// The ".ply" is added automatically.
	bool SaveAsPlyFile(sModelDrawInfo& drawInfo, std::string fileName, bool bOverwrite,
					   bool bSaveRGB, bool bSaveNormals, bool bSaveUVs, 
					   std::string &error);


	// ****************************************************************************************
	//    _____ _                        _          _   _                 _           
	//   |_   _| |__  _ __ ___  __ _  __| | ___  __| | | | ___   __ _  __| | ___ _ __ 
	//     | | | '_ \| '__/ _ \/ _` |/ _` |/ _ \/ _` | | |/ _ \ / _` |/ _` |/ _ \ '__|
	//     | | | | | | | |  __/ (_| | (_| |  __/ (_| | | | (_) | (_| | (_| |  __/ |   
	//     |_| |_| |_|_|  \___|\__,_|\__,_|\___|\__,_| |_|\___/ \__,_|\__,_|\___|_|   
	//                                                                                
	// This is a blocking call to indicate which model to return if a model isn't loaded yet
	void SetShaderProgramID_Threaded(unsigned int shaderProgramID);
	bool LoadModelIntoVAO_Threaded(std::string meshName, 
								   sModelDrawInfo &drawInfo);
	bool Load_PendingLoadingModel_IntoVAO(std::string meshName, 
										  sModelDrawInfo &drawInfo);
	// Called every frame (until all the models are loaded)
	void Update(double deltaTime);	// Delta time because... I'm not sure
	// This will add the model onto the "to be loaded" list
	// ****************************************************************************************

private:
	// For the threaded loader, holding the pending "models to be loaded"
	std::list< sModelDrawInfo* > m_list_pPendingModelsToLoad;
	// Assumption is that all models VAOs are being bound to the same shader program
	unsigned int m_shaderProgramID_ThreadedLoader;
	
	// The model that we return if the async models haven't loaded yet
	sModelDrawInfo* m_pPendingLoadingModel;

	// This is true when the thread exists and is currently running
	bool m_bWorkerThreadIsRunning = false;
	// *******************************************************************

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// This will be added to the path when a file is loaded
	std::string m_FilePath;

	std::string m_lastError;
};

#endif	// _cVAOManager_HG_
