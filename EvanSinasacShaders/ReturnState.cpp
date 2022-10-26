#include "ReturnState.h"
#include "globalThings.h"

Node* AStar(Graph* graph, Node* rootNode, Node* goal);

ReturnState::ReturnState()
	: FSMState("Return State")
{
	this->m_Timer = 0.0f;
}

ReturnState::~ReturnState()
{

}

glm::vec3 ReturnState::Update(float dt, glm::vec3 curPos, int& stateID)
{
	stateID = 3;
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
		this->m_Timer += dt;
		if (this->m_Timer >= 2.0f)
		{	// waits 2 seconds before returning the resource to the base (simulates offloading the resource from the unit)
			printf("ReturnState: Returned a resource!\n");
			mCurrentCondition = 1;
		}
	}
	
	return curPos;
}

void ReturnState::OnEnterState(glm::vec3 curPos)
{
	// Use A* to find the path back to the home base
	printf("ReturnState: Entered\n");

	Node* currentNode = nullptr;
	for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
	{
		if (glm::distance(curPos, glm::vec3(::g_Graph->nodes[index]->position.x, ::g_Graph->nodes[index]->position.y, 0.0f)) < 0.1f)
		{
			currentNode = ::g_Graph->nodes[index];
			break;
		}
	}

	Node* homeBase = nullptr;
	for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
	{
		if (::g_Graph->nodes[index]->isHomeBase)
		{
			homeBase = ::g_Graph->nodes[index];
			break;
		}
	}
	//if (currentNode != nullptr)
	//{
	//	std::cout << "Current Node ID: " << currentNode->id << std::endl;
	//}

	::g_Graph->ResetGraph();
	Node* targetNode = AStar(::g_Graph, currentNode, homeBase);

	if (targetNode != nullptr)
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

	this->m_Timer = 0.0f;
	mCurrentCondition = 0;
}

void ReturnState::OnExitState()
{
	printf("ReturnState: Exited\n");
}