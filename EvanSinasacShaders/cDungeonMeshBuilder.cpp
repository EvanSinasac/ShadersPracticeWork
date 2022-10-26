#include "cDungeonMeshBuilder.h"
//#include "cTorchObject.h"
//#include "globalThings.h"

cDungeonMeshBuilder::cDungeonMeshBuilder()
{

}
cDungeonMeshBuilder::~cDungeonMeshBuilder()
{

}

// cDungeonMeshBuilder::MakeMesh makes the mesh and sets the textures and such, also sets position of children meshes
// to make the main.cpp a bit cleaner
// position and orientation of the mesh built here should still be set outside
cMesh* cDungeonMeshBuilder::MakeMesh(TypeOfMesh type, glm::vec3 scale)
{
	cMesh* meshToBuild = nullptr;
	bool dLight = false;					// TODO: This is true just for testing

	//FLOOR = 0,
	//WALL = 1,
	//DOOR = 2,
	//SECRETDOOR = 3,
	//PORTCULLIS = 4,
	//STAIRS = 5,
	//LIGHT_FIXTURE = 6,
	//BOOKSHELF = 7,
	//CHAIR = 8,
	//CRYSTAL_1 = 9,
	//CRYSTAL_2 = 10,
	//TABLE = 11,
	//CHEST = 12,

	if (type == TypeOfMesh::DOOR)
	{
		cMesh* doorWallMesh = new cMesh();
		doorWallMesh->meshName = "dfk_wall_1_bevel_door_XYZ_N_RGBA_UV_transformed.ply";
		doorWallMesh->scale = scale;
		doorWallMesh->positionXYZ = glm::vec3(0.0f);
		//orientation
		doorWallMesh->bDontLight = dLight;
		doorWallMesh->clearTextureRatiosToZero();
		doorWallMesh->textureNames[1] = "dfk_wall_a_basecolor.bmp";
		doorWallMesh->textureNames[2] = "dfk_wall_a_normal.bmp";
		doorWallMesh->textureNames[3] = "dfk_wall_a_SmMetAO.bmp";
		doorWallMesh->textureRatios[1] = 1.0f;
		doorWallMesh->textureRatios[2] = 1.0f;
		doorWallMesh->textureRatios[3] = 0.3f;
		doorWallMesh->textureOperator = 4;

		cMesh* doorFrameMesh = new cMesh();
		doorFrameMesh->meshName = "dfk_doorframe_XYZ_N_RGBA_UV_transformed.ply";
		doorFrameMesh->positionXYZ = glm::vec3(0.0f);
		// orientation
		doorFrameMesh->bDontLight = dLight;
		doorFrameMesh->clearTextureRatiosToZero();
		doorFrameMesh->textureNames[1] = "dfk_doorframe_basecolor.bmp";
		doorFrameMesh->textureNames[2] = "dfk_doorframe_normal.bmp";
		doorFrameMesh->textureNames[3] = "dfk_doorframe_SmMetAO.bmp";
		doorFrameMesh->textureRatios[1] = 1.0f;
		doorFrameMesh->textureRatios[2] = 1.0f;
		doorFrameMesh->textureRatios[3] = 0.3f;
		doorFrameMesh->textureOperator = 4;
		doorWallMesh->vec_pChildMeshes.push_back(doorFrameMesh);

		cMesh* doorMesh = new cMesh();
		doorMesh->meshName = "dfk_door_01_XYZ_N_RGBA_UV_transformed.ply";
		doorMesh->positionXYZ = glm::vec3(0.0f);
		// orientation
		doorMesh->bDontLight = dLight;
		doorMesh->clearTextureRatiosToZero();
		doorMesh->textureNames[1] = "dfk_doors_01_basecolor.bmp";
		doorMesh->textureNames[2] = "dfk_doors_01_normal.bmp";
		doorMesh->textureNames[3] = "dfk_doors_01_SmMetAO.bmp";
		doorMesh->textureRatios[1] = 1.0f;
		doorMesh->textureRatios[2] = 1.0f;
		doorMesh->textureRatios[3] = 0.3f;
		doorMesh->textureOperator = 4;
		doorWallMesh->vec_pChildMeshes.push_back(doorMesh);

		
		//meshToBuild->vec_pChildMeshes.push_back(doorWallMesh);
		meshToBuild = doorWallMesh;
	}
	else if (type == TypeOfMesh::FLOOR)
	{
		meshToBuild = new cMesh();
		meshToBuild->meshName = "dfk_floor_XYZ_N_RGBA_UV_transformed.ply";
		meshToBuild->scale = scale;
		meshToBuild->bDontLight = dLight;
		meshToBuild->clearTextureRatiosToZero();
		meshToBuild->textureNames[1] = "dfk_floor_01_basecolor.bmp";
		meshToBuild->textureNames[2] = "dfk_floor_01_normal.bmp";
		meshToBuild->textureNames[3] = "dfk_floor_01_SmMetAO.bmp";
		meshToBuild->textureRatios[1] = 1.0f;
		meshToBuild->textureRatios[2] = 1.0f;
		meshToBuild->textureRatios[3] = 0.3f;
		meshToBuild->textureOperator = 4;
	}
	else if (type == TypeOfMesh::PORTCULLIS)
	{
		meshToBuild = new cMesh();
		// the gate model has the door built into it
		meshToBuild->meshName = "dfk_gate_small_XYZ_N_RGBA_UV_transformed.ply";
		meshToBuild->scale = scale;
		meshToBuild->bDontLight = dLight;
		meshToBuild->clearTextureRatiosToZero();
		meshToBuild->textureNames[1] = "dfk_gate_01_basecolor.bmp";
		meshToBuild->textureNames[2] = "dfk_gate_01_normal.bmp";
		meshToBuild->textureNames[3] = "dfk_gate_01_SmMetAO.bmp";
		meshToBuild->textureRatios[1] = 1.0f;
		meshToBuild->textureRatios[2] = 1.0f;
		meshToBuild->textureRatios[3] = 0.3f;
		meshToBuild->textureOperator = 4;
	}
	else if (type == TypeOfMesh::SECRETDOOR)
	{
		meshToBuild = new cMesh();
		// the gate model has the door built into it
		meshToBuild->meshName = "dfk_gate_small_XYZ_N_RGBA_UV_transformed.ply";
		meshToBuild->scale = scale;
		meshToBuild->bDontLight = dLight;
		meshToBuild->clearTextureRatiosToZero();
		meshToBuild->textureNames[1] = "dfk_gate_01_basecolor.bmp";
		meshToBuild->textureNames[2] = "dfk_gate_01_normal.bmp";
		meshToBuild->textureNames[3] = "dfk_gate_01_SmMetAO.bmp";
		meshToBuild->textureRatios[1] = 1.0f;
		meshToBuild->textureRatios[2] = 1.0f;
		meshToBuild->textureRatios[3] = 0.3f;
		meshToBuild->textureOperator = 4;
	}
	else if (type == TypeOfMesh::STAIRS)
	{
		meshToBuild = new cMesh();
		meshToBuild->meshName = "dfk_stairs_wood_XYZ_N_RGBA_UV_transformed.ply";
		meshToBuild->scale = scale;
		meshToBuild->scale.x *= 2.0f;
		meshToBuild->bDontLight = dLight;
		meshToBuild->clearTextureRatiosToZero();
		meshToBuild->textureNames[1] = "dfk_stairs_01_basecolor.bmp";
		meshToBuild->textureNames[2] = "dfk_stairs_01_normal.bmp";
		meshToBuild->textureNames[3] = "dfk_stairs_01_SmMetAO.bmp";
		meshToBuild->textureRatios[1] = 1.0f;
		meshToBuild->textureRatios[2] = 1.0f;
		meshToBuild->textureRatios[3] = 0.3f;
		meshToBuild->textureOperator = 4;
	}
	else if (type == TypeOfMesh::WALL)
	{
		meshToBuild = new cMesh();
		meshToBuild->meshName = "dfk_wall_1_XYZ_N_RGBA_N_transformed.ply";
		meshToBuild->bDontLight = dLight;
		meshToBuild->clearTextureRatiosToZero();
		meshToBuild->textureNames[1] = "dfk_wall_a_basecolor.bmp";
		meshToBuild->textureNames[2] = "dfk_wall_a_normal.bmp";
		meshToBuild->textureNames[3] = "dfk_wall_a_SmMetAO.bmp";
		meshToBuild->textureRatios[1] = 1.0f;
		meshToBuild->textureRatios[2] = 1.0f;
		meshToBuild->textureRatios[3] = 0.3f;
		meshToBuild->textureOperator = 4;
	}
	else if (type == TypeOfMesh::LIGHT_FIXTURE)
	{
		meshToBuild = new cMesh();
		meshToBuild->meshName = "dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply";
		meshToBuild->positionXYZ = glm::vec3(0.0f, 2.4f, -0.1f);
		meshToBuild->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
		meshToBuild->bDontLight = dLight;
		meshToBuild->clearTextureRatiosToZero();
		meshToBuild->textureNames[1] = "dfk_candleholders_01_basecolor.bmp";
		meshToBuild->textureNames[2] = "dfk_candleholders_01_normal.bmp";
		meshToBuild->textureNames[3] = "dfk_candleholders_01_SmMetAO.bmp";
		meshToBuild->textureRatios[1] = 1.0f;
		meshToBuild->textureRatios[2] = 1.0f;
		meshToBuild->textureRatios[3] = 0.3f;
		meshToBuild->textureOperator = 4;

		cMesh* torch1 = new cMesh();
		torch1->meshName = "dfk_torch_XYZ_N_RGBA_UV_transformed.ply";
		torch1->positionXYZ = glm::vec3(0.0f);
		torch1->orientationXYZ = glm::vec3(0.0f);
		torch1->bDontLight = dLight;
		torch1->clearTextureRatiosToZero();
		torch1->textureNames[1] = "dfk_candleholders_01_basecolor.bmp";
		torch1->textureNames[2] = "dfk_candleholders_01_normal.bmp";
		torch1->textureNames[3] = "dfk_candleholders_01_SmMetAO.bmp";
		torch1->textureRatios[1] = 1.0f;
		torch1->textureRatios[2] = 1.0f;
		torch1->textureRatios[3] = 0.3f;
		torch1->textureOperator = 4;

		cMesh* flame1 = new cMesh();
		flame1->meshName = "Engine_Exhaust_Imposter.ply";
		flame1->clearTextureRatiosToZero();
		flame1->textureNames[1] = "Long_blue_Jet_Flame.bmp";
		flame1->textureRatios[1] = 1.0f;
		flame1->friendlyName = "Torch Light";
		flame1->orientationXYZ = glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(-22.5f));
		flame1->positionXYZ = glm::vec3(0.35f, 0.575f, 0.0f);
		flame1->bIsImposter = true;
		flame1->scale = glm::vec3(0.25f, 0.25f, 0.5f);

		// flames need to be drawn before everything else... ughhh lol
		torch1->vec_pChildMeshes.push_back(flame1);
		meshToBuild->vec_pChildMeshes.push_back(torch1);

	}

	return meshToBuild;
}