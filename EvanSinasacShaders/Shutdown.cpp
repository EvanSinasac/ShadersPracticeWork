#include "globalThings.h"
//Stuff done in class with Michael Feeney

// Deletes global things, etc. 
bool g_ShutDown(GLFWwindow* pWindow)
{

	delete ::g_pShaderManager;

	delete ::g_pVAOManager;

	delete ::g_pTheLights;

	//delete ::g_pFlyCamera;

	delete ::g_pTextureManager;

	for (unsigned int index = 0; index != ::g_vec_pParticles.size(); index)
	{
		::g_pWorld->RemoveParticle(::g_vec_pParticles[index]);
		::g_vec_pParticles.erase(::g_vec_pParticles.begin());
	}
	delete ::g_pWorld;

	return true;
}

bool g_FMODShutDown() 
{
	std::vector<FMOD::Sound*>::iterator itSounds = g_vecSounds.begin();
	for (itSounds; itSounds != g_vecSounds.end(); itSounds++)
	{
		_result = (*itSounds)->release();
		if (_result != FMOD_OK)
		{
			fprintf(stderr, "Unable to release sound!\n");
		}
	}

	_result = g_pFMODSystem->close();
	if (_result != FMOD_OK)
		fprintf(stderr, "Unable to close system!\n");

	_result = g_pFMODSystem->release();
	if (_result != FMOD_OK)
		fprintf(stderr, "Unable to release system!\n");

	return true;
}