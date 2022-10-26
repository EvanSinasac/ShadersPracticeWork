#include "Graph.h"
#include <iostream>

Graph::Graph()
{

}

Node* Graph::CreateNode(unsigned int id, glm::vec3 position, bool bHasGoal)
{
	Node* node = new Node();
	node->id = id;
	node->visited = false;
	node->position = position;
	node->parent = NULL;
	node->hasGoal = bHasGoal;
	node->costSoFar = FLT_MAX;
	node->hDistance = 0.0f;
	node->type = "-";
	node->isHomeBase = false;

	node->isOccupied = false;
	node->occupiedBy = -1;

	node->m_Footprints = new cFootprints(position);

	this->nodes.push_back(node);
	return node;
}

Node* Graph::CreateNode(unsigned int id, glm::vec3 position, std::string typ, bool isHomeBase, bool bHasGoal, bool bIsExit)
{
	Node* node = new Node();
	node->id = id;
	node->visited = false;
	node->position = position;
	node->parent = NULL;
	node->hasGoal = bHasGoal;
	node->costSoFar = FLT_MAX;
	node->hDistance = 0.0f;
	node->type = typ;
	node->isHomeBase = isHomeBase;
	node->isExit = bIsExit;

	node->isOccupied = false;
	node->occupiedBy = -1;
	node->m_Footprints = new cFootprints(position);

	this->nodes.push_back(node);
	return node;
}

void Graph::AddEdge(Node* origin, Node* destination, float weight, bool bUndirected)
{
	pair<Node*, float> edge;
	float defaultWeight = weight;
	edge.first = destination;
	if (destination->type == "y")
	{
		defaultWeight *= 2.0f;
	}
	edge.second = defaultWeight;
	origin->edges.push_back(edge);

	if (bUndirected)
	{
		defaultWeight = weight;
		pair<Node*, float> reverseEdge;
		reverseEdge.first = origin;
		if (origin->type == "y")
		{
			defaultWeight *= 2.0f;
		}
		reverseEdge.second = defaultWeight;
		destination->edges.push_back(reverseEdge);
	}
}

void Graph::PrintGraph()
{
	for (Node*& currNode : this->nodes)
	{
		cout << "Node " << currNode->id << " -> ";
		for (pair<Node*, float>& edge : currNode->edges)
		{
			cout << " (" << edge.first->id << ", " << edge.second << ") ";
		}
		cout << "  X: " << currNode->position.x << "  Y: " << currNode->position.y << "  Z: " << currNode->position.z;
		//if (currNode->hasGoal)
		//{
		//	cout << " this node has a resource";
		//}
		//if (currNode->isHomeBase)
		//{
		//	cout << " this node is the homeBase";
		//}
		cout << endl;
	}
}

void Graph::ResetGraph()
{
	for (Node*& currNode : this->nodes)
	{
		currNode->visited = false;
		currNode->parent = NULL;
		currNode->costSoFar = FLT_MAX;
	}
}

void Graph::PrintParents(bool includeCost)
{
	for (Node*& currNode : this->nodes)
	{
		cout << "Node: " << currNode->id << " -> ";
		if (currNode->parent != NULL)
		{
			cout << currNode->parent->id; 
		}
		else
		{
			cout << "NULL";
		}
		if (includeCost)
		{
			cout << " cost so far: " << currNode->costSoFar << " hDist= " << currNode->hDistance << " f cost = " << currNode->costSoFar + currNode->hDistance;
		}
		cout << endl;
	}
}