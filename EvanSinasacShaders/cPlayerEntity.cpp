#include "cPlayerEntity.h"
#include <iostream>

#include "sIdleState.h"
#include "sExitMazeState.h"

#include "globalThings.h"

cPlayerEntity::cPlayerEntity()
{
	this->position = glm::vec3(0.0f);
	this->lookAt = vec3NORTH;
	this->direction = (int)DIRECTIONS::NORTH;

	this->m_CurrNode = NULL;

	this->m_Mesh = new cMesh();
	//this->m_Mesh->meshName = "dalek2005_XYZ_N_RGBA_UV_hi_res.ply";
	this->m_Mesh->meshName = "hollowknight_XYZ_N_RGBA_UV.ply";
	this->m_Mesh->positionXYZ = this->position;
	this->m_Mesh->orientationXYZ = glm::vec3(0.0f);
	//this->m_Mesh->setUniformScale(0.03f);
	this->m_Mesh->setUniformScale(0.4f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->friendlyName = "Player";
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[1] = "uv_hollow.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
	this->m_Mesh->bUseStencil = true;

	type = ENTITY_TYPE::PLAYER;

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = this->position;
	this->m_LowResMesh->setUniformScale(0.5f);
	this->m_LowResMesh->bDontLight = true;
	this->m_LowResMesh->friendlyName = "Player";
	this->m_LowResMesh->clearTextureRatiosToZero();
	this->m_LowResMesh->textureNames[1] = "uv_hollow.bmp";
	this->m_LowResMesh->textureRatios[1] = 1.0f;
	this->m_LowResMesh->bUseStencil = true;


	this->m_FSMSystem = new sFSMSystem();
	this->m_IdleState = new sIdleState();
	this->m_SearchState = new sExitMazeState();

	// setup transitions
	this->m_IdleState->AddTransition(1, this->m_SearchState);
	
	this->m_FSMSystem->AddState(this->m_IdleState);
	this->m_FSMSystem->AddState(this->m_SearchState);

	cheating = false;

	soundIndex = 0;
	nextSoundIndex = 1;
}
cPlayerEntity::cPlayerEntity(glm::vec3 startPos, glm::vec3 startLookAt, Node* startNode)
	: position(startPos)
	, lookAt(startLookAt)
	, m_CurrNode(startNode)
{
	this->m_Mesh = new cMesh();
	//this->m_Mesh->meshName = "dalek2005_XYZ_N_RGBA_UV_hi_res.ply";
	this->m_Mesh->meshName = "hollowknight_XYZ_N_RGBA_UV.ply";
	this->m_Mesh->positionXYZ = this->position;
	// orientation in lookAt
	//this->m_Mesh->setUniformScale(0.03f);
	this->m_Mesh->setUniformScale(0.4f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->friendlyName = "Player";
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[1] = "uv_hollow.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
	this->m_Mesh->bUseWholeObjectDiffuseColour = false;
	this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(0.6f, 0.2f, 0.6f, 1.0f);
	this->m_Mesh->bUseStencil = true;


	if (lookAt == vec3NORTH)
	{
		this->direction = (int)DIRECTIONS::NORTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (lookAt == vec3NORTH_EAST)
	{
		this->direction = (int)DIRECTIONS::NORTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(45.0f), 0.0f);
	}
	else if (lookAt == vec3EAST)
	{
		this->direction = (int)DIRECTIONS::EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
	}
	else if (lookAt == vec3SOUTH_EAST)
	{
		this->direction = (int)DIRECTIONS::SOUTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(135.0f), 0.0f);
	}
	else if (lookAt == vec3SOUTH)
	{
		this->direction = (int)DIRECTIONS::SOUTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);
	}
	else if (lookAt == vec3SOUTH_WEST)
	{
		this->direction = (int)DIRECTIONS::SOUTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(225.0f), 0.0f);
	}
	else if (lookAt == vec3WEST)
	{
		this->direction = (int)DIRECTIONS::WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
	}
	else if (lookAt == vec3NORTH_WEST)
	{
		this->direction = (int)DIRECTIONS::NORTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(315.0f), 0.0f);
	}
	else 
	{
		this->direction = (int)DIRECTIONS::NORTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(45.0f), 0.0f);
	}

	
	type = ENTITY_TYPE::PLAYER;


	this->m_CurrNode->isOccupied = true;
	this->m_CurrNode->occupiedBy = (int)this->type;

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = this->position;
	this->m_LowResMesh->setUniformScale(0.5f);
	this->m_LowResMesh->bDontLight = true;
	this->m_LowResMesh->friendlyName = "Player";
	this->m_LowResMesh->clearTextureRatiosToZero();
	this->m_LowResMesh->textureNames[1] = "uv_hollow.bmp";
	this->m_LowResMesh->textureRatios[1] = 1.0f;
	this->m_LowResMesh->bUseStencil = true;


	this->m_FSMSystem = new sFSMSystem();
	this->m_IdleState = new sIdleState();
	this->m_SearchState = new sExitMazeState();

	// setup transitions
	this->m_IdleState->AddTransition(1, this->m_SearchState);

	this->m_FSMSystem->AddState(this->m_IdleState);
	this->m_FSMSystem->AddState(this->m_SearchState);

	cheating = false;

	soundIndex = 0;
	nextSoundIndex = 1;
}
cPlayerEntity::~cPlayerEntity()
{

}

