#pragma once
#include <vector>
#include <iostream>

template <typename T>
concept HasGetManhattanFromTarget = requires(T t1, T* t2) {
	{ t1.GetManhattanFromTarget(t2) } -> std::same_as<int>;  
};

template <typename T>
requires HasGetManhattanFromTarget<T>
struct Node
{
	T* data;

	bool walkable;
	bool visited;
	bool inPath;

	int distToStart;
	int distToTarget;

	Node* cameFrom;

	std::vector<Node*> neighbors;

	void CalculateDistToTarget(const Node<T>* node) {
		distToTarget = data->GetManhattanFromTarget(node->data);
	}
};