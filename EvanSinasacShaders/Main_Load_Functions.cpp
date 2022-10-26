#include "globalThings.h"
#include <sstream>
#include <iostream>
#include "cDungeonMeshBuilder.h"

#include "cTorchObject.h"

// Modified from previous versions to take texture information
bool loadWorldFile(unsigned int& numberOfTransparentObjects)
{
	std::stringstream ss;
	std::stringstream sFile;


	//ss << SOLUTION_DIR << "common\\assets\\worldFile.txt";
	ss << "assets\\worldFile.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open worldFile.txt");
		return false;
	}

	std::string nextToken;
	ss.str("");

	//Throwaway text describing the format of the file
	theFile >> nextToken;       //ModelFileName(extension)
	theFile >> nextToken;       //Position(x,y,z)
	theFile >> nextToken;       //Orientation(x,y,z)
	theFile >> nextToken;       //Scale(x,y,z)
	theFile >> nextToken;       //Colour(r,g,b,a)
	theFile >> nextToken;		//UseColour
	theFile >> nextToken;		//Light
	theFile >> nextToken;		//TextureOp
	theFile >> nextToken;		//TextureName1
	theFile >> nextToken;		//Ratio1
	theFile >> nextToken;		//TextureName2
	theFile >> nextToken;		//Ratio2
	theFile >> nextToken;		//TextureName3
	theFile >> nextToken;		//Ratio3
	theFile >> nextToken;		//DiscardTexture

	theFile >> nextToken;		//Camera(x,y,z)
	theFile >> nextToken;		//x position
	::cameraEye.x = std::stof(nextToken);
	theFile >> nextToken;		//y position
	::cameraEye.y = std::stof(nextToken);
	theFile >> nextToken;		//z position
	::cameraEye.z = std::stof(nextToken);

	theFile >> nextToken;						// initial camera target for observation mode
	::cameraTarget.x = std::stof(nextToken);	// x
	theFile >> nextToken;
	::cameraTarget.y = std::stof(nextToken);	// y
	theFile >> nextToken;
	::cameraTarget.z = std::stof(nextToken);	// z

	theFile >> nextToken;		//number of transparent objects
	numberOfTransparentObjects = std::stoi(nextToken);

	//From here modify based on worldFile format
	while (theFile >> nextToken)    //this should always be the name of the model to load or end.  Potential error check, add a check for "ply" in the mdoel name
	{
		cMesh* curMesh = new cMesh;
		if (nextToken == "end")
		{
			break;
		}
		std::cout << nextToken << std::endl;        //Printing model names to console, just making sure we're loading ok.  Can be commented out whenever
		//First is the file name of model
		//ss << SOLUTION_DIR << "common\\assets\\models\\" << nextToken;		// don't need this with the setBasePath function for the loading intot the VAO
		curMesh->meshName = nextToken;
		//Next 3 are the position of the model
		theFile >> nextToken;                                               //x position for the model
		curMesh->positionXYZ.x = std::stof(nextToken);
		theFile >> nextToken;                                               //y position for the model
		curMesh->positionXYZ.y = std::stof(nextToken);
		theFile >> nextToken;                                               //z position for the model
		curMesh->positionXYZ.z = std::stof(nextToken);
		//Next 3 are the orientation of the model
		theFile >> nextToken;                                               //x orientation value
		//curMesh.orientationXYZ.x = std::stof(nextToken);
		curMesh->orientationXYZ.x = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //y orientation value
		//curMesh.orientationXYZ.y = std::stof(nextToken);
		curMesh->orientationXYZ.y = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //z orientation value
		//curMesh.orientationXYZ.z = std::stof(nextToken);
		curMesh->orientationXYZ.z = glm::radians(std::stof(nextToken));
		//Next is the scale to multiply the model by
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.x = std::stof(nextToken);
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.y = std::stof(nextToken);
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.z = std::stof(nextToken);

		//Next 3 are the r, g, b values for the model
		curMesh->bUseWholeObjectDiffuseColour = false;
		theFile >> nextToken;													//RGB red value
		curMesh->wholeObjectDiffuseRGBA.r = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB green value
		curMesh->wholeObjectDiffuseRGBA.g = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB blue value
		curMesh->wholeObjectDiffuseRGBA.b = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;
		curMesh->wholeObjectDiffuseRGBA.a = std::stof(nextToken);
		curMesh->alphaTransparency = std::stof(nextToken);
		theFile >> nextToken;
		curMesh->bUseWholeObjectDiffuseColour = (nextToken == "1");				// 1 means use colour, anything else means don't

		// Light
		theFile >> nextToken;
		curMesh->bDontLight = (nextToken == "0");		// set to true or "0" in the worldFile to see textures without lighting

		// Texture operator
		theFile >> nextToken;
		curMesh->textureOperator = std::stoi(nextToken);

		// texture for mesh is in worldFile.txt now
		curMesh->clearTextureRatiosToZero();

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[1] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[1] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[2] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[2] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[3] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[3] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		if (curMesh->textureRatios[1] == 0.0f && curMesh->textureRatios[2] == 0.0f && curMesh->textureRatios[3] == 0.0f)
		{
			curMesh->textureRatios[0] = 1.0f;
		}

		// Discard texture
		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->discardTexture = nextToken;
			curMesh->bHasDiscardTexture = true;
		}

		::g_vec_pMeshes.push_back(curMesh);     //push the model onto our vector of meshes
		ss.str("");                         //reset the stringstream
	} //end of while
	theFile.close();
	return true;
}	//end of load world file

