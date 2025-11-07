#pragma once
#include "Node.hpp"
#include "Tile.h"

struct CompareBfs
{
	template<typename T>
	bool operator()(Node<T>* a, Node<T>* b) {
		return a->distToStart > b->distToStart;
	}
};

struct CompareDfs
{
	template<typename T>
	bool operator()(Node<T>* a, Node<T>* b) {
		return a->distToTarget > b->distToTarget;
	}
};

struct CompareAStar
{
	template<typename T>
	bool operator()(Node<T>* a, Node<T>* b) {
		return (a->distToTarget + a->distToStart) > (b->distToTarget + b->distToStart);
	}
};