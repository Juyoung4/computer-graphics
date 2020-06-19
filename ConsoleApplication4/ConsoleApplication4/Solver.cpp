//James Aubin 
//522009777
//CSCE 420
//Due: February 12, 2018
//Solver.cpp
#include "stdafx.h"
#include "Solver.h"

using namespace std;

//helper function, used to sort open set with lowest total cost at back 
bool compare(Node* lhs, Node* rhs)
{
	return (lhs->getTotalCost() > rhs->getTotalCost());
}

Solver::Solver(vector<vector<int>> board)
{
	this->root = new Node(board);
}

void Solver::AStar(vector<vector<int>> goalState)
{
	Node* start = this->root;
	if (isGoal(start->getBoardState(), goalState))
	{
		cout << "Solution found  (no moves generated)" << endl;
		printSolutionSteps(getSolutionSteps(start));
		return;
	}

	//cannot use std::find on a single-ended queue
	deque<Node*> openSet;
	vector<Node*> closedSet;

	openSet.push_back(start);
	start->setMoveCost(0);
	start->updateTotalCost(goalState);

	while (!openSet.empty())
	{
		sort(openSet.begin(), openSet.end(), compare);
		Node* current = openSet.back();

		if (isGoal(current->getBoardState(), goalState))
		{
			cout << "Solution found! (" << openSet.size() + closedSet.size() << " moves generated)" << endl;
			printSolutionSteps(getSolutionSteps(current));
			return;
		}

		openSet.pop_back();
		closedSet.push_back(current);

		vector<Node*> successors = current->getSuccessors();

		for (int i = 0; i < successors.size(); i++)
		{
			Node* currSuccessor = successors.at(i);

			//if successor is in closed set, ignore it
			if (find(closedSet.begin(), closedSet.end(), currSuccessor) != closedSet.end())
			{
				continue;
			}
			//if successor is not in the open set AND does not match the current node, evaluate it
			if ((find(openSet.begin(), openSet.end(), currSuccessor) == openSet.end())
				&& (!boardsMatch(current->getBoardState(), currSuccessor->getBoardState())))
			{
				currSuccessor->setMoveCost(current->getMoveCost() + 1);
				currSuccessor->updateTotalCost(goalState);

				//~ currSuccessor->setParent(current);	//attach successor to tree
				openSet.push_back(currSuccessor);
			}
		}
		closedSet.push_back(current);
	}
}

bool Solver::isGoal(vector<vector<int>> currState, vector<vector<int>> goalState)
{
	for (int i = 0; i < currState.size(); i++)
	{
		for (int j = 0; j < currState.at(i).size(); j++)
		{
			if (currState.at(i).at(j) != goalState.at(i).at(j))
			{
				return false;
			}
		}
	}
	return true;
}

bool Solver::boardsMatch(vector<vector<int>> state1, vector<vector<int>> state2)
{
	return isGoal(state1, state2);
}

void Solver::printSolutionSteps(vector<Node*> steps)
{
	cout << "Move count: " << steps.size() - 1 << endl; //disregard path start in move count
	for (int i = 0; i < steps.size(); i++)
	{
		if (steps.at(i)->getParent() == nullptr)
		{
			cout << "START:" << endl;
		}
		else
		{
			cout << "move blank " << steps.at(i)->getDirection() << endl;
		}

		steps.at(i)->print(); //print the current board state
	}

}

vector<Node*> Solver::getSolutionSteps(Node* goal)
{

	//Start from goal node, move backwards to start
	vector<Node*> stepBuffer; //container for solution steps
	Node* current = goal;

	while (current != nullptr)
	{
		//insert at the front - traversing path backwards
		stepBuffer.insert(stepBuffer.begin(), current);
		current = current->getParent();
	}

	return stepBuffer;
}