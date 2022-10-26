#include "sAttackState.h"
#include "iEntity.h"
#include "globalThings.h"

sAttackState::sAttackState()
{
	playerNode = nullptr;
	timeToNextAction = 0.0f;
	dirToRotate = 0;
}
sAttackState::~sAttackState()
{

}

std::string sAttackState::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	timeToNextAction += dt * ::entitySpeedModifier;
	if (timeToNextAction > 1.0f)
	{
		timeToNextAction = 0.0f;
		if (glm::distance(currentNode->position + lookDirection, playerNode->position) <= 0.75f)
		{
			return "Attack";
		}
		else
		{
			if (dirToRotate == 0)
			{
				// Determine the direction to rotate once
				int dirIWant = -1;
				if (glm::distance(currentNode->position + vec3NORTH, playerNode->position) <= 0.75f)
					dirIWant = 0;
				else if (glm::distance(currentNode->position + vec3NORTH_EAST, playerNode->position) <= 0.75f)
					dirIWant = 1;
				else if (glm::distance(currentNode->position + vec3EAST, playerNode->position) <= 0.75f)
					dirIWant = 2;
				else if (glm::distance(currentNode->position + vec3SOUTH_EAST, playerNode->position) <= 0.75f)
					dirIWant = 3;
				else if (glm::distance(currentNode->position + vec3SOUTH, playerNode->position) <= 0.75f)
					dirIWant = 4;
				else if (glm::distance(currentNode->position + vec3SOUTH_WEST, playerNode->position) <= 0.75f)
					dirIWant = 5;
				else if (glm::distance(currentNode->position + vec3WEST, playerNode->position) <= 0.75f)
					dirIWant = 6;
				else if (glm::distance(currentNode->position + vec3NORTH_WEST, playerNode->position) <= 0.75f)
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
		}
	}	// end of if timeToNextAction > 1.0f

	if (playerNode != nullptr)
	{
		if (!playerNode->isOccupied || playerNode->occupiedBy != 0)
		{
			this->mCurrentCondition = 1;
		}
	}
	else
	{
		this->mCurrentCondition = 1;
	}

	return "nothing";
}
void sAttackState::OnEnterState(Node* currentNode)
{
	for (unsigned int index = 0; index != currentNode->edges.size(); index++)
	{
		if (currentNode->edges[index].first->isOccupied && currentNode->edges[index].first->occupiedBy == 0)
		{
			playerNode = currentNode->edges[index].first;
			break;
		}
	}
	dirToRotate = 0;
	timeToNextAction = 0.0f;
	this->mCurrentCondition = 0;

	//printf("Attck Enter");
}
void sAttackState::OnExitState()
{
	playerNode = nullptr;
	timeToNextAction = 0.0f;
	dirToRotate = 0;
	mCurrentCondition = 0;

	//printf("Attack Exit");
}