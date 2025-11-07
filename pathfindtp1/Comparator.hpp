#pragma once
#include "Node.hpp"
#include "Tile.h"

struct CompareBfs
{
	bool operator()(Node<Tile>* a, Node<Tile>* b) {
		return a->distToStart > b->distToStart;
	}
};

struct CompareDfs
{
	bool operator()(Node<Tile>* a, Node<Tile>* b) {
		return a->distToTarget > b->distToTarget;
	}
};

struct CompareAStar
{
	bool operator()(Node<Tile>* a, Node<Tile>* b) {
		return (a->distToTarget + a->distToStart) > (b->distToTarget + b->distToStart);
	}
};