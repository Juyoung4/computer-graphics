#pragma once
//James Aubin 
//522009777
//CSCE 420
//Due: February 12, 2018
//Solver.h
#pragma once
#include <deque>
#include <iostream>
#include <algorithm>
#include "Node.h"

class Solver
{
private:
	Node * root;

public:
	Solver(std::vector<std::vector<int>> board);

	void AStar(std::vector<std::vector<int>> goalState);
	void printSolutionSteps(std::vector<Node*> steps);

	bool isGoal(std::vector<std::vector<int>> currState, std::vector<std::vector<int>> goalState);
	bool boardsMatch(std::vector<std::vector<int>> state1, std::vector<std::vector<int>> state2);

	std::vector<Node*> getSolutionSteps(Node* goal);
};