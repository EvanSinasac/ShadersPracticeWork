#ifndef _globalThings_HG_
#define _globalThings_HG_
//Stuff done in class with Michael Feeney

#include "GLCommon.h"

//#include "cFlyCamera.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "TextureMapping/cBasicTextureManager.h"
#include "cMesh.h"
#include <vector>
#include "cFBO/cFBO.h"

#include <physics/cParticleWorld.h>

#include <FMOD/fmod.hpp>

#include "ResourceManager.h"
#include "Graph.h"
#include "cFSMEntity.h"
#include "iEntity.h"
//#include "cTorchObject.h"
//#include "cPlayerEntity.h"
// 

extern bool g_updateFBOResolution;
extern int g_2DEffectOp;
extern bool g_UseBloom;

extern unsigned int g_Mode;
extern bool g_ObservationMode;
extern bool g_FirstPersonMode;
extern bool g_OverheadMode;
extern bool g_DrawAllHighResModels;
extern glm::vec3 lastCamPosition;
extern glm::vec3 lastCamLookAt;

extern bool g_bStencilsOn;

// These are all things that are "project global", 
//	so need to be visible to many files and parts

// Creates global things, etc. (to control the order)
bool g_StartUp(GLFWwindow* pWindow);

// Deletes global things, etc. 
bool g_ShutDown(GLFWwindow* pWindow);
bool g_FMODShutDown();

extern cShaderManager::cShaderProgram* pShaderProc;

//extern std::map<unsigned int, std::vector<glm::vec3>> g_debugNormals;
extern bool g_turnDebugNormalsOn;

extern ResourceManager gResourceManager;
extern Graph* g_Graph;
extern Node* g_exitNode;

extern std::vector<cFSMEntity*> vec_pFSMEntities;
extern float entitySpeedModifier;

//extern cPlayerEntity* g_pPlayer;
extern iEntity* g_pPlayer;

//extern std::vector<cTorchObject*> vec_pTorches;
extern std::vector<iEntity*> vec_pTorches;

extern std::vector<iEntity*> vec_pAllEntities;
extern std::vector<iEntity*> vec_pEnemies;      // for the FBO stuff I want
// ************************************************
//     ___                         
//    / __|__ _ _ __  ___ _ _ __ _ 
//   | (__/ _` | '  \/ -_) '_/ _` |
//    \___\__,_|_|_|_\___|_| \__,_|
//                                 
//extern cFlyCamera* g_pFlyCamera; // = NULL;

// This the keyboard based eye location
// Like if you AREN'T using the fly camera
//extern glm::vec3 g_cameraEye;	//	= glm::vec3(0.0f, 0.0f, +4.0f);
//extern glm::vec3 g_cameraAt;	//	= glm::vec3(0.0f, 0.0f, 0.0f);
// ************************************************
extern glm::vec3 cameraEye;// = glm::vec3(0.0f, 0.0f, 3.0f);     //default camera start position, just in case something goes wrong with loading the worldFile
extern glm::vec3 cameraTarget;// = glm::vec3(0.0f, 0.0f, 1.0f);   //default camera Target position
extern glm::vec3 upVector;// = glm::vec3(0.0f, 1.0f, 0.0f);       //default upVector


extern cVAOManager* g_pVAOManager;
extern cShaderManager* g_pShaderManager;
extern cBasicTextureManager* g_pTextureManager;

extern cLightManager* g_pTheLights;
extern unsigned int g_currentLightIndex;
// This got moved into the light manager class
//extern cLightHelper* g_pTheLightHelper;

extern cMesh* g_pDebugSphere;	// = NULL;
extern bool g_bShowDebugShere;	// = true;

extern bool g_bShowDebugObjects;	// = true;


extern unsigned int g_selectedObject;	// = 0;
extern unsigned int g_selectedLight;	// = 0;

// Used for the texture height displacement example
// x & y are the U & V axis displacemnt, and z is the rotation 
// Note that when we rotate, the 0,0 location is at the corner of the 
//  texture, but we can offset this to 0.5,0.5 to rotate around the "centre"
extern glm::vec3 g_heightMapUVOffsetRotation;


// This will be printed in the title bar
extern std::string g_TitleText;	// = "";



// List of objects to draw
extern std::vector< cMesh* > g_vec_pMeshes;
extern std::vector< cMesh* > g_vec_pFootprintMeshes;
extern std::vector< cMesh* > g_vec_pFloorsForAlpha;
extern std::vector<cMesh*> g_vec_pPointLights;
extern bool drawLightBalls;

extern std::vector<cMesh*> g_vec_pNodes;

// Full screen quad mesh for 2nd rendering pass
extern cMesh* g_pFullScreenQuad;    //NULL;
//extern cMesh* g_pStationAngle1Quad;
//extern cMesh* g_pStationAngle2Quad;
//extern cMesh* g_pStationAngle3Quad;
//extern cMesh* g_pStationAngle4Quad;
// The FBO that the full screen quad is going to use
extern cFBO* g_pFBO;        //NULL;
//extern cFBO* g_pFBOAngle1;
//extern cFBO* g_pFBOAngle2;
//extern cFBO* g_pFBOAngle3;
//extern cFBO* g_pFBOAngle4;

