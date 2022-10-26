#include "GatherState.h"
#include "globalThings.h"

GatherState::GatherState(void)
	: FSMState("Gather State") {
	this->m_Timer = 0.0f;
}

GatherState::~GatherState(void) {
}

glm::vec3 GatherState::Update(float dt, glm::vec3 curPos, int& stateID)
{
	stateID = 2;
	// wait 4 seconds then enter return state
	this->m_Timer += dt;
	if (this->m_Timer >= 4.0f)
	{
		printf("GatherState: Gathered a resource!\n");
		gNumResources--;
		mCurrentCondition = 1;

		// find the mesh of the node at the current location
		// if I were to redo this, I would definitely make the resources as a kind of entity rather than just an int
		// and the entity would have their own states, maybe it's a renewable source that after a timer the resource
		// would be available again

		for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
		{
			if (glm::distance(curPos, ::g_vec_pMeshes[index]->positionXYZ) < 0.1f)
			{
				::g_vec_pMeshes[index]->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.6f, 0.0f, 1.0f);
				break;
			}
		}

	}
	return curPos;
}

void GatherState::OnEnterState(glm::vec3 curPos) 
{
	// we'll mark the resource as gathered or being gathered so that if other entites get here after they'll move on
	printf("GatherState: Entered\n");
	
	Node* currentNode = nullptr;	// this could've been in the search state exit, but since I already need curPos for OnEnter for Search and Return this works better
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
		currentNode->hasGoal = false;
	}

	this->m_Timer = 0.0f;
	mCurrentCondition = 0;
}

void GatherState::OnExitState(void) {
	printf("GatherState: Exited\n");
}
