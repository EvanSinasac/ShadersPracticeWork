#include "sEnclosedWanderState.h"
#include "iEntity.h"
#include "globalThings.h"

#include <iostream>

Node* BFS_LookForPlayerWithinEnclosedArea(Graph* graph, Node* rootNode);

sEnclosedWanderState::sEnclosedWanderState()
	: sFSMState("Enclosed Wander State")
{
	timeToNextAction = 0.0f;
	nextNode = nullptr;
	stage = 0;
	dirToRotate = 0;
}
sEnclosedWanderState::~sEnclosedWanderState()
{

}

std::string sEnclosedWanderState::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	Node* playerNode = BFS_LookForPlayerWithinEnclosedArea(::g_Graph, currentNode);
	if (playerNode != NULL)
	{
		this->mCurrentCondition = 1;
		return "EnteringApproach";
	}
	timeToNextAction += dt * ::entitySpeedModifier;
	if (timeToNextAction >= 1.0f)
	{
		if (stage == 0)						// choose direction to move to
		{
			//if (nextNode == nullptr || nextNode == currentNode)
			OnEnterState(currentNode);
			if (nextNode->type == "D" || nextNode->type == "DP" || nextNode->type == "DS")
			{
				stage = 0;
				//std::cout << "DOOR" << std::endl;
			}	
			else
			{
				stage = 1;
			}
			timeToNextAction = 0.0f;
			return "DirectionChosen";
		}
		else if (stage == 1)
		{
			timeToNextAction = 0.0f;
			if (glm::distance(currentNode->position + lookDirection, nextNode->position) <= 0.75f)
			{
				dirToRotate = 0;
				stage = 2;
				return "Rotate0";
			}
			if (dirToRotate == 0)
			{
				// Determine the direction to rotate once
				int dirIWant = -1;
				if (glm::distance(currentNode->position + vec3NORTH, nextNode->position) <= 0.75f)
					dirIWant = 0;
				else if (glm::distance(currentNode->position + vec3NORTH_EAST, nextNode->position) <= 0.75f)
					dirIWant = 1;
				else if (glm::distance(currentNode->position + vec3EAST, nextNode->position) <= 0.75f)
					dirIWant = 2;
				else if (glm::distance(currentNode->position + vec3SOUTH_EAST, nextNode->position) <= 0.75f)
					dirIWant = 3;
				else if (glm::distance(currentNode->position + vec3SOUTH, nextNode->position) <= 0.75f)
					dirIWant = 4;
				else if (glm::distance(currentNode->position + vec3SOUTH_WEST, nextNode->position) <= 0.75f)
					dirIWant = 5;
				else if (glm::distance(currentNode->position + vec3WEST, nextNode->position) <= 0.75f)
					dirIWant = 6;
				else if (glm::distance(currentNode->position + vec3NORTH_WEST, nextNode->position) <= 0.75f)
					dirIWant = 7;

				int currentDirection = direction;
				int numbOfRightRotations = 0;
				int numbOfLeftRotations = 0;
				do
				{
					currentDirection--;
					numbOfRightRotations++;
					if (currentDirection < 0)
						currentDirection = 7;
				} while (currentDirection != dirIWant);
				currentDirection = direction;
				do
				{
					currentDirection++;
					numbOfLeftRotations++;
					if (currentDirection > 7)
						currentDirection = 0;
				} while (currentDirection != dirIWant);

				if (numbOfRightRotations < numbOfLeftRotations)
					dirToRotate = -1;
				else
					dirToRotate = 1;
			}

			if (dirToRotate == 1)
			{
				return "Rotate1";
			}
			else if (dirToRotate == -1)
			{
				return "Rotate-1";
			}
			else
			{
				return "RotateWTF";
			}
		}
		else if (stage == 2)
		{
			stage = 0;
			timeToNextAction = 0.0f;
			return "Move";
		}
		else
		{
			return "nothing";
		}
	}
	else
	{
		return "nothing";
	}
}
void sEnclosedWanderState::OnEnterState(Node* currentNode)
{
	unsigned int index = rand() % currentNode->edges.size();
	nextNode = currentNode->edges[index].first;
	stage = 0;

	//std::cout << "Enclosed Wander State Entered" << std::endl;
	this->mCurrentCondition = 0;
	//printf("Enclosed Wander Enter");
}
void sEnclosedWanderState::OnExitState()
{
	stage = 0;
	dirToRotate = 0;
	nextNode = nullptr;
	mCurrentCondition = 0;
	//std::cout << "Enclosed Wander State Entered" << std::endl;
	//printf("Enclosed Wander Exit");
}