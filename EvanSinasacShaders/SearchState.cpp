#include "SearchState.h"
#include "globalThings.h"
#include <iostream>

Node* Dijkstra(Graph* graph, Node* rootNode);

SearchState::SearchState(void)
	: FSMState("Search State") {
}

SearchState::~SearchState(void) {
}

glm::vec3 SearchState::Update(float dt, glm::vec3 curPos, int& stateID) 
{
	stateID = 1;
	if (gNumResources <= 0) {
		printf("SearchState: No resources found!\n");
		mCurrentCondition = 2;
	}
	else {
		//printf("SearchState: Found %d resources!\n", gNumResources);
		
		if (pathToTarget.size() > 0)
		{
			glm::vec3 targetPos = glm::vec3(pathToTarget[0]->position.x, pathToTarget[0]->position.y, 0.0f);
			glm::vec3 direction = targetPos - curPos;
			//direction = glm::normalize(direction) * dt * ::entitySpeedModifier;
			direction = glm::normalize(direction) * (0.05f * ::entitySpeedModifier);

			curPos += direction;
			//printf("%f %f %f\n", curPos.x, curPos.y, curPos.z);
			if (glm::distance(targetPos, curPos) < 0.1f)
			{
				pathToTarget.erase(pathToTarget.begin());
			}
		}
		else
		{
			// If the resource is gone by the time we get here, re-make the pathToTarget and continue searching
			printf("SearchState: Arrived at resource!  Checking if the resource is here...\n");

			Node* currentNode = nullptr;
			for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
			{
				if (glm::distance(curPos, glm::vec3(::g_Graph->nodes[index]->position.x, ::g_Graph->nodes[index]->position.y, 0.0f)) < 0.1f)
				{
					currentNode = ::g_Graph->nodes[index];
					break;
				}
			}

			if (currentNode != nullptr)
			{
				if (currentNode->hasGoal)
				{
					printf("SearchState: Resource still here!  Commencing collection...\n");
					mCurrentCondition = 1;
				}
				else
				{
					printf("SearchState: Resource not here, running OnEnter to find new resource...\n");
					OnEnterState(curPos);
				}
			}
			else
			{
				printf("SearchState: Could not find node in graph near current location!\n");
			}

		}

		//mCurrentCondition = 1;
	}

	return curPos;
}

void SearchState::OnEnterState(glm::vec3 curPos) 
{
	// Use Dijkstra's algorithm to find a path, then follow the path until the Entity reaches the end position
	printf("SearchState: Entered\n");

	Node* currentNode = nullptr;
	for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
	{
		if (glm::distance(curPos, glm::vec3(::g_Graph->nodes[index]->position.x, ::g_Graph->nodes[index]->position.y, 0.0f)) < 0.1f)
		{
			currentNode = ::g_Graph->nodes[index];
			break;
		}
	}
	if (currentNode != nullptr)
	{
		std::cout << "Current Node ID: " << currentNode->id << std::endl;
	}
	
	::g_Graph->ResetGraph();
	Node* targetNode = Dijkstra(::g_Graph, currentNode);

	if (targetNode != NULL)
	{
		// path from goal to root
		do
		{
			std::cout << targetNode->id << " <- ";
			this->pathToTarget.push_back(targetNode);
			targetNode = targetNode->parent;
		} while (targetNode->parent != NULL);

		std::cout << targetNode->id << endl;
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
	

	mCurrentCondition = 0;
}

void SearchState::OnExitState(void) 
{
	printf("SearchState: Exited\n");
}