//    ____  _               _          
//   |  _ \| |__  _   _ ___(_) ___ ___ 
//   | |_) | '_ \| | | / __| |/ __/ __|
//   |  __/| | | | |_| \__ \ | (__\__ \
//   |_|   |_| |_|\__, |___/_|\___|___/
//                |___/                

extern nPhysics::cParticleWorld* g_pWorld;
extern std::vector<nPhysics::cParticle*> g_vec_pParticles;


//       _             _ _       
//      / \  _   _  __| (_) ___  
//     / _ \| | | |/ _` | |/ _ \ 
//    / ___ \ |_| | (_| | | (_) |
//   /_/   \_\__,_|\__,_|_|\___/ 
//                               
extern FMOD::System* g_pFMODSystem;
extern FMOD_RESULT _result;
extern std::vector<std::string> songNames;
extern std::vector<FMOD::Sound*> g_vecSounds;
extern std::vector<FMOD::Channel*> g_vecChannels;

// Note: This returns a COPY of the object as a stack variable (not a pointer)
bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh& theObjectIFound);
// This returns a pointer to the object
bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh*& theObjectIFound);

// If not found, return NULL (0)
cMesh* g_FindObjectByUniqueID(unsigned int uniqueID_toFind);
cMesh* g_findObjectByFriendlyName(std::string NametoFind);



// ************************************************
//    _  __         _                      _                 _ _  __ _            
//   | |/ /___ _  _| |__  ___  __ _ _ _ __| |  _ __  ___  __| (_)/ _(_)___ _ _ ___
//   | ' </ -_) || | '_ \/ _ \/ _` | '_/ _` | | '  \/ _ \/ _` | |  _| / -_) '_(_-<
//   |_|\_\___|\_, |_.__/\___/\__,_|_| \__,_| |_|_|_\___/\__,_|_|_| |_\___|_| /__/
//             |__/                                                               
//
// Note: This is here, rather than in a separate header file since
//  it's pretty "global" and we don't have to include the GFLW stuff again.
//
class cGFLWKeyboardModifiers
{
public:
    static bool isLeftShiftKeyDown(GLFWwindow* pWindow);
    static bool isRightShiftKeyDown(GLFWwindow* pWindow);
    static bool isShiftKeyDown(GLFWwindow* pWindow);        // Either down

    static bool isLeftControlKeyDown(GLFWwindow* pWindow);
    static bool isRightControlKeyDown(GLFWwindow* pWindow);
    static bool isControlKeyDown(GLFWwindow* pWindow);      // Either down

    static bool isLeftAltKeyDown(GLFWwindow* pWindow);
    static bool isRightAltKeyDown(GLFWwindow* pWindow);
    static bool isAltKeyDown(GLFWwindow* pWindow);          // Either down

    // Returns true if this pattern matches
    // example: (true, false, false) returns true if ONLY shift is down
    static bool isModifierDown(GLFWwindow* pWindow, bool bShift, bool bControl, bool bAlt);

    static bool areAllModsUp(GLFWwindow* pWindow);
};
// ************************************************

void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime);
void handleAsyncMouse(GLFWwindow* pWindow, double deltaTime);


// ************************************************
//     ___ _    _____      __          _ _ _             _       
//    / __| |  | __\ \    / /  __ __ _| | | |__  __ _ __| |__ ___
//   | (_ | |__| _| \ \/\/ /  / _/ _` | | | '_ \/ _` / _| / /(_-<
//    \___|____|_|   \_/\_/   \__\__,_|_|_|_.__/\__,_\__|_\_\/__/
//                                                               
// In the documentation, these are static, but in C++ we don't need to do this. 
void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFW_error_callback(int error, const char* description);
void GLFW_cursor_enter_callback(GLFWwindow* window, int entered);
void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void GLFW_window_size_callback(GLFWwindow* window, int width, int height);


// ************************************************


// This silly define is so that you 100% know this is NOT some pre-defined define. 
// This enables the use of the Windows context menu.
// See the cWinContextMenu class and the Win32 API code for details.
// (Obviously, if you aren't running this on Windows, you don't want this, right?)
#define YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

    // Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#endif  // YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

// Note that this is declared no matter what:
void ShowWindowsContextMenu(GLFWwindow* window, int button, int action, int mods);


// SUPER quick-n-dirty template example
template <class T>
T addTwoNumbers(T a, T b)
{
    return a + b;
}

// Here's a more useful templated one, based on this one:
//https://stackoverflow.com/questions/686353/random-float-number-generation
template <class T>
T gGetRandBetween(T LO, T HI)
{
    float fLO = static_cast<float>(LO);
    float fHI = static_cast<float>(HI);
    float r3 = fLO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (fHI - fLO)));
    return r3;
}


#endif // _globalThings_HG_