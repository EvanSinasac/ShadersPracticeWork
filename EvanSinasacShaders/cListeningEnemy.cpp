#include "cListeningEnemy.h"

#include "sIdleState.h"
#include "sEnclosedWanderState.h"
#include "sApproachState.h"
#include "sAttackState.h"

#include <iostream>

cListeningEnemy::cListeningEnemy()
{
	this->position = glm::vec3(0.0f);
	this->lookAt = vec3NORTH;
	this->direction = (int)DIRECTIONS::NORTH;

	this->m_CurrNode = NULL;

	this->m_Mesh = new cMesh();
	//this->m_Mesh->meshName = "dalek2005_XYZ_N_RGBA_UV_hi_res.ply";
	this->m_Mesh->meshName = "Pokemon.ply";
	this->m_Mesh->positionXYZ = this->position;
	this->m_Mesh->orientationXYZ = glm::vec3(0.0f);
	this->m_Mesh->setUniformScale(0.01f);
	//this->m_Mesh->setUniformScale(0.4f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->friendlyName = "Listening Enemy";
	this->m_Mesh->textureOperator = 4;
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[1] = "Final_Pokemon_Diffuse.bmp";
	this->m_Mesh->textureNames[2] = "Final_Pokemon_Normal.bmp";
	this->m_Mesh->textureNames[2] = "Final_Pokemon_Ambient_Occlusion.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
	this->m_Mesh->textureRatios[2] = 1.0f;
	this->m_Mesh->textureRatios[3] = 0.3f;
	this->m_Mesh->bUseStencil = true;
	//this->m_Mesh->bUseWholeObjectDiffuseColour = true;
	//this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(rand() % 255 / 255.0f, 0.5f, rand() % 255 / 255.0f, 1.0f);		// guarenteed at least some green colour

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = this->position;
	this->m_LowResMesh->setUniformScale(0.5f);
	this->m_LowResMesh->bDontLight = true;
	this->m_LowResMesh->friendlyName = "Listening Enemy";
	this->m_LowResMesh->clearTextureRatiosToZero();
	this->m_LowResMesh->textureNames[1] = "Final_Pokemon_Diffuse.bmp";
	this->m_LowResMesh->textureRatios[1] = 1.0f;
	this->m_LowResMesh->bUseStencil = true; 

	type = ENTITY_TYPE::ENEMY_LISTEN;


	this->m_FSMSystem = new sFSMSystem();
	this->m_IdleState = new sIdleState();
	this->m_WanderState = new sEnclosedWanderState();
	this->m_ApproachState = new sApproachState();
	this->m_AttackState = new sAttackState();

	// Setup transitions
	this->m_IdleState->AddTransition(1, this->m_WanderState);

	this->m_WanderState->AddTransition(1, this->m_ApproachState);

	this->m_ApproachState->AddTransition(1, this->m_WanderState);
	this->m_ApproachState->AddTransition(2, this->m_AttackState);

	this->m_AttackState->AddTransition(1, this->m_ApproachState);

	this->m_FSMSystem->AddState(this->m_IdleState);
	this->m_FSMSystem->AddState(this->m_WanderState);
	this->m_FSMSystem->AddState(this->m_ApproachState);
	this->m_FSMSystem->AddState(this->m_AttackState);
	this->m_FSMSystem->Start();
}
cListeningEnemy::cListeningEnemy(glm::vec3 startPos, glm::vec3 startLookAt, Node* startNode)
	: position(startPos)
	, lookAt(startLookAt)
	, m_CurrNode(startNode)
{
	this->m_Mesh = new cMesh();
	//this->m_Mesh->meshName = "dalek2005_XYZ_N_RGBA_UV_hi_res.ply";
	this->m_Mesh->meshName = "Pokemon.ply";
	this->m_Mesh->positionXYZ = this->position;
	this->m_Mesh->orientationXYZ = glm::vec3(0.0f);
	this->m_Mesh->setUniformScale(0.01f);
	//this->m_Mesh->setUniformScale(0.4f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->friendlyName = "Listening Enemy";
	this->m_Mesh->textureOperator = 4;
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[1] = "Final_Pokemon_Diffuse.bmp";
	this->m_Mesh->textureNames[2] = "Final_Pokemon_Normal.bmp";
	this->m_Mesh->textureNames[2] = "Final_Pokemon_Ambient_Occlusion.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
	this->m_Mesh->textureRatios[2] = 1.0f;
	this->m_Mesh->textureRatios[3] = 0.3f;
	this->m_Mesh->bUseStencil = true;
	//this->m_Mesh->bUseWholeObjectDiffuseColour = true;
	//this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(rand() % 255 / 255.0f, 0.5f, rand() % 255 / 255.0f, 1.0f);		// guarenteed at least some green colour
	
	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = this->position;
	this->m_LowResMesh->setUniformScale(0.5f);
	this->m_LowResMesh->bDontLight = true;
	this->m_LowResMesh->friendlyName = "Listening Enemy";
	this->m_LowResMesh->clearTextureRatiosToZero();
	this->m_LowResMesh->textureNames[1] = "Final_Pokemon_Diffuse.bmp";
	this->m_LowResMesh->textureRatios[1] = 1.0f;
	this->m_LowResMesh->bUseStencil = true;

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


	type = ENTITY_TYPE::ENEMY_LISTEN;
	this->m_CurrNode->isOccupied = true;
	this->m_CurrNode->occupiedBy = (int)this->type;

	this->m_FSMSystem = new sFSMSystem();
	this->m_IdleState = new sIdleState();
	this->m_WanderState = new sEnclosedWanderState();
	this->m_ApproachState = new sApproachState();
	this->m_AttackState = new sAttackState();

	// Setup transitions
	this->m_IdleState->AddTransition(1, this->m_WanderState);

	this->m_WanderState->AddTransition(1, this->m_ApproachState);

	this->m_ApproachState->AddTransition(1, this->m_WanderState);
	this->m_ApproachState->AddTransition(2, this->m_AttackState);

	this->m_AttackState->AddTransition(1, this->m_ApproachState);

	this->m_FSMSystem->AddState(this->m_IdleState);
	this->m_FSMSystem->AddState(this->m_WanderState);
	this->m_FSMSystem->AddState(this->m_ApproachState);
	this->m_FSMSystem->AddState(this->m_AttackState);
	this->m_FSMSystem->Start();
}
cListeningEnemy::~cListeningEnemy()
{

}

