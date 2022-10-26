#include "GLCommon.h"

#include "cVAOManager.h"

#include <glm/glm.hpp>              // For normalize
#include <glm/gtc/constants.hpp>    // For pi
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <sstream>
#include <fstream>


//bool LoadPLYModelFromFile(std::string fileName, sModelDrawInfo& drawInfo);



sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

    this->maxExtent = 0.0f;
    this->minValues.x = this->minValues.y = this->minValues.z = 0.0f;
    this->maxValues.x = this->maxValues.y = this->maxValues.z = 0.0f;

    // For the threading...
    this->bReadFromFile = false;
    this->bLoadedIntoVAO = false;

	return;
}

void sModelDrawInfo::calculateExtents(void)
{
    if ( this->pVertices == NULL )
    {
        // No vertices
        return;
    }

    this->minValues.x = this->maxValues.x = this->pVertices[0].x;
    this->minValues.y = this->maxValues.y = this->pVertices[0].y;
    this->minValues.z = this->maxValues.z = this->pVertices[0].z;
    
    for ( unsigned int index = 0; index != this->numberOfVertices; index++ )
    {
        if ( this->pVertices[index].x < this->minValues.x ) { this->minValues.x = this->pVertices[index].x; }
        if ( this->pVertices[index].y < this->minValues.y ) { this->minValues.y = this->pVertices[index].y; }
        if ( this->pVertices[index].z < this->minValues.z ) { this->minValues.z = this->pVertices[index].z; }

        if ( this->pVertices[index].x > this->maxValues.x ) { this->maxValues.x = this->pVertices[index].x; }
        if ( this->pVertices[index].y > this->maxValues.y ) { this->maxValues.y = this->pVertices[index].y; }
        if ( this->pVertices[index].z > this->maxValues.z ) { this->maxValues.z = this->pVertices[index].z; }
    }

    this->maxExtents.x = this->maxValues.x - this->minValues.x;
    this->maxExtents.y = this->maxValues.y - this->minValues.y;
    this->maxExtents.z = this->maxValues.z - this->minValues.z;

    this->maxExtent = this->maxExtents.x;
    if ( this->maxExtents.y > this->maxExtent ) { this->maxExtent = this->maxExtents.y; }
    if ( this->maxExtents.z > this->maxExtent ) { this->maxExtent = this->maxExtents.z; }

    return;
}

cVAOManager::cVAOManager()
{
    this->m_shaderProgramID_ThreadedLoader = 0;
    this->m_pPendingLoadingModel = NULL;
}

bool cVAOManager::LoadModelIntoVAO(
		std::string meshName, 
		sModelDrawInfo &drawInfo,
	    unsigned int shaderProgramID)
{
	// Load the model from file
	// (We do this here, since if we can't load it, there's 
	//	no point in doing anything else, right?)

    // UPDATE: We are going to add the filename and full file path, too
    drawInfo.meshName = meshName;
    
    // Add the path 
    drawInfo.fileNameWithPath = this->m_FilePath + drawInfo.meshName;

    // This does the loading from the hard drive (REALLY slowly)
    if ( !this->LoadPLYModelFromFile(drawInfo) )
    {
        return false;
    }
	// 
	// Model is loaded and the vertices and indices are in the drawInfo struct
	// 

    drawInfo.calculateExtents();

    // This is so fast, it's trivial. 
    return this->LoadModelIntoVAO(drawInfo, shaderProgramID);
}


// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		sModelDrawInfo &drawInfo,
        bool bReturnPendingLoadingModelIfNotFound /*=true*/) 
{
	std::map< std::string /*model name*/,
			sModelDrawInfo /* info needed to draw*/ >::iterator 
		itDrawInfo = this->m_map_ModelName_to_VAOID.find( filename );

	// Find it? 
	if ( itDrawInfo == this->m_map_ModelName_to_VAOID.end() )
	{
		// Nope, didn't find that model
        // Are we returning the "model still loading" model instead? 
        if (bReturnPendingLoadingModelIfNotFound)
        {
            // ...and did we actually load something like that??
            if (this->m_pPendingLoadingModel != NULL)
            {
                // Yes, we have an alternative "placeholder" model to reutrn 
                // (We loaded the Utah teapot for this)
                drawInfo = *(this->m_pPendingLoadingModel);
                return true;
            }

        }//if (bReturnPendingLoadingModelIfNotFound)

		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	drawInfo = itDrawInfo->second;
	return true;
}

bool cVAOManager::LoadPLYModelFromFile_SupahFastSpeedyWayLikeABoss(std::string fileName, sModelDrawInfo& drawInfo)
{
    // Add the path 
    std::string fileNameWithPath = this->m_FilePath + fileName;




    std::ifstream theFile(fileNameWithPath.c_str());   // Here's an example of using the underlying C IO call where you just 
    // load the entire file directly into memory.
    std::ifstream theFileFast;
    theFileFast.open( fileNameWithPath.c_str(), std::ios_base::binary );
    // How big (# bytes) is this file?
    theFileFast.seekg(0, std::ios::end);        // Scans to the end of the file
    std::ios::pos_type numBytesInFile = theFileFast.tellg();
    // Move to the start of the file
    theFileFast.seekg(0, std::ios::beg);

    char* pTheDataInTheFile = new char[(unsigned int)numBytesInFile];
    // Boom goes the dynomite! 
    theFileFast.read( pTheDataInTheFile, numBytesInFile);
    theFileFast.close();

    // And then, of course, you have to process this stream of bytes, but 
    // the good news is that you can use the streaming operators you used 
    // before (<< and >>) to translate the data types, BUT it's 
    // ***** WAY ***** faster because it's all in RAM


    return true;
}


bool cVAOManager::LoadPLYModelFromFile(sModelDrawInfo& drawInfo)
{
    // These structures match the PLY file format
    struct sVertex
    {
        float x, y, z;      // , c, i;
        // Now with normals...
        float nx, ny, nz;
        // Now with RGBA...
        // (Note the file is in HTML style, so 0 to 255
        //  but the shader wants 0.0 to 1.0)
        float red, green, blue, alpha;

        // Now with UV (texture) coordinates, too!
        float u, v;
    };
    struct sTriangle
    {
        unsigned int vertIndex[3];
    };

// UPDATED: meshName is already loaded into the drawInfo
//    drawInfo.meshName = fileName;
// Add the path 
//    std::string fileNameWithPath = this->m_FilePath + fileName;


    std::ifstream theFile(drawInfo.fileNameWithPath.c_str());

    //    std::ifstream theFile("GalacticaOriginal_ASCII_no_text.ply");
        //    std::ifstream theFile( fileName.c_str() );

            // Did it open?
    if (!theFile.is_open())          // theFile.is_open() == false
    {
        this->m_lastError = "Didn't load " + drawInfo.fileNameWithPath;
        return false;
    }

    // The file is good to go.

    std::string nextToken;

    // Scan until we find the word "vertex"...
    while (theFile >> nextToken)
    {
         if (nextToken == "vertex")
        {
            break;  // Exits the while loop
        }
    }

    // Read the number of vertices
    theFile >> drawInfo.numberOfVertices;


    // Scan until we find the word "face"...
    while (theFile >> nextToken)
    {
        if (nextToken == "face")
        {
            break;  // Exits the while loop
        }
    }

    // Read the number of triangles (aka "faces")
    theFile >> drawInfo.numberOfTriangles;

    // Scan until we find the word "end_header"...
    while (theFile >> nextToken)
    {
        if (nextToken == "end_header")
        {
            break;  // Exits the while loop
        }
    }

    //    sVertex myVertexArray[500];
    std::vector<sVertex> vecVertexArray;    // aka "smart array"

    // Now we can read the vertices (in a for loop)
    for (unsigned int index = 0; index < drawInfo.numberOfVertices; index++)
    {
        sVertex tempVertex;

        theFile >> tempVertex.x;
        theFile >> tempVertex.y;
        theFile >> tempVertex.z;
        //        theFile >> tempVertex.c;
        //       theFile >> tempVertex.i;

        // Now load normals:
        theFile >> tempVertex.nx;
        theFile >> tempVertex.ny;
        theFile >> tempVertex.nz;

        // Now load RGBA colours:
        theFile >> tempVertex.red;
        theFile >> tempVertex.green;
        theFile >> tempVertex.blue;
        theFile >> tempVertex.alpha;

        // Load UVs, too
        theFile >> tempVertex.u;
        theFile >> tempVertex.v;


                //vecVertexArray[index] = tempVertex;
                // "Add to the end of the vector"
                // "push" == "add", "back" == "end"
        vecVertexArray.push_back(tempVertex);       // Add thing at end of smart array

    }

    std::vector<sTriangle> vecTriagleArray;    // aka "smart array"

    // Now we can read the triangles (in a for loop)
    for (unsigned int index = 0; index < drawInfo.numberOfTriangles; index++)
    {
        sTriangle tempTri;
        int discardThis;

        // 3 3087 3639 5315 
        theFile >> discardThis; // The "3" at the start
        theFile >> tempTri.vertIndex[0];
        theFile >> tempTri.vertIndex[1];
        theFile >> tempTri.vertIndex[2];

        vecTriagleArray.push_back(tempTri);
    }



    // Allocate the amount of space we need for the GPU side arrays
    drawInfo.pVertices = new sVertex_XYZW_RGBA_N_UV_T_B[drawInfo.numberOfVertices];
//    drawInfo.pVertices = new sVertex_XYZW_RGBA[drawInfo.numberOfVertices];
//    drawInfo.pVertices = new sVertex_XYZ_RGB[drawInfo.numberOfVertices];

    // There are 3 indices per triangle...
    drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];


    // Copy the vertices from the PLY format vector
    //  to the one we'll use to draw in the GPU
    for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
    {
        // Copy position...
        drawInfo.pVertices[index].x = vecVertexArray[index].x;
        drawInfo.pVertices[index].y = vecVertexArray[index].y;
        drawInfo.pVertices[index].z = vecVertexArray[index].z;
        // If unsure about the 4th value of a vec4, make it 1.0f
        drawInfo.pVertices[index].w = 1.0f;

        // Copy colour...
        // And convert from 0-255 into 0-1
        drawInfo.pVertices[index].r = (vecVertexArray[index].red / 255.0f);
        drawInfo.pVertices[index].g = (vecVertexArray[index].green / 255.0f);
        drawInfo.pVertices[index].b = (vecVertexArray[index].blue / 255.0f);
        drawInfo.pVertices[index].a = (vecVertexArray[index].alpha / 255.0f);

        //drawInfo.pVertices[index].r = (rand() % 255) / 255.0f;
        //drawInfo.pVertices[index].g = (rand() % 255) / 255.0f;
        //drawInfo.pVertices[index].b = (rand() % 255) / 255.0f;
        //drawInfo.pVertices[index].a = (vecVertexArray[index].alpha / 255.0f);

        // And copy the other things, too:
//        float nx, ny, nz, nw;   // in vec4 vNormal;	Vertex normal X,Y,Z (W ignored)
        drawInfo.pVertices[index].nx = vecVertexArray[index].nx;
        drawInfo.pVertices[index].ny = vecVertexArray[index].ny;
        drawInfo.pVertices[index].nz = vecVertexArray[index].nz;
        drawInfo.pVertices[index].nw = 1.0f;

//        float u0, v0, u1, v1;   //in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
        drawInfo.pVertices[index].u0 = vecVertexArray[index].u;
        drawInfo.pVertices[index].v0 = vecVertexArray[index].v;
        drawInfo.pVertices[index].u1 = 0.0f;
        drawInfo.pVertices[index].v1 = 0.0f;

//        float tx, ty, tz, tw;   //in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
        // idk, tangents are made by triangles, but stored in drawInfo per vertex
        // just gotta try something else rn

        // this might work but there's a chance that u0 is 0, 
        //glm::vec3 tangent = glm::vec3(0.0f);
        //if (drawInfo.pVertices[index].u0 > 0.0001f)
        //{
        //    tangent = glm::vec3 (drawInfo.pVertices[index].x / drawInfo.pVertices[index].u0
        //    , drawInfo.pVertices[index].y / drawInfo.pVertices[index].u0
        //    , drawInfo.pVertices[index].z / drawInfo.pVertices[index].u0);
        //    tangent = glm::normalize(tangent);
        //}
        //drawInfo.pVertices[index].tx = tangent.x;
        //drawInfo.pVertices[index].ty = tangent.y;
        //drawInfo.pVertices[index].tz = tangent.z;
        drawInfo.pVertices[index].tw = 0.0f;


//        float bx, by, bz, bw;   //in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
    }

    // Copy the triangle ("index") values to the index (element) array
    unsigned int elementIndex = 0;
    for (unsigned int triIndex = 0; triIndex < drawInfo.numberOfTriangles; 
         triIndex++, elementIndex += 3)
    {
        drawInfo.pIndices[elementIndex + 0] = vecTriagleArray[triIndex].vertIndex[0];
        drawInfo.pIndices[elementIndex + 1] = vecTriagleArray[triIndex].vertIndex[1];
        drawInfo.pIndices[elementIndex + 2] = vecTriagleArray[triIndex].vertIndex[2];

        unsigned int vert1Index = vecTriagleArray[triIndex].vertIndex[0];
        unsigned int vert2Index = vecTriagleArray[triIndex].vertIndex[1];
        unsigned int vert3Index = vecTriagleArray[triIndex].vertIndex[2];

        // positions of vertex
        glm::vec3 pos1 = glm::vec3(drawInfo.pVertices[vert1Index].x
            , drawInfo.pVertices[vert1Index].y
            , drawInfo.pVertices[vert1Index].z);
            
        glm::vec3 pos2 = glm::vec3(drawInfo.pVertices[vert2Index].x
            , drawInfo.pVertices[vert2Index].y
            , drawInfo.pVertices[vert2Index].z);

        glm::vec3 pos3 = glm::vec3(drawInfo.pVertices[vert3Index].x
            , drawInfo.pVertices[vert3Index].y
            , drawInfo.pVertices[vert3Index].z);

        // texture coordinates
        glm::vec2 uv1 = glm::vec2(drawInfo.pVertices[vert1Index].u0, drawInfo.pVertices[vert1Index].v0);
        glm::vec2 uv2 = glm::vec2(drawInfo.pVertices[vert2Index].u0, drawInfo.pVertices[vert2Index].v0);
        glm::vec2 uv3 = glm::vec2(drawInfo.pVertices[vert3Index].u0, drawInfo.pVertices[vert3Index].v0);

        // edges
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        // tangent & bitangent
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent1, bitangent1;

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        if (drawInfo.pVertices[vert1Index].tw == 0.0f)
        {
            drawInfo.pVertices[vert1Index].tx = tangent1.x;
            drawInfo.pVertices[vert1Index].ty = tangent1.y;
            drawInfo.pVertices[vert1Index].tz = tangent1.z;
            drawInfo.pVertices[vert1Index].tw = 1.0f;

            drawInfo.pVertices[vert1Index].bx = bitangent1.x;
            drawInfo.pVertices[vert1Index].by = bitangent1.x;
            drawInfo.pVertices[vert1Index].bz = bitangent1.x;
            drawInfo.pVertices[vert1Index].bw = 1.0f;
        }

        if (drawInfo.pVertices[vert2Index].tw == 0.0f)
        {
            drawInfo.pVertices[vert2Index].tx = tangent1.x;
            drawInfo.pVertices[vert2Index].ty = tangent1.y;
            drawInfo.pVertices[vert2Index].tz = tangent1.z;
            drawInfo.pVertices[vert2Index].tw = 1.0f;

            drawInfo.pVertices[vert2Index].bx = bitangent1.x;
            drawInfo.pVertices[vert2Index].by = bitangent1.x;
            drawInfo.pVertices[vert2Index].bz = bitangent1.x;
            drawInfo.pVertices[vert2Index].bw = 1.0f;
        }

        if (drawInfo.pVertices[vert3Index].tw == 0.0f)
        {
            drawInfo.pVertices[vert3Index].tx = tangent1.x;
            drawInfo.pVertices[vert3Index].ty = tangent1.y;
            drawInfo.pVertices[vert3Index].tz = tangent1.z;
            drawInfo.pVertices[vert3Index].tw = 1.0f;

            drawInfo.pVertices[vert3Index].bx = bitangent1.x;
            drawInfo.pVertices[vert3Index].by = bitangent1.x;
            drawInfo.pVertices[vert3Index].bz = bitangent1.x;
            drawInfo.pVertices[vert3Index].bw = 1.0f;
        }

    }

    // https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    // https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/4.normal_mapping/normal_mapping.cpp
    



    return true;
}


