//Stuff done in class with Michael Feeney
#include "GLCommon.h" 
#include "globalThings.h"

#include <sstream>
#include <iostream>

// Handle async IO (keyboard, joystick, mouse, etc.)


// This is so the "fly camera" won't pay attention to the mouse if it's 
// not directly over the window. 
bool g_MouseIsInsideWindow = false;



void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime)
{
   // float cameraMoveSpeed = 50.0f * deltaTime;   //20.0f for space scene
    //float cameraMoveSpeed = 5.0f * deltaTime;   //20.0f for space scene
    float cameraMoveSpeed = 20.0f * deltaTime;   //20.0f for space scene
    //float lightMoveSpeed = 5.0f * deltaTime;


    if (cGFLWKeyboardModifiers::areAllModsUp(pWindow))
    {
        if (::g_ObservationMode)
        {
            // Use "fly" camera (keyboard for movement, mouse for aim)
            if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
            {
                ::cameraEye.x += ::cameraTarget.x * cameraMoveSpeed;
                ::cameraEye.z += ::cameraTarget.z * cameraMoveSpeed;
                //::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
            }
            if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
            {
                ::cameraEye.x -= ::cameraTarget.x * cameraMoveSpeed;
                ::cameraEye.z -= ::cameraTarget.z * cameraMoveSpeed;
                //::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
            }
            if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)	// "left"
            {
                ::cameraEye -= glm::normalize(glm::cross(::cameraTarget, ::upVector)) * cameraMoveSpeed;
                //::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
            }
            if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)	// "right"
            {
                ::cameraEye += glm::normalize(glm::cross(::cameraTarget, ::upVector)) * cameraMoveSpeed;
                //::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
            }
            if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
            {
                ::cameraEye += cameraMoveSpeed * ::upVector;
                //::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
            }
            if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)	// "down"
            {
                ::cameraEye -= cameraMoveSpeed * ::upVector;
                //::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
            }
        }

        //if (glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_PRESS)
        //{
        //    //Open bay doors
        //    ::g_pTheLights->TurnOnLight(1);
        //    ::openDoors = true;
        //    ::closeDoors = false;
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS)
        //{
        //    //Close bay doors
        //    ::g_pTheLights->TurnOnLight(1);
        //    ::openDoors = false;
        //    ::closeDoors = true;
        //}

        if (glfwGetKey(pWindow, GLFW_KEY_B) == GLFW_PRESS)
        {
            ::g_turnDebugNormalsOn = !::g_turnDebugNormalsOn;
        }


        // For the texture height map displacement example:
       // const float TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED = 0.005f;
       // const float TERRAIN_MAP_DISPLACEMENT_ROTATION_SPEED = 0.025f;

        //if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) { ::g_heightMapUVOffsetRotation.x -= TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        //if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) { ::g_heightMapUVOffsetRotation.x += TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        //if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) { ::g_heightMapUVOffsetRotation.y -= TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        //if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) { ::g_heightMapUVOffsetRotation.y += TERRAIN_MAP_DISPLACEMENT_MOVEMENT_SPEED; }
        //// Rotation with the num-pad "+" and "-"
        //if (glfwGetKey(pWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS) { ::g_heightMapUVOffsetRotation.z += TERRAIN_MAP_DISPLACEMENT_ROTATION_SPEED; }
        //if (glfwGetKey(pWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) { ::g_heightMapUVOffsetRotation.z -= TERRAIN_MAP_DISPLACEMENT_ROTATION_SPEED; }

        std::stringstream strTitle;
        // std::cout << 
        //glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        strTitle << "GDP Game Jam!  Camera: "
            << ::cameraEye.x << ", "
            << ::cameraEye.y << ", "
            << ::cameraEye.z
            << " Camera Target: "
            << ::cameraTarget.x << ", "
            << ::cameraTarget.y << ", "
            << ::cameraTarget.z; //<< std::endl;

        ::g_TitleText = strTitle.str();

    }//if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )


    // If JUST the shift is down, move the "selected" object
    if (cGFLWKeyboardModifiers::isModifierDown(pWindow, true, false, false))
    {
        // Move the spotlight up and down
        //if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) { ::g_pTheLights->theLights[::g_selectedLight].position.y += lightMoveSpeed; }
        //if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) { ::g_pTheLights->theLights[::g_selectedLight].position.y -= lightMoveSpeed; }
    }//if ( cGFLWKeyboardModifiers::...


    // If JUST the CTRL is down, move the "selected" light
    if (cGFLWKeyboardModifiers::isModifierDown(pWindow, false, true, false))
    {
        // Move the spotlight around
        //if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) { ::g_pTheLights->theLights[::g_selectedLight].position.z += lightMoveSpeed; }
        //if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) { ::g_pTheLights->theLights[::g_selectedLight].position.z -= lightMoveSpeed; }
        //if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) { ::g_pTheLights->theLights[::g_selectedLight].position.x += lightMoveSpeed; }
        //if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) { ::g_pTheLights->theLights[::g_selectedLight].position.x -= lightMoveSpeed; }
    }//if ( cGFLWKeyboardModifiers::...

    return;
}


// We call these every frame
void handleAsyncMouse(GLFWwindow* window, double deltaTime)
{

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    //::g_pFlyCamera->setMouseXY(x, y);

    const float MOUSE_SENSITIVITY = 2.0f;


    // Mouse left (primary?) button pressed? 
    // AND the mouse is inside the window...
    //if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    //    && ::g_MouseIsInsideWindow)
    //{
    //    // Mouse button is down so turn the camera
    //    ::g_pFlyCamera->Yaw_LeftRight(::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime);

    //    ::g_pFlyCamera->Pitch_UpDown(-::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime);

    //}

    // Adjust the mouse speed
    if (::g_MouseIsInsideWindow)
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        // Adjust the movement speed based on the wheel position
        //::g_pFlyCamera->movementSpeed -= (::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY);

        //// Clear the mouse wheel delta (or it will increase constantly)
        //::g_pFlyCamera->clearMouseWheelValue();


        //if (::g_pFlyCamera->movementSpeed <= 0.0f)
        //{
        //    ::g_pFlyCamera->movementSpeed = 0.0f;
        //}
    }


    return;
}

void GLFW_cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        std::cout << "Mouse cursor is over the window" << std::endl;
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
        std::cout << "Mouse cursor is no longer over the window" << std::endl;
        ::g_MouseIsInsideWindow = false;
    }
    return;
}

// Called when the mouse scroll wheel is moved
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float mouseScrollWheelSensitivity = 0.1f;

    //::g_pFlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);

    return;
}

void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

//#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG
//    // Right button is pop-up
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//    {
//        ShowWindowsContextMenu(window, button, action, mods);
//    }
//#endif

    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