bool loadLightsFile()
{
	cDungeonMeshBuilder main_DungeonBuilder;
	std::stringstream ss;
	std::stringstream sFile;

	//std::vector<float> rotations;

	//ss << SOLUTION_DIR << "common\\assets\\lights.txt";
	ss << "assets\\lights.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open lights.txr");
		return false;
	}

	std::string nextToken;
	ss.str("");

	// Throw away description 
	theFile >> nextToken;		// position(x,y,z)
	theFile >> nextToken;		// diffuse(r,g,b)
	theFile >> nextToken;		// specular(r,g,b)
	theFile >> nextToken;		// atten(x,y,z,w)
	theFile >> nextToken;		// direction(x,y,z)
	theFile >> nextToken;		// param1(x,y,z)
	theFile >> nextToken;		// param2(x)

	theFile >> nextToken;		// rotation (for the torch objects)

	::g_pTheLights->TurnOffLight(0);

	unsigned int index = 0;	// can't start at 0 because for some reason the 0 light over writes all other lights

	while (theFile >> nextToken)
	{
		if (nextToken == "end" || index >= cLightManager::NUMBER_OF_LIGHTS)
		{
			break;
		}
		glm::vec3 position;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec4 atten;
		glm::vec3 direction;
		glm::vec3 param1;
		float param2;

		float rotation;

		// Position
		position.x = std::stof(nextToken);
		theFile >> nextToken;
		position.y = std::stof(nextToken);
		theFile >> nextToken;
		position.z = std::stof(nextToken);

		// Diffuse
		theFile >> nextToken;
		diffuse.x = std::stof(nextToken);
		theFile >> nextToken;
		diffuse.y = std::stof(nextToken);
		theFile >> nextToken;
		diffuse.z = std::stof(nextToken);

		// Specular
		theFile >> nextToken;
		specular.x = std::stof(nextToken);
		theFile >> nextToken;
		specular.y = std::stof(nextToken);
		theFile >> nextToken;
		specular.z = std::stof(nextToken);

		// Atten
		theFile >> nextToken;
		atten.x = std::stof(nextToken);
		theFile >> nextToken;
		atten.y = std::stof(nextToken);
		theFile >> nextToken;
		atten.z = std::stof(nextToken);
		theFile >> nextToken;
		atten.w = std::stof(nextToken);

		// Direction
		theFile >> nextToken;
		direction.x = std::stof(nextToken);
		theFile >> nextToken;
		direction.y = std::stof(nextToken);
		theFile >> nextToken;
		direction.z = std::stof(nextToken);

		// Param1
		theFile >> nextToken;
		param1.x = std::stof(nextToken);;
		theFile >> nextToken;
		param1.y = std::stof(nextToken);
		theFile >> nextToken;
		param1.z = std::stof(nextToken);

		// Param2
		theFile >> nextToken;
		param2 = std::stof(nextToken);

		// Torches rotation
		theFile >> nextToken;
		rotation = std::stof(nextToken);

		// Load everything into the lights
		float ogX = position.x;
		float ogY = position.y;
		float ogZ = position.z;


		float intX = (int)position.x;
		float diffX = ogX - intX;
		if (diffX > 0.5f)
		{
			diffX = -1.0f + diffX;
		}
		if (diffX != 0.5f)
		{
			position.x += diffX * 3.0f;
			//position.x += 0.75f;
		}

		position.y += 0.575f;

		float intZ = (int)position.z;
		float diffZ = ogZ - intZ;
		if (diffZ > 0.5f)
		{
			diffZ = -1.0f + diffZ;
		}
		if (diffZ != 0.5f)
		{
			position.z += diffZ * 3.0f;
			//position.z += 0.75f;
		}



		::g_pTheLights->theLights[index].position = glm::vec4(position, 1.0f);


		::g_pTheLights->theLights[index].diffuse = glm::vec4(diffuse, 1.0f);
		::g_pTheLights->theLights[index].specular = glm::vec4(specular, 1.0f);
		::g_pTheLights->theLights[index].atten = atten;
		::g_pTheLights->theLights[index].direction = glm::vec4(direction, 1.0f);
		::g_pTheLights->theLights[index].param1 = glm::vec4(param1, 1.0f);
		::g_pTheLights->theLights[index].param2 = glm::vec4(param2, 0.0f, 0.0f, 1.0f);

		//rotations.push_back(rotation);

		if (::g_pTheLights->theLights[index].param1.x == 0)
		{
			// Make torch entities where there are point lights
			cMesh* torchSet = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::LIGHT_FIXTURE, glm::vec3(1.0f));
			torchSet->positionXYZ = glm::vec3(ogX, ogY, ogZ);
			torchSet->orientationXYZ = glm::vec3(0.0f, glm::radians(rotation), 0.0f);
			iEntity* torchObject = new cTorchObject(torchSet, index);
			//::vec_pTorches.push_back(torchObject);
			//::g_vec_pMeshes.push_back(torchSet);

			::vec_pAllEntities.push_back(torchObject);

			cMesh* pointLightBall = new cMesh();
			pointLightBall->meshName = "Isosphere_Smooth_Normals.ply";
			pointLightBall->bIsWireframe = true;
			pointLightBall->bDontLight = true;
			pointLightBall->bUseWholeObjectDiffuseColour = true;
			pointLightBall->wholeObjectDiffuseRGBA = ::g_pTheLights->theLights[index].specular;
			pointLightBall->positionXYZ = ::g_pTheLights->theLights[index].position;
			pointLightBall->setUniformScale(0.1f);
			::g_vec_pPointLights.push_back(pointLightBall);
		}

		index++;
	} //end of while

	//mazeLightsStartIndex = index;
	//::g_currentLightIndex = index;

	theFile.close();

	return true;

} //end of load lights

