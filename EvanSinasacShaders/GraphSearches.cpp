#include <iostream>
#include <stdio.h>
#include <stack>
#include <queue>
#include <list>

#include "Graph.h"

// Depth first search
Node* DFS(Graph* graph, Node* rootNode)
{
	rootNode->visited = true;
	rootNode->parent = NULL;

	stack<Node*> openStack;
	openStack.push(rootNode);

	// keep checking the nodes inside the stack until all nodes have been checked
	while (!openStack.empty())
	{
		Node* currNode = openStack.top();
		openStack.pop();

		cout << currNode->id << endl;

		// Next, check if the node contains our goal
		if (currNode->hasGoal)
		{
			return currNode;
		}

		// Check all neighbouring nodes to add to the stack if unvisited
		for (pair<Node*, float>& neighbour : currNode->edges)
		{
			if (neighbour.first->visited == false)
			{
				neighbour.first->visited = true;
				neighbour.first->parent = currNode;
				openStack.push(neighbour.first);
			}
		}

	}

	return NULL;
}

// Breadth first search
Node* BFS(Graph* graph, Node* rootNode)
{
	rootNode->visited = true;
	queue<Node*> openQueue;
	openQueue.push(rootNode);

	while (!openQueue.empty())
	{
		Node* currNode = openQueue.front();
		openQueue.pop();

		cout << currNode->id << endl;

		if (currNode->hasGoal)	// check if the node contains our goal
		{
			return currNode;
		}


		// we check all the neighbouring nodes and add any that have not been added to the queue yet
		for (pair <Node*, float>& neighbhour : currNode->edges)
		{
			//if (!neighbhour.first->visited)
			if (neighbhour.first->visited == false)
			{
				neighbhour.first->visited = true;
				neighbhour.first->parent = currNode;
				openQueue.push(neighbhour.first);
			}
		}
	}

	return NULL;	// no goal has been found
}

// Breadth First Search for player within enclosed area between door nodes
Node* BFS_LookForPlayerWithinEnclosedArea(Graph* graph, Node* rootNode)
{
	graph->ResetGraph();
	rootNode->visited = true;
	queue<Node*> openQueue;
	openQueue.push(rootNode);		// the root will never contain the player but we still need to start with it so we don't re-search it

	while (!openQueue.empty())
	{
		Node* currNode = openQueue.front();
		openQueue.pop();

		//cout << currNode->id << endl;

		//if (currNode->hasGoal)	// check if the node contains our goal
		if (currNode->isOccupied && currNode->occupiedBy == 0)	// occupied by player
		{
			return currNode;
		}


		// we check all the neighbouring nodes and add any that have not been added to the queue yet
		for (pair <Node*, float>& neighbhour : currNode->edges)
		{
			//if (!neighbhour.first->visited)
			if (neighbhour.first->visited == false &&
				!(neighbhour.first->type == "D" || neighbhour.first->type == "DP" || neighbhour.first->type == "DS")
				/*&& !(neighbhour.first->isOccupied && neighbhour.first->occupiedBy == 0)*/)	// go up to door nodes
			{
				neighbhour.first->visited = true;
				neighbhour.first->parent = currNode;
				openQueue.push(neighbhour.first);
			}
		}
	}

	return NULL;	// no goal has been found
}

Node* GetLowestCostNode(list<Node*> openList)
{
	float minCost = FLT_MAX;
	Node* lowestCostNode = NULL;
	// find the node with the lowest cost so far value
	for (auto iter = openList.begin(); iter != openList.end(); iter++)
	{
		if ((*iter)->costSoFar < minCost)
		{
			minCost = (*iter)->costSoFar;
			lowestCostNode = *iter;
		}
	}
	return lowestCostNode;
}

bool IsNodeInOpenList(list<Node*> openList, Node* neighbour)
{
	for (auto iter = openList.begin(); iter != openList.end(); iter++)
	{
		if (*iter == neighbour)
			return true;
	}
	return false;
}

