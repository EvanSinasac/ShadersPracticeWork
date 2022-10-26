#include "cFBO.h"


// Calls shutdown(), then init()
bool cFBO::reset(int width, int height, std::string &error)
{
	if ( ! this->shutdown() )
	{
		error = "Could not shutdown";
		return false;
	}

	return this->init( width, height, error );
}

bool cFBO::shutdown(void)
{
	glDeleteTextures( 1, &(this->colourTexture_0_ID) );
	glDeleteTextures( 1, &(this->depthTexture_ID) );

	glDeleteTextures( 1, &(this->vertexMaterialColour_1_ID) );
	glDeleteTextures( 1, &(this->vertexNormal_2_ID) );
	glDeleteTextures( 1, &(this->vertexWorldPos_3_ID) );
	glDeleteTextures( 1, &(this->vertexSpecular_4_ID) );

	glDeleteFramebuffers( 1, &(this->ID) );

	return true;
}


bool cFBO::init( int width, int height, std::string &error )
{
	this->width = width;
	this->height = height;

//	glCreateFramebuffers(1, &( this->ID ) );	// GL 4.5		//g_FBO
	glGenFramebuffers( 1, &( this->ID ) );		// GL 3.0
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

//************************************************************
	// Create the colour buffer (texture)
	glGenTextures(1, &(this->colourTexture_0_ID ) );		//g_FBO_colourTexture
	glBindTexture(GL_TEXTURE_2D, this->colourTexture_0_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8,		// 8 bits per colour
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,	// 32 bits per colour (a float per colour)
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//***************************************************************
	
	// Here's the additional layers of the G-buffer
	glGenTextures(1, &(this->vertexMaterialColour_1_ID ) );		
	glBindTexture(GL_TEXTURE_2D, this->vertexMaterialColour_1_ID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8,		// 8 bits per colour
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glGenTextures(1, &(this->vertexNormal_2_ID ) );			
	glBindTexture(GL_TEXTURE_2D, this->vertexNormal_2_ID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 1 float per 'color' (xyz)
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &(this->vertexWorldPos_3_ID ) );		
	glBindTexture(GL_TEXTURE_2D, this->vertexWorldPos_3_ID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 1 float per xyz coordinate
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &(this->vertexSpecular_4_ID ) );		
	glBindTexture(GL_TEXTURE_2D, this->vertexSpecular_4_ID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8,		// 8 bits per colour
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// *********************************************

	// Create the depth buffer (texture)
	glGenTextures(1, &( this->depthTexture_ID ));			//g_FBO_depthTexture
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);

	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, ]

	// Note that, unless you specifically ask for it, the stencil buffer
	// is NOT present... i.e. GL_DEPTH_COMPONENT32F DOESN'T have stencil

	// These are:
	// - GL_DEPTH32F_STENCIL8, which is 32 bit float depth + 8 bit stencil
	// - GL_DEPTH24_STENCIL8,  which is 24 bit float depth + 8 bit stencil (more common?)
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8,	//GL_DEPTH32F_STENCIL8,
				   this->width,		//g_FBO_SizeInPixes
				   this->height);
//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT );
//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_COMPONENT );
//	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0, GL_EXT_packe

//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, this->depthTexture_ID, 0);

// ***************************************************************

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT0,			// Colour goes to #0
						 this->colourTexture_0_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT1,			// Vertex Colour goes to #01
						 this->vertexMaterialColour_1_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT2,			// Normal goes to #02
						 this->vertexNormal_2_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT3,			// Vertex World Position goes to #03
						 this->vertexWorldPos_3_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT4,			// Vertex Specular colour + Power Position goes to #03
						 this->vertexSpecular_4_ID, 0);

//	glFramebufferTexture(GL_FRAMEBUFFER,
//						 GL_DEPTH_ATTACHMENT,
//						 this->depthTexture_ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_DEPTH_STENCIL_ATTACHMENT,
						 this->depthTexture_ID, 0);

	static const GLenum draw_bufers[] = 
	{ 
		GL_COLOR_ATTACHMENT0,		// Defaults OG colour buffer output
		GL_COLOR_ATTACHMENT1,		// vertexMaterialColour_1_ID
		GL_COLOR_ATTACHMENT2,		// vertexNormal_2_ID
		GL_COLOR_ATTACHMENT3,		// vertexWorldPos_3_ID
		GL_COLOR_ATTACHMENT4		// vertexSpecular_4_ID
	};