// Graphics 2 Midterm Models
void loadGraphicsMidtermModels(std::vector<std::string>& modelLocations)
{
	// Midterm Models
	modelLocations.push_back("Entire_Station_xyz_rgba_n_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply");

	modelLocations.push_back("SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_04_xyz_n_rgba_uv.ply");

	// yeah, the FBO might want this... lol
	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	// space station
	modelLocations.push_back("Arms Small_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Arms_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Cooling Arms_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Hanger_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Parts_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Reactor_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Ring Small1_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Ring Small2_xyz_rgba_n_uv.ply");
	modelLocations.push_back("RingBig_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Solar Panels_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Triangle Parts_xyz_rgba_n_uv.ply");

} //end of loadMidtermModels

// Graphics 2 Project 2 Models
void loadGraphicsProject2Models(std::vector<std::string>& modelLocations)
{
	modelLocations.push_back("SM_Prop_Beaker_01_xyz_n_rgba_uv.ply");

	// Follow along models
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("mod-bomber_xyz_n_rgba_uv.ply");

	// DFK Models
	modelLocations.push_back("dfk_bed_single_01_XYZ_N_RGBA_UV_tranformed.ply");
	modelLocations.push_back("dfk_bookshelf_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_chairs_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_crystal_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_crystal_02_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_floor_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_table_large_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_XYZ_N_RGBA_N_transformed.ply");
	modelLocations.push_back("dfk_wardrobe_01_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_torch_XYZ_N_RGBA_UV_transformed.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Engine_Exhaust_Imposter.ply");
	//modelLocations.push_back("dfk_bed_single_01.ply");
	//modelLocations.push_back("dfk_bookshelf.ply");
	//modelLocations.push_back("dfk_chairs.ply");
	//modelLocations.push_back("dfk_crystal_01.ply");
	//modelLocations.push_back("dfk_crystal_02.ply");
	//modelLocations.push_back("dfk_floor.ply");
	//modelLocations.push_back("dfk_table_large.ply");
	//modelLocations.push_back("dfk_wall_1.ply");
	//modelLocations.push_back("dfk_wardrobe_01.ply");

	modelLocations.push_back("SM_Prop_Desk_04_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Prop_Monitor_03_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Prop_SwivelChair_01_xyz_n_rgba_uv.ply");

} //end of loadProject2Models

// AI Project 3 Models
void loadAIModels(std::vector<std::string>& modelLocations)
{
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
}

// GameJam Models
void loadGameJamModels(std::vector<std::string>& modelLocations)
{
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");

	modelLocations.push_back("dfk_bed_single_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_ceiling_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_door_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_doorframe_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_floor_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_gate_small_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_stairs_wood_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_bevel_door_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_XYZ_N_RGBA_N_transformed.ply");
	modelLocations.push_back("dfk_bookshelf_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_chairs_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_crystal_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_crystal_02_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_table_large_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wardrobe_01_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_chest_02_closed_XYZ_N_RGBA_UV_transformed.ply");

	modelLocations.push_back("dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_torch_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("Engine_Exhaust_Imposter.ply");	// for the torch lights
	modelLocations.push_back("Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply");

	modelLocations.push_back("SM_Env_Consoles_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply");

	modelLocations.push_back("SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_01_xyz_n_rgba_uv.ply");

	//modelLocations.push_back("SK_Anglerox_XYZ_N_RGBA_UV_converted_3.ply");
	modelLocations.push_back("Pokemon.ply");


	modelLocations.push_back("dalek2005_XYZ_N_RGBA_UV_hi_res.ply");
	modelLocations.push_back("dalek2005_XYZ_N_RGBA_UV_low_res.ply");
	modelLocations.push_back("hollowknight_XYZ_N_RGBA_UV.ply");
}

bool loadTextures()
{
	bool loadedAll = true;
	//Michael Feeney's BMP texture mapping
	//// Load the textures

	std::stringstream ss;
	//ss << SOLUTION_DIR << "common\\assets\\textures\\";
	ss << "assets\\textures\\";
	::g_pTextureManager->SetBasePath(ss.str());

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Scope.bmp", true);

	//if (::g_pTextureManager->Create2DTextureFromBMPFile("Scope.bmp", true))
	//{
	//	std::cout << "Loaded Scope texture" << std::endl;
	//}
	//else
	//{
	//	std::cout << "DIDN'T load Scope Texture" << std::endl;
	//}

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Window.bmp", true);

	//if (::g_pTextureManager->Create2DTextureFromBMPFile("Window.bmp", true))
	//{
	//	std::cout << "Loaded Window texture" << std::endl;
	//}
	//else
	//{
	//	std::cout << "DIDN'T load Window texture" << std::endl;
	//}

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true);

	// Default don't have a texture
	//if (::g_pTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true))
	//	//if (::g_pTextureManager->Create2DTextureFromBMPFile("Pebbleswithquarzite.bmp", true))
	//{
	//	std::cout << "Loaded Bright Coloured texture" << std::endl;
	//}
	//else
	//{
	//	std::cout << "DIDN'T load Bright Coloured texture" << std::endl;
	//}

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Long_blue_Jet_Flame.bmp", true);

	//if (::g_pTextureManager->Create2DTextureFromBMPFile("Long_blue_Jet_Flame.bmp", true))
	//{
	//	std::cout << "Loaded long blue jet flame texture" << std::endl;
	//}
	//else
	//{
	//	std::cout << "DIDN'T load long blue jet flame texture" << std::endl;
	//}

	//if (::g_pTextureManager->Create2DTextureFromBMPFile("uv_hollow.bmp", true))
	//{
	//	std::cout << "uv_hollow finished ok" << std::endl;
	//}
	//else
	//{
	//	std::cout << "uv_hollow did not finish ok" << std::endl;
	//}
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("uv_hollow.bmp", true);


	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("SpaceInteriors_Texture.bmp", true);
	//loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("SpaceInteriors_Emmision.bmp", true);
	//loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("WorldMap.bmp", true);


	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Final_Pokemon_Diffuse.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Final_Pokemon_Normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Final_Pokemon_Ambient_Occlusion.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("Dinosaur-Footprints-clipart-2.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("treads.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("metroid.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("metroidDreadSpider.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("metroidVisor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("static.bmp", true);

	ss << "DFK Textures\\";
	::g_pTextureManager->SetBasePath(ss.str());
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bed_single_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bed_single_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bed_single_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_ceiling_stone_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_ceiling_stone_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_ceiling_stone_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doors_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doors_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doors_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_floor_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_floor_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_floor_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_wood_01_albedo.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_wood_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_wood_01_metallic.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wall_a_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wall_a_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wall_a_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doorframe_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doorframe_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doorframe_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_gate_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_gate_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_gate_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_candleholders_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_candleholders_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_candleholders_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bookshelf_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bookshelf_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bookshelf_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_braziers_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_braziers_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_braziers_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_chair_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_chair_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_chair_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_table_large_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_table_large_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_table_large_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wardrobe_01_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wardrobe_01_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wardrobe_01_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_small_containers_blue_basecolor.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_small_containers_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_small_containers_SmMetAO.bmp", true);

	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_blue_albedo.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_red_albedo.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_green_albedo.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_yellow_albedo.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_white_albedo.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_normal.bmp", true);
	loadedAll &= ::g_pTextureManager->Create2DTextureFromBMPFile("dfk_crystal_SmMetAO.bmp", true);

	
	return loadedAll;
}

bool loadDefaultSkyBox()
{
	bool loadedAll = true;
	// Add a skybox texture
	std::string errorTextString;
	std::stringstream ss;
	//ss << SOLUTION_DIR << "common\\assets\\textures\\cubemaps\\";
	ss << "assets\\textures\\cubemaps\\";
	::g_pTextureManager->SetBasePath(ss.str());		// update base path to cube texture location

	loadedAll &= ::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
		"TropicalSunnyDayRight2048.bmp",	/* +X */	"TropicalSunnyDayLeft2048.bmp" /* -X */,
		"TropicalSunnyDayUp2048.bmp",		/* +Y */	"TropicalSunnyDayDown2048.bmp" /* -Y */,
		"TropicalSunnyDayFront2048.bmp",	/* +Z */	"TropicalSunnyDayBack2048.bmp" /* -Z */,
		true, errorTextString);



	loadedAll &= ::g_pTextureManager->CreateCubeTextureFromBMPFiles("Space01",
		"SpaceBox_right1_posX.bmp",		/* +X */	"SpaceBox_left2_negX.bmp" /* -X */,
		"SpaceBox_top3_posY.bmp",		/* +Y */	"SpaceBox_bottom4_negY.bmp" /* -Y */,
		"SpaceBox_front5_posZ.bmp",		/* +Z */	"SpaceBox_back6_negZ.bmp" /* -Z */,
		true, errorTextString);

	loadedAll &= ::g_pTextureManager->CreateCubeTextureFromBMPFiles("City01",
		"city_lf.bmp",		/* +X */	"city_rt.bmp" /* -X */,
		"city_dn.bmp",		/* +Y */	"city_up.bmp" /* -Y */,
		"city_ft.bmp",		/* +Z */	"city_bk.bmp" /* -Z */,
		true, errorTextString);

	//// Turns out to make a "cube" map, the images need to be squares
	loadedAll &= ::g_pTextureManager->CreateCubeTextureFromBMPFiles("Skyrim",
		"Skyrim_Right.bmp",		/* +X */	"Skyrim_Left.bmp" /* -X */,
		"Skyrim_Top.bmp",		/* +Y */	"Skyrim_Bottom.bmp" /* -Y */,
		"Skyrim_Forward.bmp",		/* +Z */	"Skyrim_Back.bmp" /* -Z */,
		true, errorTextString);

	return loadedAll;
}


// load sounds from audio list
bool loadSounds()
{
	std::stringstream ss;
	std::stringstream sFile;
	//ss << SOLUTION_DIR << "common\\assets\\audio\\audioList.txt";
	ss << "assets\\audio\\audioList.txt";

	FMOD::Sound* sound;
	FMOD::Channel* channel;

	std::ifstream theFile(ss.str());
	ss.str("");

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open audioList.txt\n");
		return false;
	}
	std::string nextToken;
	while (theFile >> nextToken)
	{
		if (nextToken == "end")
		{
			break;
		}

		if (nextToken.find("mp3") != std::string::npos || nextToken.find("wav") != std::string::npos)
		{
			sFile << nextToken.c_str();
			ss << "assets\\audio\\" << sFile.str().c_str();
			songNames.push_back(sFile.str().c_str());
			// LinearRollOff makes the sound go mute when the listener has moved away from the max distance
			_result = g_pFMODSystem->createSound(ss.str().c_str(), FMOD_3D_LINEARROLLOFF, 0, &sound);
			if (_result != FMOD_OK)
			{
				fprintf(stderr, "Unable to create a sound: %s", ss.str().c_str());
			}
			else
			{
				//Set min/max distance
				_result = sound->set3DMinMaxDistance(0.5f, 20.0f);
				if (FMOD_OK != _result) {
					fprintf(stderr, "Unable to set min/max distance for sound: %s", ss.str().c_str());
					//return false;
				}
				
				//_result = sound->setMode(FMOD_LOOP_NORMAL);
				_result = sound->setMode(FMOD_LOOP_OFF);	// not looping
				if (FMOD_OK != _result) {
					fprintf(stderr, "Unable to set loop normal for sound: %s", ss.str().c_str());
				}

				g_vecSounds.push_back(sound);
				g_vecChannels.push_back(channel);
			}

			ss.str("");
			sFile.str("");
		}
		else
		{
			//in case audio file name contains spaces
			sFile << nextToken.c_str() << " ";
		}
	} //end while
	theFile.close();
	return true;
} //end of loadSounds