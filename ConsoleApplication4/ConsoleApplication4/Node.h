#pragma once
//James Aubin 
//522009777
//CSCE 420
//Due: February 12, 2018
//Node.h
#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <utility>

class Node
{
private:

	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Direction pathDirection;

	Node* parent;	//reference to parent
	std::vector<std::vector<int>> boardState;
	int moveCost, totalCost; //A* f/g values

public:
	//constructor(s)
	Node(std::vector<std::vector<int>> boardState);

	//used to expand nodes 
	Node(std::vector<std::vector<int>> prevState, Direction newMove);
	Direction getDirection();

	int heuristic(std::vector<std::vector<int>> goalState);
	int getTotalCost();
	int getMoveCost();

	std::vector<Direction> getValidMoves(std::vector<std::vector<int>> boardState);

	std::vector<Node*> getSuccessors();

	std::vector<std::vector<int>> getBoardState();

	std::tuple<int, int> getEmptyPosition(std::vector<std::vector<int>> boardState);

	bool isBounded(std::vector<std::vector<int>> boardState, int rowNum, int colNum);

	void print();
	void setParent(Node* parent);
	void updateTotalCost(std::vector<std::vector<int>> goalState);
	void setTotalCost(int totalCost);
	void setMoveCost(int moveCost);

	Node* getParent();

	friend std::ostream& operator<<(std::ostream& os, const Node::Direction& direction);
};