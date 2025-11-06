#pragma once
#include "Node.hpp"
#include "Tile.h"

struct CompareBfs
{
	bool operator()(Node<Tile>* a, Node<Tile>* b) {
		return a->data->distToStart > b->data->distToStart;
	}
};

struct CompareDfs
{
	bool operator()(Node<Tile>* a, Node<Tile>* b) {
		return a->data->distToTarget > b->data->distToTarget;
	}
};

struct CompareAStar
{
	bool operator()(Node<Tile>* a, Node<Tile>* b) {
		return (a->data->distToTarget + a->data->distToStart) > (b->data->distToTarget + b->data->distToStart);
	}
};