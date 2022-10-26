#ifndef _FBO_HG_
#define _FBO_HG_

#include <string>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "../GLCommon.h"

class cFBO
{
public:
	cFBO() : 
		ID(0), 
		colourTexture_0_ID(0),
		depthTexture_ID(0), 
		// For deferred, we are using these
		vertexMaterialColour_1_ID(0),
		vertexNormal_2_ID(0),
		vertexWorldPos_3_ID(0),
		vertexSpecular_4_ID(0),

		width(-1), height(-1) {};

	GLuint ID;						// = 0;
	GLuint colourTexture_0_ID;		// = 0;

	// Our fragment shader is using this lighting function:
	// vec4 calcualteLightContrib( 
	//		vec3 vertexMaterialColour, 
	//		vec3 vertexNormal, 
	//		vec3 vertexWorldPos, 
	//		vec4 vertexSpecular );

	GLuint vertexMaterialColour_1_ID;		// = 1;
	GLuint vertexNormal_2_ID;		// = 2;
	GLuint vertexWorldPos_3_ID;		// = 3;
	GLuint vertexSpecular_4_ID;		// = 4;


	GLuint depthTexture_ID;		// = 0;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);
	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);
	
	void clearBuffers(bool bClearColour = true, bool bClearDepth = true);

	void clearColourBuffer(int bufferindex);
	void clearAllColourBuffers(void);
	void clearDepthBuffer(void);
	void clearStencilBuffer( int clearColour, int mask = 0xFF );

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif
