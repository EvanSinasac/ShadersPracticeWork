#include "cMesh.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "GLCommon.h"       // Gives us glad.h
#include "cVAOManager.h"
#include "globalThings.h"


void GLFW_window_size_callback(GLFWwindow* window, int width, int height)
{
    // TODO: GLFW_window_size_callback()

    return;
}


// HACK: We shouldn't do the getUniformLocation every frame.
//  These don't change, so we should store them outside    
void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgram)
{
    //GLuint textureOperator_Location = glGetUniformLocation(shaderProgram, "textureOperator");
    //glUniform1i(textureOperator_Location, pCurrentMesh->textureOperator);
    glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["textureOperator"], pCurrentMesh->textureOperator);

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[0] >= 0.0f)
    {
        // uniform sampler2D texture_00;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Fauci.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[0]);

        GLuint textureUnit = 0;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        //GLint texture_00_LocID = glGetUniformLocation(shaderProgram, "texture_00");
        //glUniform1i(texture_00_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texture_00"], textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[1] >= 0.0f)
    {
        // uniform sampler2D texture_01;
        //GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Pebbleswithquarzite.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[1]);

        GLuint textureUnit = 1;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        //GLint texture_01_LocID = glGetUniformLocation(shaderProgram, "texture_01");
        //glUniform1i(texture_01_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texture_01"], textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[2] >= 0.0f)
    {
        // uniform sampler2D texture_01;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[2]);

        GLuint textureUnit = 2;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        //GLint texture_02_LocID = glGetUniformLocation(shaderProgram, "texture_02");
        //glUniform1i(texture_02_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texture_02"], textureUnit);
    }
    //*****************************************************************************************    

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[3] >= 0.0f) {
        // uniform sampler2D texture_01;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Broc_tree_house.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);

        GLuint textureUnit = 3;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        //GLint texture_03_LocID = glGetUniformLocation(shaderProgram, "texture_03");
        //glUniform1i(texture_03_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texture_03"], textureUnit);
    }
    //*****************************************************************************************   

    if (pCurrentMesh->textureRatios[4] >= 0.0f)
    {
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[4]);

        GLuint textureUnit = 4;
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        //GLint texture_04_LocID = glGetUniformLocation(shaderProgram, "texture_04");
        //glUniform1i(texture_04_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texture_04"], textureUnit);
    }

    // Set all the texture ratios in the shader
    //GLint texture2DRatios0to3_LocID = glGetUniformLocation(shaderProgram, "texture2DRatios0to3");
    // Set them
    //glUniform4f(texture2DRatios0to3_LocID,
    glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["texture2DRatios0to3"],
        pCurrentMesh->textureRatios[0],
        pCurrentMesh->textureRatios[1],
        pCurrentMesh->textureRatios[2],
        pCurrentMesh->textureRatios[3]);

    //GLint texture2DRatios4to7_LocID = glGetUniformLocation(shaderProgram, "texture2DRatios4to7");
    //glUniform4f(texture2DRatios4to7_LocID,
    glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["texture2DRatios4to7"],
        pCurrentMesh->textureRatios[4],
        pCurrentMesh->textureRatios[5],
        pCurrentMesh->textureRatios[6],
        pCurrentMesh->textureRatios[7]);
    

    // TODO: Make this more "fancy", but for now, we'll set a single cubemap
    // Out "1st" cubemap is set to texture unit 40 (so we don't overlap the 2D textures)
    // cubeMap_00 TropicalSunnyDay
    // cubeMap_01 Space01
    {
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("TropicalSunnyDay");

        // Be careful that you don't mix up the 2D and the cube assignments for the texture units
        //
        // Here, I'll say that the cube maps start at texture unit 40
        //
        GLuint textureUnit = 40;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

        // ***NOTE*** Binding to a CUBE MAP not a 2D Texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        //GLint cubeMap_00_LocID = glGetUniformLocation(shaderProgram, "cubeMap_00");
        //glUniform1i(cubeMap_00_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["cubeMap_00"], textureUnit);
    }

    {
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Space01");
       // GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Skyrim");
        GLuint textureUnit = 41;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);
        //GLint cubeMap_01_LocID = glGetUniformLocation(shaderProgram, "cubeMap_01");
        //glUniform1i(cubeMap_01_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["cubeMap_01"], textureUnit);
    }
    // TODO: And so on, for the remaining cubemap texture assignments...

    {
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("City01");
        GLuint textureUnit = 42;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);
        //GLint cubeMap_02_LocID = glGetUniformLocation(shaderProgram, "cubeMap_02");
        //glUniform1i(cubeMap_02_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["cubeMap_02"], textureUnit);
    }

    {
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Skyrim");
        GLuint textureUnit = 43;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);
        //GLint cubeMap_03_LocID = glGetUniformLocation(shaderProgram, "cubeMap_03");
        //glUniform1i(cubeMap_03_LocID, textureUnit);
        glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["cubeMap_03"], textureUnit);
    }

    // Set up the blend values for the skyboxes
    {
        //GLint cubeMap_Ratios0to3_LocID = glGetUniformLocation(shaderProgram, "cubeMap_Ratios0to3");

        // Hack: Change the blend ratios of the skyboxes over time
        float howMuch_cubeMap_00 = 1.0f;
        float howMuch_cubeMap_01 = 0.0f;
        float howMuch_cubeMap_02 = 0.0f;
        float howMuch_cubeMap_03 = 0.0f;

        howMuch_cubeMap_00 = fabs( (float)glm::cos(glfwGetTime() / 10.0) );
        howMuch_cubeMap_01 = 1.0f - howMuch_cubeMap_00;

        glUniform4f(
            pShaderProc->mapUniformName_to_UniformLocation["cubeMap_Ratios0to3"],       //cubeMap_Ratios0to3_LocID,
            howMuch_cubeMap_00,   // How much "tropical sunny day" we have
            howMuch_cubeMap_01,   // How much "space" we have
            howMuch_cubeMap_02,   // How much "city" we have
            howMuch_cubeMap_03);  // How much "skyrim" we have
    }

    return;
}


void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel,
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager)
{
    if (!pCurrentMesh->bIsVisible)
    {
        return;
    }

    // Set up textures for this object
    SetUpTextures(pCurrentMesh, program);

    //if (pCurrentMesh->friendlyName == "Torch Light 1" || pCurrentMesh->friendlyName == "Torch Light 2")
    //{
    //    float randomAddedAtten = (rand() % 100) / 100.0f;
    //    pCurrentMesh->scale.z = 0.1f + randomAddedAtten * gGetRandBetween<float>(1.0f, 2.0f);
    //}


    // Is this thing transparent, or an impostor object?
    if ((pCurrentMesh->alphaTransparency < 1.0f) || (pCurrentMesh->bIsImposter))
    {
        // Alpha transparency
        glEnable(GL_BLEND);
        // Basic "alpha transparency"
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        // It's not
        glDisable(GL_BLEND);
    }


    // Set the flag if this is an impostor
    //GLint bIsImposter_LocID = glGetUniformLocation(program, "bIsImposter");
    if (pCurrentMesh->bIsImposter)
    {
        //glUniform1f(bIsImposter_LocID, (float)GL_TRUE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bIsImposter"], (float)GL_TRUE);
    }
    else
    {
       // glUniform1f(bIsImposter_LocID, (float)GL_FALSE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bIsImposter"], (float)GL_FALSE);

    }

    // Get the uniform (should be outside of the draw call)
    //GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(program, "wholeObjectAlphaTransparency");
    //Set this value here
    //glUniform1f(wholeObjectAlphaTransparency_LocID, pCurrentMesh->alphaTransparency);
    glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["wholeObjectAlphaTransparency"], 
        pCurrentMesh->alphaTransparency);

    // *****************************************************
    // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        pCurrentMesh->positionXYZ);
    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        pCurrentMesh->orientationXYZ.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        pCurrentMesh->orientationXYZ.y,
        glm::vec3(0.0f, 1.0f, 0.0f));
    //matModel = matModel * rotateY;
    // *****************************************************

    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        pCurrentMesh->orientationXYZ.x,
        glm::vec3(1.0f, 0.0f, 0.0f));
    //matModel = matModel * rotateX;
    // *****************************************************


    // *****************************************************
    // Scale the model
    //glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
    //    glm::vec3(pCurrentMesh->scale,  // Scale in X
    //        pCurrentMesh->scale,  // Scale in Y
    //        pCurrentMesh->scale));// Scale in Z
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        pCurrentMesh->scale);
//matModel = matModel * matScale;
// *****************************************************

