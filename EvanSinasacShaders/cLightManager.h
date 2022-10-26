#pragma once
//Stuff done in class with Michael Feeney

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <string>
#include "cLightHelper.h"


// This is based on the sLight struct inside the shader
struct sLight
{
	// You could put a constructor here, if you'd like
	sLight();

	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;	// rgb = highlight colour, w = power
	glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	glm::vec4 direction;	// Spot, directional lights
	glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
					// 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	glm::vec4 param2;	// x = 0 for off, 1 for on

	// Here's the uniform locations of the light values in the shader
	int position_uniform_location;
	int diffuse_uniform_location;
	int specular_uniform_location;
	int atten_uniform_location;
	int direction_uniform_location;
	int param1_uniform_location;
	int param2_uniform_location;
};

class cLightManager
{
public:
	cLightManager();

	// This got moved into here (seemed more clear)
	static cLightHelper lightHelper;

	// You could do this sort of thing, too:
	void SetLightColour(unsigned int lightNumber, float red, float green, float blue);
	// and so on...

	void TurnOnLight(unsigned int lightNumber);
	void TurnOffLight(unsigned int lightNumber);

	void SetPosition(glm::vec3 newPosition);
	void MoveLightRelative(glm::vec3 deltaPosition);

	// Here's an array of lights that match
	//	the ones inside the shader
	static const unsigned int NUMBER_OF_LIGHTS = 100;
	sLight theLights[NUMBER_OF_LIGHTS];

	// This sets up the initial uniform locations from the shader
	void SetUpUniformLocations(unsigned int shaderProgram);

	// Copies the values from the array into the shader
	void CopyLightInfoToShader(void);

	void SaveLightInformationToFile(std::string fileName);
	void LoadLightInformationFromFile(std::string fileName);


};