void cListeningEnemy::Update(float dt)
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
	else if (stateMessage == "Attack")
	{
		std::cout << "Ha HA!  I'm attacking the player!" << std::endl;
	}

	this->m_Mesh->positionXYZ = glm::vec3(this->position.x, 0.0f, this->position.z);
	this->m_LowResMesh->positionXYZ = glm::vec3(this->m_Mesh->positionXYZ.x, 0.5f, this->m_Mesh->positionXYZ.z);
}

void cListeningEnemy::Move(std::string directionToMove)
{
	if (directionToMove == "FORWARD")
	{
		for (pair<Node*, float>& neighbour : this->m_CurrNode->edges)
		{
			glm::vec3 pos = glm::vec3(neighbour.first->position.x, neighbour.first->position.y, neighbour.first->position.z);
			if (glm::distance(pos, this->position + this->lookAt) <= 0.75f)
			{
				if (neighbour.first->type != "-" && !neighbour.first->isOccupied && 
					!(neighbour.first->type == "D" || neighbour.first->type == "DS" || neighbour.first->type == "DP"))
				{
					this->position = glm::vec3(pos.x, this->position.y, pos.z);
					this->m_CurrNode->isOccupied = false;
					this->m_CurrNode->occupiedBy = -1;

					this->m_CurrNode->m_Footprints->Activate();
					this->m_CurrNode->m_Footprints->SetTextures(0);
					switch (direction)
					{
					case (int)DIRECTIONS::NORTH:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-45.0f), 0.0f));
						break;
					case(int)DIRECTIONS::NORTH_EAST:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-90.0f), 0.0f));
						break;
					case(int)DIRECTIONS::EAST:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-135.0f), 0.0f));
						break;
					case(int)DIRECTIONS::SOUTH_EAST:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-180.0f), 0.0f));
						break;
					case(int)DIRECTIONS::SOUTH:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-225.0f), 0.0f));
						break;
					case(int)DIRECTIONS::SOUTH_WEST:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-270.0f), 0.0f));
						break;
					case(int)DIRECTIONS::WEST:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), glm::radians(-315.0f), 0.0f));
						break;
					case(int)DIRECTIONS::NORTH_WEST:
						this->m_CurrNode->m_Footprints->SetOrientation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
						break;
					default:
						//std::cout << "Direction is somehow invalid (the switch statement didn't work)" << std::endl;
						break;
					}

					this->m_CurrNode = neighbour.first;
					this->m_CurrNode->isOccupied = true;
					this->m_CurrNode->occupiedBy = (int)this->type;
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
				}
				break;
			}
		}
	}
	else
	{
		//std::cout << "directionToMove is wrong!" << std::endl;
	}
}
void cListeningEnemy::Rotate(std::string directionToRotate)
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
		//std::cout << "NOT A VALID DIRECTION" << std::endl;
	}

	switch (direction)
	{
	case (int)DIRECTIONS::NORTH:
		this->lookAt = vec3NORTH;
		// sigh, the venusaur model is opposite of everything else lol
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-180.0f), 0.0f);
		break;
	case(int)DIRECTIONS::NORTH_EAST:
		this->lookAt = vec3NORTH_EAST;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-45.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-225.0f), 0.0f);
		break;
	case(int)DIRECTIONS::EAST:
		this->lookAt = vec3EAST;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-90.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-270.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH_EAST:
		this->lookAt = vec3SOUTH_EAST;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-135.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-315.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH:
		this->lookAt = vec3SOUTH;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-180.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH_WEST:
		this->lookAt = vec3SOUTH_WEST;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-225.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-45.0f), 0.0f);
		break;
	case(int)DIRECTIONS::WEST:
		this->lookAt = vec3WEST;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-270.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-90.0f), 0.0f);
		break;
	case(int)DIRECTIONS::NORTH_WEST:
		this->lookAt = vec3NORTH_WEST;
		//this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-315.0f), 0.0f);
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-135.0f), 0.0f);
		break;
	default:
		//std::cout << "Direction is somehow invalid (the switch statement didn't work)" << std::endl;
		break;
	}
}