// *****************************************************
    matModel = matModel * pCurrentMesh->matModelBaseOffset;
    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;     // <-- mathematically, this is 1st

    // Save this matrix in the mesh object
    pCurrentMesh->matLastDrawCallWorldMatrix = matModel;

    // Now the matModel ("Model" or "World") matrix
    //  represents ALL the transformations we want, in ONE matrix.

    // Moved view and projection ("v" and "p") to outside draw scene loop.
    // (because they are the same for all objects)

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));


    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));


    // Copy the whole object colour information to the sahder               

    //// This is used for wireframe or whole object colour. 
    //// If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
    //GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
    //GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");

    //// If true, then the lighting contribution is NOT used. 
    //// This is useful for wireframe object
    //GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");

    //// The "whole object" colour (diffuse and specular)
    //GLint wholeObjectDiffuseColour_Location = glGetUniformLocation(program, "wholeObjectDiffuseColour");
    //GLint bUseWholeObjectDiffuseColour_Location = glGetUniformLocation(program, "bUseWholeObjectDiffuseColour");
    //GLint wholeObjectSpecularColour_Location = glGetUniformLocation(program, "wholeObjectSpecularColour");

    if (pCurrentMesh->bUseWholeObjectDiffuseColour)
    {
        //glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bUseWholeObjectDiffuseColour"], (float)GL_TRUE);
        glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["wholeObjectDiffuseColour"],     //wholeObjectDiffuseColour_Location,
            pCurrentMesh->wholeObjectDiffuseRGBA.r,
            pCurrentMesh->wholeObjectDiffuseRGBA.g,
            pCurrentMesh->wholeObjectDiffuseRGBA.b,
            pCurrentMesh->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        //glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bUseWholeObjectDiffuseColour"], (float)GL_FALSE);
    }

    glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"],    //wholeObjectSpecularColour_Location,
        pCurrentMesh->wholeObjectSpecularRGB.r,
        pCurrentMesh->wholeObjectSpecularRGB.g,
        pCurrentMesh->wholeObjectSpecularRGB.b,
        pCurrentMesh->wholeObjectShininess_SpecPower);


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (pCurrentMesh->bUseObjectDebugColour)
    {
        // Override the colour...
        //glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bUseDebugColour"], (float)GL_TRUE);
        glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["objectDebugColour"],        //objectDebugColour_Location,
            pCurrentMesh->objectDebugColourRGBA.r,
            pCurrentMesh->objectDebugColourRGBA.g,
            pCurrentMesh->objectDebugColourRGBA.b,
            pCurrentMesh->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        //glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bUseDebugColour"], (float)GL_FALSE);
    }


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (pCurrentMesh->bDontLight)
    {
        // Override the colour...
        //glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDontLightObject"], (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        //glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
        glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDontLightObject"], (float)GL_FALSE);
    }

    
    // Wireframe
    if (pCurrentMesh->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Depth buffer
    if (pCurrentMesh->bDisableDepthBufferCheck)
    {
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
    }



    // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!
    //if (::g_turnDebugNormalsOn)
    //{
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    glLineWidth(20.0f);

    //    GLint fVertWorld_Location = glGetUniformLocation(program, "fVertWorldLocation");
    //    GLint fNormal_Location = glGetUniformLocation(program, "fNormal");

    //    

    //    /*for (std::map<unsigned int, std::vector<glm::vec3>>::iterator it = ::g_debugNormals.begin();
    //        it != ::g_debugNormals.end(); it++)
    //    {
    //        glBegin(GL_LINES);
    //            glVertex3f(it->second.at(0).x, it->second.at(0).y, it->second.at(0).z);
    //            glVertex3f(it->second.at(0).x + it->second.at(1).x,
    //                it->second.at(0).y + it->second.at(1).y,
    //                it->second.at(0).z + it->second.at(1).z);
    //        glEnd();
    //    }*/


    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}


    sModelDrawInfo modelInfo;
    //        if (gVAOManager.FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
    //        if (gVAOManager.FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

    if (pVAOManager->FindDrawInfoByModelName(pCurrentMesh->meshName, modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);
    }

    // Now draw all the "child" objects.
    for (std::vector<cMesh*>::iterator itChildMesh = pCurrentMesh->vec_pChildMeshes.begin();
        itChildMesh != pCurrentMesh->vec_pChildMeshes.end(); itChildMesh++)
    {
        cMesh* pChildMesh = *itChildMesh;

        // To clarify, we are passing this object's world matrix as the starting world matris:
        // (this copy here isn't needed, but hoefully clarifies what's happening
        glm::mat4 matModel_Parent = matModel;

        // Call DrawObject again, but I pass the PARENT's final model matrix value
        // Which will make the child's "origin" at the final location of the parent.
        DrawObject(pChildMesh,
            matModel_Parent/*matModel*/,       // The parent's final mat mode, not an identity matrix
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            pVAOManager);
    } // for (cMesh*::iterator itChildMesh...


    return;
}
