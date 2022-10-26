#include "sExitMazeState.h"
#include "globalThings.h"
#include <iostream>

Node* AStar(Graph* graph, Node* rootNode, Node* goal);

sExitMazeState::sExitMazeState()
{
	timeToNextAction = 0.0f;
	dirToRotate = 0;
	this->mCurrentCondition = 0;
}
sExitMazeState::~sExitMazeState()
{

}

std::string sExitMazeState::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	timeToNextAction += dt * ::entitySpeedModifier;
	if (timeToNextAction >= 0.5f)
	{
		if (pathToTarget.size() > 0)
		{
			if (pathToTarget[0]->isOccupied)
			{
				// next Node is occupied, we need to reset the path
				OnExitState();
				OnEnterState(currentNode);
				if (pathToTarget.size() == 0)
				{
					this->mCurrentCondition = 1;	// either there's no possible path OR we've reached the end somehow
					return "EndCheating";
				}
				return "PathReCalculated";
			}
			else	// so we have a path and the next Node in the path isn't occupied
			{
				timeToNextAction = 0.0f;
				if (glm::distance(currentNode->position + lookDirection, pathToTarget[0]->position) <= 0.75f)
				{	// facing the right direction, move to next node
					dirToRotate = 0;
					pathToTarget.erase(pathToTarget.begin());
					return "Move";
				}
				else
				{
					// so rotate to face it and then move in that direction
					// we're not facing the target, so let's use the same logic from the wander stuff to rotate to face the correct direction
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
				}
			}
		}
		else
		{
			// path is empty, so we should be at the exit
			this->mCurrentCondition = 1;
			return "EndCheating";
		}		
	}
	else
	{
		return "nothing";
	}
}

void sExitMazeState::OnEnterState(Node* currentNode)
{
	std::cout << "Player is Cheating!  Calculating Best Path to Exit using AStar!" << std::endl;
	::g_Graph->ResetGraph();
	Node* targetNode = AStar(::g_Graph, currentNode, ::g_exitNode);
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
	}

	dirToRotate = 0;
	timeToNextAction = 0.0f;
	this->mCurrentCondition = 0;
}
void sExitMazeState::OnExitState()
{
	for (unsigned int index = 0; index != pathToTarget.size(); index)
	{
		pathToTarget.erase(pathToTarget.begin());	// empty the list
	}

	dirToRotate = 0;
	timeToNextAction = 0.0f;
	mCurrentCondition = 0;
}