//	glDrawBuffers(1, draw_bufers);		// There is only 1 output
	glDrawBuffers(5, draw_bufers);		// There are 5 outputs now

	// ***************************************************************




	// ADD ONE MORE THING...
	bool bFrameBufferIsGoodToGo = true; 

	switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )
	{
	case GL_FRAMEBUFFER_COMPLETE:
		bFrameBufferIsGoodToGo = true; 
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		bFrameBufferIsGoodToGo = false; 
		break;
//	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	case GL_FRAMEBUFFER_UNSUPPORTED:
	default:
		bFrameBufferIsGoodToGo = false; 
		break;
	}//switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFrameBufferIsGoodToGo;
}

void cFBO::clearColourBuffer(int bufferindex)
{
	glViewport(0, 0, this->width, this->height);
	GLfloat	zero = 0.0f;
	glClearBufferfv(GL_COLOR, bufferindex, &zero);

	return;
}


void cFBO::clearBuffers(bool bClearColour, bool bClearDepth)
{
	glViewport(0, 0, this->width, this->height);

//	POSSIBLE ERROR:	GLfloat	zero = 0.0f;
//
//	If buffer is GL_COLOR, a particular draw buffer GL_DRAW_BUFFERi is specified 
//	by passing i as drawbuffer, and value points to a four-element vector specifying 
//	the R, G, B and A color to clear that draw buffer to. If the value of 
//	GL_DRAW_BUFFERi is GL_NONE, the command has no effect. Otherwise, the value 
//	of GL_DRAW_BUFFERi identifies one or more color buffers, each of which is 
//	cleared to the same value. Clamping and type conversion for fixed-point 
//	color buffers are performed in the same fashion as for glClearColor. 
//	The *fv, *iv and *uiv forms of these commands should be used to clear 
//	fixed- and floating-point, signed integer, and unsigned integer color 
//	buffers respectively.

//	Should be:
	GLfloat	zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	if ( bClearColour )
	{
		glClearBufferfv(GL_COLOR, 0, zero);		// Colour
	}

//	If buffer is GL_DEPTH, drawbuffer must be zero, and value points to a single 
//  value to clear the depth buffer to. Clamping and type conversion for fixed-point 
//  depth buffers are performed in the same fashion as for glClearDepth. Only the 
//  *fv forms of these commands should be used to clear depth buffers; other forms 
//  do not accept a buffer of GL_DEPTH.
	GLfloat one = 1.0f;
	if ( bClearDepth )
	{
		glClearBufferfv(GL_DEPTH, 0, &one);		// Depth is normalized 0.0 to 1.0f
	}

	// And clear all the other buffers, too
//	GL_COLOR_ATTACHMENT0,		// Defaults OG colour buffer output
//		This is happening at line 201 already
//	GL_COLOR_ATTACHMENT1,		// vertexMaterialColour_1_ID
//	GL_COLOR_ATTACHMENT2,		// vertexNormal_2_ID
//	GL_COLOR_ATTACHMENT3,		// vertexWorldPos_3_ID
//	GL_COLOR_ATTACHMENT4		// vertexSpecular_4_ID

	glClearBufferfv(GL_COLOR, 1, zero);		// vertexMaterialColour_1_ID
	glClearBufferfv(GL_COLOR, 2, zero);		// vertexNormal_2_ID
	glClearBufferfv(GL_COLOR, 3, zero);		// vertexWorldPos_3_ID
	glClearBufferfv(GL_COLOR, 4, zero);		// vertexSpecular_4_ID

	// If buffer is GL_STENCIL, drawbuffer must be zero, and value points to a 
	//  single value to clear the stencil buffer to. Masking is performed in the 
	//  same fashion as for glClearStencil. Only the *iv forms of these commands 
	//  should be used to clear stencil buffers; be used to clear stencil buffers; 
	//  other forms do not accept a buffer of GL_STENCIL.
	
	// 
	glStencilMask(0xFF);

	{	// Clear stencil
		//GLint intZero = 0;
		//glClearBufferiv(GL_STENCIL, 0, &intZero );
		glClearBufferfi( GL_DEPTH_STENCIL, 
						 0,		// Must be zero
						 1.0f,	// Clear value for depth
						 0 );	// Clear value for stencil
	}

	return;
}


int cFBO::getMaxColourAttachments(void)
{
	//  void glGetIntegerv(GLenum pname,
	// 				       GLint * data);
	
	int maxColourAttach = 0;
	glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &maxColourAttach );

	return maxColourAttach;
}

int cFBO::getMaxDrawBuffers(void)
{
	int maxDrawBuffers = 0;
	glGetIntegerv( GL_MAX_DRAW_BUFFERS, &maxDrawBuffers );
	
	return maxDrawBuffers;
}