void cPlayerEntity::Update(float dt)
{
	if (cheating)
	{
		std::string stateMessage = this->m_FSMSystem->Update(dt, m_CurrNode, lookAt, direction);
		//std::cout << stateMessage << std::endl;
		if (stateMessage == "Rotate1")
		{
			Rotate("LEFT");
		}
		else if (stateMessage == "Rotate-1")
		{
			Rotate("RIGHT");
		}
		else if (stateMessage == "Move")
		{
			Move("FORWARD");
		}
		else if (stateMessage == "EndCheating")
		{
			cheating = false;
			this->m_FSMSystem->Reset();
		}
	}


	this->m_Mesh->positionXYZ = this->position;
	this->m_LowResMesh->positionXYZ = glm::vec3(this->position.x, 0.5f, this->position.z);
	
}

void cPlayerEntity::Move(std::string directionToMove)
{
	if (directionToMove == "FORWARD")
	{
		for (pair<Node*, float>& neighbour : this->m_CurrNode->edges)
		{
			glm::vec3 pos = glm::vec3(neighbour.first->position.x, neighbour.first->position.y, neighbour.first->position.z);
			if (glm::distance(pos, this->position + this->lookAt) <= 0.75f)
			{
				if (neighbour.first->type != "-" && !neighbour.first->isOccupied)
				{
					this->position = glm::vec3(pos.x, this->position.y, pos.z);
					this->m_CurrNode->isOccupied = false;
					this->m_CurrNode->occupiedBy = -1;
					this->m_CurrNode = neighbour.first;
					this->m_CurrNode->isOccupied = true;
					this->m_CurrNode->occupiedBy = (int)this->type;

					// If we actually move then we play the footstep sound
					PlayFootstepSound();
				}
				break;
			}
		}
	}
	else if (directionToMove == "BACKWARDS")
	{
		for (pair<Node*, float>& neighbour : this->m_CurrNode->edges)
		{
			glm::vec3 pos = glm::vec3(neighbour.first->position.x, neighbour.first->position.y, neighbour.first->position.z);
			if (glm::distance(pos, this->position - this->lookAt) <= 0.75f)
			{
				if (neighbour.first->type != "-" && !neighbour.first->isOccupied)
				{
					this->position = glm::vec3(pos.x, this->position.y, pos.z);
					this->m_CurrNode->isOccupied = false;
					this->m_CurrNode->occupiedBy = -1;
					this->m_CurrNode = neighbour.first;
					this->m_CurrNode->isOccupied = true;
					this->m_CurrNode->occupiedBy = (int)this->type;

					// If we actually move then we play the footstep sound
					PlayFootstepSound();
				}
				break;
			}
		}
	}
	else
	{
		std::cout << "directionToMove is wrong!" << std::endl;
	}
	
}
void cPlayerEntity::Rotate(std::string directionToRotate)
{
	if (directionToRotate == "LEFT")
	{
		direction++;
		if (direction > (int)DIRECTIONS::NORTH_WEST)
		{
			direction = (int)DIRECTIONS::NORTH;
		}
	}
	else if (directionToRotate == "RIGHT")
	{
		direction--;
		if (direction < (int)DIRECTIONS::NORTH)
		{
			direction = (int)DIRECTIONS::NORTH_WEST;
		}
	}
	else
	{
		std::cout << "NOT A VALID DIRECTION" << std::endl;
	}

	switch (direction)
	{
	case (int)DIRECTIONS::NORTH:
		this->lookAt = vec3NORTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case(int)DIRECTIONS::NORTH_EAST:
		this->lookAt = vec3NORTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-45.0f), 0.0f);
		break;
	case(int)DIRECTIONS::EAST:
		this->lookAt = vec3EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-90.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH_EAST:
		this->lookAt = vec3SOUTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-135.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH:
		this->lookAt = vec3SOUTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-180.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH_WEST:
		this->lookAt = vec3SOUTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-225.0f), 0.0f);
		break;
	case(int)DIRECTIONS::WEST:
		this->lookAt = vec3WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-270.0f), 0.0f);
		break;
	case(int)DIRECTIONS::NORTH_WEST:
		this->lookAt = vec3NORTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-315.0f), 0.0f);
		break;
	default:
		std::cout << "Direction is somehow invalid (the switch statement didn't work)" << std::endl;
		break;
	}
}

void cPlayerEntity::StartCheating()
{
	cheating = true;
	this->m_FSMSystem->Start();
}

void cPlayerEntity::PlayFootstepSound()
{
	_result = g_pFMODSystem->playSound(g_vecSounds.at(soundIndex), 0, true, &g_vecChannels.at(soundIndex));
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to play sound[%d]\n", soundIndex);
		return;
	}

	FMOD_VECTOR _position = { this->position.x, this->position.y, this->position.z };
	FMOD_VECTOR _velocity = { 0.0f, 0.0f, 0.0f };
	_result = g_vecChannels.at(soundIndex)->set3DAttributes(&_position, &_velocity);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set 3D settings for channel[%d]\n", soundIndex);
		return;
	}
	// Unpause the sound now
	_result = g_vecChannels.at(soundIndex)->setPaused(false);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to unpause channel[%d]\n", soundIndex);
		return;
	}

	soundIndex += nextSoundIndex;
	if (soundIndex >= 2)
	{
		soundIndex = 2;
		nextSoundIndex = -1;
	}
	else if (soundIndex <= 0)
	{
		soundIndex = 0;
		nextSoundIndex = 1;
	}

	return;
}