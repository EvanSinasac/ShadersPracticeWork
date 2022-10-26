#include "cMesh.h"
//Stuff done in class with Michael Feeney

cMesh::cMesh()
{
	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	//this->scale = 1.0f;
	this->scale = glm::vec3(1.0f);

	this->bIsWireframe = false;
	this->bDisableDepthBufferCheck = false;

	this->bIsImposter = false;

	this->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// White
	this->bUseObjectDebugColour = false;
	this->bDontLight = false;

	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	this->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// The "colour" of the object
	this->bUseWholeObjectDiffuseColour = false;
	//
	// Specular HIGHLIGHT colour (usually the same as the light, or white)
	this->wholeObjectSpecularRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	// Specular highlight POWER (or shininess). Starts at 1.0 and goes to ? (like 100, 1000, 10000 is OK)
	this->wholeObjectShininess_SpecPower = 1.0f;	// 1.0 to ??

	// Default to 1.0f (all solid)
	this->alphaTransparency = 1.0f;

	// Identity in case we don't need them
	//this->matLocalOriginTransform = glm::mat4(1.0f);
	//this->matTranslateBack = glm::mat4(1.0f);

	// Clear all the textures
	for (unsigned int index = 0; index != cMesh::MAX_TEXTURES; index++)
	{
		this->textureNames[index] = "";
		this->textureRatios[index] = 0.0f;
	}
	this->textureNames[0] = "BrightColouredUVMap.bmp";
	this->textureRatios[0] = 1.0f;

	// Set these to an identity matrix (in case we don't need them)
	this->matModelBaseOffset = glm::mat4(1.0f);

	// Assign a unique ID
	this->m_UniqueID = cMesh::m_NextID;
	cMesh::m_NextID++;

	//this->friendlyID = 1;

	this->textureOperator = 0;

	this->discardTexture = "";
	this->bHasDiscardTexture;

	this->bIsVisible = true;

	this->bUseStencil = false;
}

void cMesh::setUniformScale(float uniformScale)
{
	this->scale.x = this->scale.y = this->scale.z = uniformScale;
	return;
}

float cMesh::getAverageScale(void)
{
	return (this->scale.x + this->scale.y + this->scale.z) / 3.0f;
}



void cMesh::clearTextureRatiosToZero(void)
{
	for (unsigned int index = 0; index < cMesh::MAX_TEXTURES; index++)
	{
		this->textureRatios[index] = 0.0f;
	}
}


unsigned int cMesh::getUniqueID(void)
{
	return this->m_UniqueID;
}

//static 
unsigned int cMesh::m_NextID = cMesh::INITIAL_UNIQUE_ID;

