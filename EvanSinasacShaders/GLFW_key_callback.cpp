#include "globalThings.h"
//Stuff done in class with Michael Feeney

#include <sstream>
#include <iostream>
#include "cPlayerEntity.h"

bool bShowAllLights = false;

// Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

/*static*/ void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    // From the help review session.
    // Move the camera to a location and look somewhere
    //if (key == GLFW_KEY_L && action == GLFW_PRESS)
    //{
    //    // Move the eye
    //    ::g_pFlyCamera->setEye(glm::vec3(8631.0f, -1487.0f, 13010.0f));

    //    // "look at" something.
    //    // Since we are using the fly camera, we can't just set the eye.
    //    //::g_pFlyCamera->setAt(glm::vec3(0.0f, 0.0f, 0.0f));

    //    ::g_pFlyCamera->Yaw_LeftRight(-80.0f);
    //    ::g_pFlyCamera->Pitch_UpDown(-10.0f);

    //}

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;

    // Change all models to wireframe
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        // TODO: Make a function that makes a list or queue and adds children to it rather than doing these nested for loops
        for (std::vector<cMesh*>::iterator it = ::g_vec_pMeshes.begin();
            it != ::g_vec_pMeshes.end(); it++)
        {
            (*it)->bIsWireframe = !(*it)->bIsWireframe;
            for (std::vector<cMesh*>::iterator iT = (*it)->vec_pChildMeshes.begin(); iT != (*it)->vec_pChildMeshes.end(); iT++)
            {
                (*iT)->bIsWireframe = !(*iT)->bIsWireframe;
                for (std::vector<cMesh*>::iterator It = (*iT)->vec_pChildMeshes.begin(); It != (*iT)->vec_pChildMeshes.end(); It++)
                {
                    (*It)->bIsWireframe = !(*It)->bIsWireframe;
                }
            }
        }
        for (unsigned int index = 0; index != ::vec_pFSMEntities.size(); index++)
        {
            ::vec_pFSMEntities[index]->m_Mesh->bIsWireframe = !::vec_pFSMEntities[index]->m_Mesh->bIsWireframe;
        }
        for (unsigned int index = 0; index != ::vec_pAllEntities.size(); index++)
        {
            ::vec_pAllEntities[index]->m_Mesh->bIsWireframe = !::vec_pAllEntities[index]->m_Mesh->bIsWireframe;
            ::vec_pAllEntities[index]->m_LowResMesh->bIsWireframe = !::vec_pAllEntities[index]->m_LowResMesh->bIsWireframe;
            for (unsigned int indexA = 0; indexA != ::vec_pAllEntities[index]->m_Mesh->vec_pChildMeshes.size(); indexA++)
            {
                ::vec_pAllEntities[index]->m_Mesh->vec_pChildMeshes[indexA]->bIsWireframe = !::vec_pAllEntities[index]->m_Mesh->vec_pChildMeshes[indexA]->bIsWireframe;
                for (unsigned int indexB = 0; indexB != ::vec_pAllEntities[index]->m_Mesh->vec_pChildMeshes[indexA]->vec_pChildMeshes.size(); indexB++)
                {
                    ::vec_pAllEntities[index]->m_Mesh->vec_pChildMeshes[indexA]->vec_pChildMeshes[indexB]->bIsWireframe = !::vec_pAllEntities[index]->m_Mesh->vec_pChildMeshes[indexA]->vec_pChildMeshes[indexB]->bIsWireframe;
                }
            }
        }
    }

    // Show debug triangles
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        ::g_bShowDebugObjects = !::g_bShowDebugObjects;
    }


    // Draw spheres at light locations
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        ::drawLightBalls = !::drawLightBalls;
    }

    // Ignore distance limit on lights
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        bShowAllLights = !bShowAllLights;
        glUniform1f(::pShaderProc->mapUniformName_to_UniformLocation["bUseAllLights"], (float)bShowAllLights);
    }

    // Draw spheres at graph node locations
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        for (unsigned int index = 0; index != ::g_vec_pNodes.size(); index++)
        {
            ::g_vec_pNodes[index]->bIsVisible = !::g_vec_pNodes[index]->bIsVisible;
        }
    }

    // Draw stencils over entities
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        ::g_bStencilsOn = !::g_bStencilsOn;
    }

    // Turn the bloom effect on
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        ::g_UseBloom = !::g_UseBloom;
    }

    // Turn higher res models on regardless of distance
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        ::g_DrawAllHighResModels = !::g_DrawAllHighResModels;
    }

    // Begin cheating (move the player to the exit)
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (!((cPlayerEntity*)::g_pPlayer)->cheating)
            ((cPlayerEntity*)::g_pPlayer)->StartCheating();
    }

    // Move player around the graph
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        if (!((cPlayerEntity*)::g_pPlayer)->cheating)
            ((cPlayerEntity*)::g_pPlayer)->Move("FORWARD");
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        if (!((cPlayerEntity*)::g_pPlayer)->cheating)
            ((cPlayerEntity*)::g_pPlayer)->Move("BACKWARDS");
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        if (!((cPlayerEntity*)::g_pPlayer)->cheating)
            ((cPlayerEntity*)::g_pPlayer)->Rotate("LEFT");
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        if (!((cPlayerEntity*)::g_pPlayer)->cheating)
            ((cPlayerEntity*)::g_pPlayer)->Rotate("RIGHT");
    }
    
    // Update the Full screen FBO resolution
    //if (key == GLFW_KEY_P && action == GLFW_PRESS)
    //{
    //    ::g_updateFBOResolution = !::g_updateFBOResolution;
    //}

    // Change camera mode
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        ::g_Mode++;
        if (::g_Mode > 2)
        {
            ::g_Mode = 0;
        }
        switch (::g_Mode)
        {
        case 0: // free roam fly camera
            ::g_ObservationMode = true;
            ::g_FirstPersonMode = false;
            ::g_OverheadMode = false;
            ::cameraEye = ::lastCamPosition;
            ::cameraTarget = ::lastCamLookAt;

            ::g_pTheLights->TurnOnLight(1);
            break;
        case 1: // first person mode to the player model
            ::g_ObservationMode = false;
            ::g_FirstPersonMode = true;
            ::g_OverheadMode = false;
            ::lastCamPosition = ::cameraEye;
            ::lastCamLookAt = ::cameraTarget;
            // set cameraEye and cameraTarget to player position
            ::cameraEye = ((cPlayerEntity*)::g_pPlayer)->position;
            ::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;

            ::g_pTheLights->TurnOffLight(1);
            break;
        case 2: // overhead of the player model
            ::g_ObservationMode = false;
            ::g_FirstPersonMode = false;
            ::g_OverheadMode = true;
            // set cameraEye and cameraTarget to above
            ::cameraEye = ((cPlayerEntity*)::g_pPlayer)->position + glm::vec3(0.0f, 15.0f, 0.0f);
            ::cameraTarget = glm::vec3(0.0f, -1.0f, 0.0f);

            ::g_pTheLights->TurnOffLight(1);
            break;
        default:
            break;
        }
        //if (::g_ObservationMode)
        //{
        //    ::cameraEye = ::lastCamPosition;
        //    ::cameraTarget = ::lastCamLookAt;
        //}
        //else
        //{
        //    ::lastCamPosition = ::cameraEye;
        //    ::lastCamLookAt = ::cameraTarget;
        //    // set cameraEye and cameraTarget to player position
        //    ::cameraEye = ((cPlayerEntity*)::g_pPlayer)->position;
        //    ::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;
        //}
    }

    // Turn on various 2D effects
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 0;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 1;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 2;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 3;
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 4;
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 5;
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 6;
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 7;
    }
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 8;
    }
    else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 9;
    }

    // Change the speed that entities make decisions at
    if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 1.0f;
    }
    else if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 2.0f;
    }
    else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 5.0f;
    }
    else if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 10.0f;
    }

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }


    //   // If JUST the shift is down, move the "selected" object
       if ( bShiftDown && (!bControlDown) && (!bAltDown) )
       {
           
    
    
       }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

       // If JUST the ALT is down, move the "selected" light
    if ((!bShiftDown) && (!bControlDown) && bAltDown)
    {
        

    }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    return;
}
