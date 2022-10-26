#include "globalThings.h"
//Stuff done in class with Michael Feeney

bool g_StartUp(GLFWwindow* pWindow)
{

	//::g_pFlyCamera = new cFlyCamera();

	// Create the shader manager
	::g_pShaderManager = new cShaderManager();

	::g_pTheLights = new cLightManager();

	::g_pVAOManager = new cVAOManager();

	::g_pTextureManager = new cBasicTextureManager();

	::g_pWorld = new nPhysics::cParticleWorld(1000);

	return true;
}
