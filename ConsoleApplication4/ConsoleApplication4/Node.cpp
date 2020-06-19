//James Aubin 
//522009777
//CSCE 420
//Due: February 12, 2018
//Node.cpp
#include "stdafx.h"
#include "Node.h"

using namespace std;

Node::Node(vector<vector<int>> boardState) :
	parent(nullptr), boardState(boardState), moveCost(0), totalCost(0)
{

}

Node::Node(vector<vector<int>> prevState, Direction newMove)
{

	this->boardState = prevState;
	tuple<int, int> emptyPosition = getEmptyPosition(this->boardState);
	int emptyRow = get<0>(emptyPosition);
	int emptyCol = get<1>(emptyPosition);

	//perform moves on the new node by swapping the empty tile with its neighbor
	switch (newMove)
	{
	case Node::Direction::UP:
		swap(this->boardState.at(emptyRow).at(emptyCol), this->boardState.at(emptyRow - 1).at(emptyCol));
		break;

	case Node::Direction::DOWN:
		swap(this->boardState.at(emptyRow).at(emptyCol), this->boardState.at(emptyRow + 1).at(emptyCol));
		break;

	case Node::Direction::LEFT:
		swap(this->boardState.at(emptyRow).at(emptyCol), this->boardState.at(emptyRow).at(emptyCol - 1));
		break;

	case Node::Direction::RIGHT:
		swap(this->boardState.at(emptyRow).at(emptyCol), this->boardState.at(emptyRow).at(emptyCol + 1));
		break;
	default:
		break;
	}

}

void Node::print()
{
	for (int i = 0; i < this->boardState.size(); i++)
	{
		for (int j = 0; j < this->boardState.at(i).size(); j++)
		{
			if (this->boardState.at(i).at(j) != -1)
			{
				cout << this->boardState.at(i).at(j) << " ";
			}
		}
		cout << endl;
	}

}

void Node::updateTotalCost(vector<vector<int>> goalState)
{
	this->totalCost = this->moveCost + this->heuristic(goalState);
}

void Node::setMoveCost(int moveCost) { this->moveCost = moveCost; }
void Node::setTotalCost(int totalCost) { this->totalCost = totalCost; }

void Node::setParent(Node* parent) { this->parent = parent; }

Node* Node::getParent() { return this->parent; }

bool Node::isBounded(vector<vector<int>> boardState, int rowNum, int colNum)
{
	return (boardState.at(rowNum).at(colNum) != -1);
}

vector<Node*> Node::getSuccessors()
{
	vector<Node::Direction> validMoves = getValidMoves(this->boardState);
	vector<Node*> successorVec;

	for (int i = 0; i < validMoves.size(); i++)
	{
		Node* currSuccessor = new Node(this->boardState, validMoves.at(i));
		currSuccessor->pathDirection = validMoves.at(i);
		currSuccessor->parent = this;
		successorVec.push_back(currSuccessor);
	}

	return successorVec;
}

vector<vector<int>> Node::getBoardState() { return this->boardState; }

Node::Direction Node::getDirection() { return this->pathDirection; }

int Node::getTotalCost() { return this->totalCost; }
int Node::getMoveCost() { return this->moveCost; }

int Node::heuristic(std::vector<std::vector<int>> goalState)
{
	int numDifferences = 0;
	for (int i = 0; i < goalState.size(); i++)
	{
		for (int j = 0; j < goalState.at(i).size(); j++)
		{
			if (this->boardState.at(i).at(j) != goalState.at(i).at(j))
			{
				numDifferences++;
			}
		}
	}

	return numDifferences;
}

vector<Node::Direction> Node::getValidMoves(vector<vector<int>> boardState)
{
	//get the row/col position of the empty puzzle slot
	tuple<int, int> emptyPosition = getEmptyPosition(boardState);
	int emptyRow = get<0>(emptyPosition);
	int emptyCol = get<1>(emptyPosition);

	vector<Node::Direction> validMoves;

	//check above the empty slot
	if (isBounded(boardState, emptyRow - 1, emptyCol))
	{
		validMoves.push_back(Node::Direction::UP);
	}
	//check below the empty slot
	if (isBounded(boardState, emptyRow + 1, emptyCol))
	{
		validMoves.push_back(Node::Direction::DOWN);
	}
	//check to the left of the empty slot
	if (isBounded(boardState, emptyRow, emptyCol - 1))
	{
		validMoves.push_back(Node::Direction::LEFT);
	}
	//check to the right of the empty slot
	if (isBounded(boardState, emptyRow, emptyCol + 1))
	{
		validMoves.push_back(Node::Direction::RIGHT);
	}

	return validMoves;
}

tuple<int, int> Node::getEmptyPosition(vector<vector<int>> boardState)
{
	for (int i = 0; i < boardState.size(); i++)
	{
		for (int j = 0; j < boardState.at(i).size(); j++)
		{
			if (boardState.at(i).at(j) == 0)
			{
				return make_tuple(i, j);
			}
		}
	}
	return make_tuple(-1, -1);
}

ostream& operator<<(std::ostream& os, const Node::Direction& direction)
{
	switch (direction)
	{
	case Node::Direction::UP:
		cout << "UP";
		break;
	case Node::Direction::DOWN:
		cout << "DOWN";
		break;
	case Node::Direction::LEFT:
		cout << "LEFT";
		break;
	case Node::Direction::RIGHT:
		cout << "RIGHT";
		break;
	default:
		cout << "?";
		break;
	}

	//~ os << direction;
	return os;
}