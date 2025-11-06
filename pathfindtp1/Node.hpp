#pragma once
#include <vector>
#include <iostream>

template <typename T>
struct Node
{
	T* data;

	bool visited;
	bool inPath;
	Node* cameFrom;

	std::vector<Node*> neighbors;
};