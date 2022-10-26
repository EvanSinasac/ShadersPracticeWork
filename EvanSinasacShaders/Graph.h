#pragma once

//#include "Vertex.h"
#include <vector>
#include <string>
#include "GLMCommon.h"
#include "cFootprints.h"

using namespace std;

struct Node
{
	//char id;
	unsigned int id;
	bool visited;
	bool hasGoal;
	float costSoFar;
	float hDistance;
	//Vertex position;					// position in our game world.  Used for calculating distances
	glm::vec3 position;
	struct Node* parent;				// parent Node that we can use to follow back to the root node
	vector<pair<Node*, float>> edges;	// Edges pointing to our neighbouring nodes <neighbour, edgeWeight>
	//char type;
	std::string type;
	bool isHomeBase;
	bool isExit;

	bool isOccupied;
	int occupiedBy;

	cFootprints* m_Footprints;
};

class Graph
{
public:
	vector<Node*> nodes;				// All the nodes belonging to this graph

	Graph();
	Node* CreateNode(unsigned int id, glm::vec3 position, bool bHasGoal = false);
	Node* CreateNode(unsigned int id, glm::vec3 position, std::string typ, bool isHomeBase = false, bool bHasGoal = false, bool bIsExit = false);
	void AddEdge(Node* origin, Node* destination, float weight, bool bUndirected = true);
	void PrintGraph();

	void ResetGraph();
	void PrintParents(bool includeCost = false);
};