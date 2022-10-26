#include "sApproachState.h"
#include "globalThings.h"
#include <iostream>

Node* BFS_LookForPlayerWithinEnclosedArea(Graph* graph, Node* rootNode);

sApproachState::sApproachState()
	: sFSMState("Approach State")
{
	timeToNextAction = 0.0f;
	dirToRotate = 0;
	this->mCurrentCondition = 0;
}
sApproachState::~sApproachState()
{

}

std::string sApproachState::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	timeToNextAction += dt * ::entitySpeedModifier;
	if (timeToNextAction > 1.0f)
	{
		if (pathToTarget.size() == 1)
		{
			if (pathToTarget[0]->isOccupied && pathToTarget[0]->occupiedBy == 0)
			{
				// we've reached the target and it's occupied by the player.  Turn towards them and then enter attack
				if (glm::distance(currentNode->position + lookDirection, pathToTarget[0]->position) <= 0.75f)
				{
					mCurrentCondition = 2;
					return "EnterAttack";
				}
				else
				{
					// we're not facing the target, so let's use the same logic from the wander stuff to rotate to face the correct direction
					timeToNextAction = 0.0f;
					if (dirToRotate == 0)
					{
						// Determine the direction to rotate once
						int dirIWant = -1;
						if (glm::distance(currentNode->position + vec3NORTH, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 0;
						else if (glm::distance(currentNode->position + vec3NORTH_EAST, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 1;
						else if (glm::distance(currentNode->position + vec3EAST, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 2;
						else if (glm::distance(currentNode->position + vec3SOUTH_EAST, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 3;
						else if (glm::distance(currentNode->position + vec3SOUTH, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 4;
						else if (glm::distance(currentNode->position + vec3SOUTH_WEST, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 5;
						else if (glm::distance(currentNode->position + vec3WEST, pathToTarget[0]->position) <= 0.75f)
							dirIWant = 6;
						else if (glm::distance(currentNode->position + vec3NORTH_WEST, pathToTarget[0]->position) <= 0.75f)
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
				} //end of enter attack or rotate if
			} //end of if next node is target
			else
			{	// we've reached the target node but it's either no longer occupied or it's not occupied by the player
				OnExitState();	// clear the path
				OnEnterState(currentNode);	// fill the path
				if (pathToTarget.size() == 0)
				{	// player is no longer within range, return to wander
					this->mCurrentCondition = 1;
					return "ReturnToWander";
				}
				return "PathReCalculated";
			}
		} //end of if pathToTarget is size 1
		else if (pathToTarget.size() == 0)
		{
			this->mCurrentCondition = 1;
			return "ReturnToWander";
		}
		else
		{
			// We need to approach the node the player was heard in
			timeToNextAction = 0.0f;
			if (pathToTarget[0]->isOccupied)
			{
				OnExitState();	// clear the path
				OnEnterState(currentNode);	// fill the path
				if (pathToTarget.size() == 0)
				{	// player is no longer within range, return to wander
					this->mCurrentCondition = 1;
					return "ReturnToWander";
				}
				//std::cout << "This was the problem" << std::endl;
				return "PathReCalculated";
			}

			if (glm::distance(currentNode->position + lookDirection, pathToTarget[0]->position) <= 0.75f)
			{	// facing the right direction, move to next node
				dirToRotate = 0;
				pathToTarget.erase(pathToTarget.begin());
				return "Move";
			}
			if (dirToRotate == 0)
			{
				// Determine the direction to rotate once
				int dirIWant = -1;
				if (glm::distance(currentNode->position + vec3NORTH, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 0;
				else if (glm::distance(currentNode->position + vec3NORTH_EAST, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 1;
				else if (glm::distance(currentNode->position + vec3EAST, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 2;
				else if (glm::distance(currentNode->position + vec3SOUTH_EAST, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 3;
				else if (glm::distance(currentNode->position + vec3SOUTH, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 4;
				else if (glm::distance(currentNode->position + vec3SOUTH_WEST, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 5;
				else if (glm::distance(currentNode->position + vec3WEST, pathToTarget[0]->position) <= 0.75f)
					dirIWant = 6;
				else if (glm::distance(currentNode->position + vec3NORTH_WEST, pathToTarget[0]->position) <= 0.75f)
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
			//otherwise, rotate until facing the correct direction
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
	} //end of if timeToNextAction >= 1.0f
	else
	{
		return "nothing";
	}

	return "nothing";
}
void sApproachState::OnEnterState(Node* currentNode)
{
	::g_Graph->ResetGraph();
	Node* targetNode = BFS_LookForPlayerWithinEnclosedArea(::g_Graph, currentNode);
	if (targetNode != NULL)
	{
		// path from goal to root
		do
		{
			//std::cout << targetNode->id << " <- ";
			this->pathToTarget.push_back(targetNode);
			targetNode = targetNode->parent;
		} while (targetNode->parent != NULL);

		//std::cout << targetNode->id << endl;
		//pathToTarget.push_back(targetNode);

		// path is in reverse order
		unsigned int i = 0, j = pathToTarget.size() - 1;
		for (i = 0; i < j; i++, j--)
		{
			Node* temp = pathToTarget[i];
			pathToTarget[i] = pathToTarget[j];
			pathToTarget[j] = temp;
		}
		//std::cout << "Path To Target: ";
		//for (unsigned int index = 0; index != pathToTarget.size(); index++)
		//{
		//	std::cout << pathToTarget[index]->id;
		//}
	}

	//std::cout << "Approach State Entered!" << std::endl;

	dirToRotate = 0;
	timeToNextAction = 0.0f;
	this->mCurrentCondition = 0;

	//printf("Approach Enter");
}
void sApproachState::OnExitState()
{
	for (unsigned int index = 0; index != pathToTarget.size(); index)
	{
		pathToTarget.erase(pathToTarget.begin());	// empty the list
	}

	dirToRotate = 0;
	timeToNextAction = 0.0f;
	mCurrentCondition = 0;
	//std::cout << "Approach State Exited!" << std::endl;
	//printf("Approach Exit");
}