void PrintListContents(list<Node*> myList)
{
	for (auto iter = myList.begin(); iter != myList.end(); iter++)
	{
		cout << (*iter)->id << " ";
	}
	cout << endl;
}

Node* Dijkstra(Graph* graph, Node* rootNode)
{
	graph->ResetGraph();

	rootNode->costSoFar = 0.0f;

	list<Node*> openList;
	list<Node*> closedList;

	openList.push_back(rootNode);

	while (!openList.empty())
	{
		Node* currNode = GetLowestCostNode(openList);	// gets the node with lowest cost
		openList.remove(currNode);
		closedList.push_back(currNode);

		currNode->visited = true;
		if (currNode->hasGoal)
		{
			return currNode;
		}

		// Go through every neighbouring node of the current node
		for (pair<Node*, float> neighbour : currNode->edges)
		{
			if (neighbour.first->visited == false)
			{
				float weightSoFar = currNode->costSoFar + neighbour.second;
				if (weightSoFar < neighbour.first->costSoFar)
				{
					neighbour.first->costSoFar = weightSoFar;
					neighbour.first->parent = currNode;
					if (!IsNodeInOpenList(openList, neighbour.first))
					{
						openList.push_back(neighbour.first);
					}
				}
			}
		}

		//cout << currNode->id << endl;
		//cout << "Open List = ";
		//PrintListContents(openList);
		//cout << "Closed List = ";
		//PrintListContents(closedList);

		//graph->PrintParents(true);

	}

	return NULL;
}

float CalculateHeuristicDistances(Node* node, Node* goal)
{
	float D = 1;
	float dx = abs(node->position.x - goal->position.x);
	float dy = abs(node->position.y - goal->position.y);
	return D * (dx + dy);
}

Node* GetLowestFCostNode(list<Node*> openList)
{
	float minCost = FLT_MAX;
	Node* lowestCostNode = NULL;
	// find the node with the lowest f cost
	for (auto iter = openList.begin(); iter != openList.end(); iter++)
	{
		if ((*iter)->costSoFar + (*iter)->hDistance < minCost)
		{
			minCost = (*iter)->costSoFar + (*iter)->hDistance;
			lowestCostNode = *iter;
		}
	}
	return lowestCostNode;
}

// For this project, AStar is looking for the path to the homeBase, so the goalNode won't have the hasGoal as true
Node* AStar(Graph* graph, Node* rootNode, Node* goal)
{
	rootNode->costSoFar = 0;
	rootNode->hDistance = CalculateHeuristicDistances(rootNode, goal);

	list<Node*> openList;
	list<Node*> closedList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		// remove the current node from the open list with the lowest f cost
		Node* currNode = GetLowestFCostNode(openList);
		openList.remove(currNode);
		closedList.push_back(currNode);


		currNode->visited = true;
		//if (currNode->hasGoal)
		//{
		//	return currNode;
		//}
		//if (currNode->isHomeBase)
		if (currNode->isExit)
		{
			return currNode;
		}

		// Go through each neighbouring node of current node
		for (pair<Node*, float>neighbour : currNode->edges)
		{
			if (neighbour.first->visited == false /*&& !neighbour.first->isOccupied*/)
			{
				float weightSoFar = currNode->costSoFar + neighbour.second;
				if (weightSoFar < neighbour.first->costSoFar)
				{
					neighbour.first->costSoFar = weightSoFar;
					neighbour.first->parent = currNode;
					if (!IsNodeInOpenList(openList, neighbour.first))
					{
						neighbour.first->hDistance = CalculateHeuristicDistances(neighbour.first, goal);
						openList.push_back(neighbour.first);
					}
				}
			}
		}
		//cout << currNode->id << endl;
		//cout << "Open List = ";
		//PrintListContents(openList);
		//cout << "Closed List = ";
		//PrintListContents(closedList);

		//graph->PrintParents(true);
	}

	return NULL;
}