// Uses a populated drawInfo structure, with the filename in the drawInfo.meshName
bool cVAOManager::LoadModelIntoVAO(sModelDrawInfo& drawInfo, unsigned int shaderProgramID)
{
    // Create a VAO (Vertex Array Object), which will 
    //	keep track of all the 'state' needed to draw 
    //	from this buffer...

    // Ask OpenGL for a new buffer ID...
    glGenVertexArrays(1, &(drawInfo.VAO_ID));
    // "Bind" this buffer:
    // - aka "make this the 'current' VAO buffer
    glBindVertexArray(drawInfo.VAO_ID);

    // Now ANY state that is related to vertex or index buffer
    //	and vertex attribute layout, is stored in the 'state' 
    //	of the VAO... 


    // NOTE: OpenGL error checks have been omitted for brevity
//	glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &(drawInfo.VertexBufferID));

    //	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    // sVert vertices[3]
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
        (GLvoid*)drawInfo.pVertices,							// pVertices,			//vertices, 
        GL_STATIC_DRAW);
    //glBufferData( GL_ARRAY_BUFFER, 
    //			  sizeof(sVertex_XYZW_RGBA) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
    //			  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
    //			  GL_STATIC_DRAW );
    //glBufferData( GL_ARRAY_BUFFER, 
    //			  sizeof(sVertex_XYZ_RGB) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
    //			  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
    //			  GL_STATIC_DRAW );


    // Copy the index buffer into the video card, too
    // Create an index buffer.
    glGenBuffers(1, &(drawInfo.IndexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    // ****************************************************************
    // Set the vertex attributes.


    //struct sVertex_XYZW_RGBA
    //{
    //    float x, y, z, w;   // Same as vec4 vPosition
    //    float r, g, b, a;   // Same as vec4 vColour
    //};

    // Set the vertex attributes for this shader
    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	    // program
    glEnableVertexAttribArray(vpos_location);	    // vPosition
    glVertexAttribPointer(vpos_location, 4,		// vPosition
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,      
        (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, x));

    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	// program;
    glEnableVertexAttribArray(vcol_location);	    // vColour
    glVertexAttribPointer(vcol_location, 4,		// vColour
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,   
        (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, r));

    // And all these, too:
    //in vec4 vNormal;				// Vertex normal X,Y,Z (W ignored)
    GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
    glEnableVertexAttribArray(vNormal_location);	    // vNormal
    glVertexAttribPointer(vNormal_location, 4,		    // vNormal
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
        (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, nx));


    //in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
    GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");	// program;
    glEnableVertexAttribArray(vUVx2_location);	        // vUVx2
    glVertexAttribPointer(vUVx2_location, 4,		    // vUVx2
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
        (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, u0));


    //in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
    GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");	// program;
    glEnableVertexAttribArray(vTangent_location);	    // vTangent
    glVertexAttribPointer(vTangent_location, 4,		    // vTangent
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
        (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, tx));


    //in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
    GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");	// program;
    glEnableVertexAttribArray(vBiNormal_location);	        // vBiNormal
    glVertexAttribPointer(vBiNormal_location, 4,		    // vBiNormal
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
        (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bx));


    // ****************************************************************

    // Now that all the parts are set up, set the VAO to zero
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    // And the newer ones:
    glDisableVertexAttribArray(vNormal_location);	    // vNormal
    glDisableVertexAttribArray(vUVx2_location);	        // vUVx2
    glDisableVertexAttribArray(vTangent_location);	    // vTangent
    glDisableVertexAttribArray(vBiNormal_location);	        // vBiNormal


    // Store the draw information into the map
    this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;


    return true;
}


std::string cVAOManager::getLastError(bool bAndClear /*=true*/)
{
    std::string error = this->m_lastError;
    if (bAndClear)
    {
        this->m_lastError = "";
    }
    return error;
}

void cVAOManager::setFilePath(std::string filePath)
{
    this->m_FilePath = filePath;
}

std::string cVAOManager::getFilePath(void)
{
    return this->m_FilePath;
}

// Generates texture coordinates based on a shperical projection from the 
// origin (0,0,0) location of the model. 
// NOTE: Will overwrite any existing texture coordinates 
// Inspired by: http://www.mvps.org/directx/articles/spheremap.htm
// 
//	basedOnNormals = true --> uses normals for calculation, so good for reflections
//	basedOnNormals = false --> uses model coordinates, so more 'shrink wrapped'
//  fast -> approximate or not (not uses sin() )
//  scale -> normally 1.0, but can be the limits of the texture coord
//
// The 'bias' is what coordinates are used for the generation. 
// For example, if POSITIVE_X is for u and POSITIVE_Y is for v, then:
//	u coords: based on -x to +x
//  v coords: based on -y to +y
//
//  enum enumTEXCOORDBIAS {
//    POSITIVE_X, POSITIVE_Y, POSITIVE_Z
//  };
void cVAOManager::GenerateSphericalTextureCoords(
    sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast)
{
    this->GenerateSphericalTextureCoords(
        cVAOManager::POSITIVE_X, cVAOManager::POSITIVE_Y, drawInfo,
        basedOnNormals, scale, fast);

    return;
}

void cVAOManager::GenerateSphericalTextureCoords(
    enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias,
    sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast)
{
    // determine the 'remaining' coordinate...
    bool xUsed = false;
    bool yUsed = false;
    bool zUsed = false;
    if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
    if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
    if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;

    for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
    {
        glm::vec3 xyz;
        if (basedOnNormals)
        {
            if (uBias == POSITIVE_X)		xyz.x = drawInfo.pVertices[index].nx;
            else if (uBias == POSITIVE_Y)	xyz.x = drawInfo.pVertices[index].ny;
            else if (uBias == POSITIVE_Z)	xyz.x = drawInfo.pVertices[index].nz;

            if (vBias == POSITIVE_X)		xyz.y = drawInfo.pVertices[index].nx;
            else if (vBias == POSITIVE_Y)	xyz.y = drawInfo.pVertices[index].ny;
            else if (vBias == POSITIVE_Z)	xyz.y = drawInfo.pVertices[index].nz;

            // Fill in the remaining coordinate...
            if (!xUsed)	xyz.z = drawInfo.pVertices[index].nx;
            if (!yUsed)	xyz.z = drawInfo.pVertices[index].ny;
            if (!zUsed)	xyz.z = drawInfo.pVertices[index].nz;
        }
        else
        {
            if (uBias == POSITIVE_X)		xyz.x = drawInfo.pVertices[index].x;
            else if (uBias == POSITIVE_Y)	xyz.x = drawInfo.pVertices[index].y;
            else if (uBias == POSITIVE_Z)	xyz.x = drawInfo.pVertices[index].z;

            if (vBias == POSITIVE_X)		xyz.y = drawInfo.pVertices[index].x;
            else if (vBias == POSITIVE_Y)	xyz.y = drawInfo.pVertices[index].y;
            else if (vBias == POSITIVE_Z)	xyz.y = drawInfo.pVertices[index].z;

            // Fill in the remaining coordinate...
            if (!xUsed)	xyz.z = drawInfo.pVertices[index].x;
            if (!yUsed)	xyz.z = drawInfo.pVertices[index].y;
            if (!zUsed)	xyz.z = drawInfo.pVertices[index].z;
        }

        xyz = glm::normalize(xyz);

        if (fast)
        {
            drawInfo.pVertices[index].u0 = ((xyz.x / 2.0f) + 0.5f) * scale;
            drawInfo.pVertices[index].v0 = ((xyz.y / 2.0f) + 0.5f) * scale;
        }
        else
        {
            drawInfo.pVertices[index].u0 = ((asin(xyz.x) / glm::pi<float>() ) + 0.5f) * scale;
            drawInfo.pVertices[index].v0 = ((asin(xyz.y) / glm::pi<float>() ) + 0.5f) * scale;
        }
        drawInfo.pVertices[index].u1 = drawInfo.pVertices[index].u0;
        drawInfo.pVertices[index].v1 = drawInfo.pVertices[index].v0;
    }
    return;
}



// Added November 24, 2021: To save the planet model so we can reload it
// The ".ply" is added automatically.
bool cVAOManager::SaveAsPlyFile(sModelDrawInfo& drawInfo, std::string fileName, bool bOverwrite,
                   bool bSaveRGB, bool bSaveNormals, bool bSaveUVs,
                   std::string& error)
{
    // See if the drawInfo has any values in it
    if ( (drawInfo.numberOfIndices == 0) || (drawInfo.numberOfVertices == 0) )
    {
        error = "Mesh has no data in it. Nothing to do.";
        return false;
    }

    // See if the file is already there (and you DON'T want to overwrite)
    if ( ! bOverwrite )
    {
        std::ifstream testFile(fileName.c_str());
        if ( testFile.is_open() )
        {
            error = "File already exists and bOverwrite isn't set to true.";
            testFile.close();
            return false;
        }
    }

    // At this point, the file either isn't there, or you're overwriting
    std::ofstream plyFile(fileName.c_str());
    if ( ! plyFile.is_open() )
    {
        error = "Couldn't open the file for writing.";
        plyFile.close();
        return false;
    }

    // Write the header:
    plyFile << "ply" << std::endl;
    plyFile << "format ascii 1.0" << std::endl;
    plyFile << "comment cVAOManager_for_Fanshawe_GDP2021 generated" << std::endl;
    plyFile << "element vertex " << drawInfo.numberOfVertices << std::endl;
    plyFile << "property float x" << std::endl;
    plyFile << "property float y" << std::endl;
    plyFile << "property float z" << std::endl;
    plyFile << "property float nx" << std::endl;
    plyFile << "property float ny" << std::endl;
    plyFile << "property float nz" << std::endl;
    plyFile << "property uchar red" << std::endl;
    plyFile << "property uchar green" << std::endl;
    plyFile << "property uchar blue" << std::endl;
    plyFile << "property uchar alpha" << std::endl;
    plyFile << "property float texture_u" << std::endl;
    plyFile << "property float texture_v" << std::endl;
    plyFile << "element face " << drawInfo.numberOfTriangles << std::endl;
    plyFile << "property list uchar int vertex_indices" << std::endl;
    plyFile << "end_header" << std::endl;

    // Write out the vertices
    for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
    {
        sVertex_XYZW_RGBA_N_UV_T_B& curVertex = drawInfo.pVertices[index];

        plyFile 
            << curVertex.x << " " 
            << curVertex.y << " " 
            << curVertex.z << " " 
            << curVertex.nx << " "
            << curVertex.ny << " "
            << curVertex.nz << " "
            << (unsigned int)(curVertex.r * 255) << " "
            << (unsigned int)(curVertex.g * 255) << " "
            << (unsigned int)(curVertex.b * 255) << " "
            << (unsigned int)(curVertex.a * 255) << " "
            << curVertex.u0 << " "
            << curVertex.v0 << std::endl;
    }//for ( unsigned int index...

    // Write out the indices
    for ( unsigned int index = 0; index < drawInfo.numberOfIndices; index += 3 )
    {
        plyFile << "3 " 
            << drawInfo.pIndices[index] << " "
            << drawInfo.pIndices[index + 1] << " "
            << drawInfo.pIndices[index + 2] << std::endl;
    }//for ( unsigned int index...


    plyFile.close();
    error = "OK";

    return true;
}


// Added November 24, 2021: To handle the cylindrical projection UV mapping for applying planet textures to spheres
// This projects on a cylinder that has an axis along the y-axis.
void cVAOManager::GenerateCylindricalTextureCoords_FromNormals(sModelDrawInfo& drawInfo)
{

    // Here's a couple links to explain this:
    // * https://www.cosc.brocku.ca/Offerings/3P98/course/lectures/texture/
    // * https://gamedev.stackexchange.com/questions/114412/how-to-get-uv-coordinates-for-sphere-cylindrical-projection

    for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
    {
        sVertex_XYZW_RGBA_N_UV_T_B& curVertex = drawInfo.pVertices[index];
        glm::vec3 vertNormal = glm::normalize(glm::vec3(curVertex.nx, curVertex.ny, curVertex.nz));

        // Compare this vertex normal to the +ve Y axis
        float dotFromPosY = glm::dot( glm::vec3( 0.0f, 1.0f, 0.0f), vertNormal );
        // This will be from 1.0 (aligned to the +ve Y axis) to -1.0 (aligned to the -ve Y axis)
        // Scale this value to the V text axis range from 0 to 1
        curVertex.v0 = (dotFromPosY + 1.0f)/2.0f;

        curVertex.u0 = atan2(vertNormal.x, vertNormal.z) / (2.0f * glm::pi<float>() ) + 0.5f;


    }//for ( unsigned int index...

    return;
}

