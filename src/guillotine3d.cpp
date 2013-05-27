/*
 * guillotine3d.cpp
 *
 *  Created on: 24-05-2013
 *      Author: krris
 */

#include "../include/guillotine3d.hpp"

using namespace std;

Guillotine3d::Guillotine3d(int width, int depth)
{
	init(width, depth);
}

void Guillotine3d::init(int width, int depth)
{
	binWidth = width;
	binHeight = std::numeric_limits<int>::max();
	binDepth = depth;

	usedCuboids.clear();

	// Fill a whole bin with a big cuboid
	Cuboid n;
	n.x = 0;
	n.y = 0;
	n.z = 0;
	n.width = width;
	n.depth = depth;
	n.height = std::numeric_limits<int>::max();

	freeCuboids.clear();
	freeCuboids.push_back(n);
}

Cuboid Guillotine3d::insert(int width, int height, int depth,
		FreeCuboidChoiceHeuristic cuboidChoice,
		GuillotineSplitHeuristic splitMethod)
{
	// Find where to put the new cuboid
	int freeNodeIndex = 0;
	Cuboid newCuboid = findPositionForNewNode(width, height, depth, cuboidChoice, &freeNodeIndex);

	// Abort if we didn't have enough space in the bin
	if (newCuboid.isPlaced == false)
		return newCuboid;

	// Remove the space that was just consumed by the new cuboid
	splitFreeCuboidByHeuristic(freeCuboids[freeNodeIndex], newCuboid, splitMethod);
	freeCuboids.erase(freeCuboids.begin() + freeNodeIndex);

	// Remember the new used cuboid
	usedCuboids.push_back(newCuboid);

	return newCuboid;
}

Cuboid Guillotine3d::findPositionForNewNode(int width, int height, int depth,
		FreeCuboidChoiceHeuristic cuboidChoice, int* nodeIndex) const
{
	Cuboid bestNode;

	int bestScore = std::numeric_limits<int>::max();

	// Try each free cuboid to find the best one for placement
	for (unsigned i = 0; i < freeCuboids.size(); ++i)
	{
		if (width <= freeCuboids[i].width && height <= freeCuboids[i].height && depth <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(width, height, depth, freeCuboids[i], cuboidChoice);

			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeCuboids[i].x;
				bestNode.y = freeCuboids[i].y;
				bestNode.z = freeCuboids[i].z;
				bestNode.width = width;
				bestNode.height = height;
				bestNode.depth = depth;
				bestScore = score;
				*nodeIndex = i;
			}
		}
	}
	return bestNode;
}

int Guillotine3d::scoreByHeuristic(int width, int height, int depth,
		const Cuboid& freeCuboid, FreeCuboidChoiceHeuristic cuboidChoice)
{
	return scoreMinHeight(width, height, depth, freeCuboid);
}


void Guillotine3d::splitFreeCuboidByHeuristic(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, GuillotineSplitHeuristic method)
{
	// Compute the lengths of the leftover area.
	const int w = freeCuboid.width - placedCuboid.width;
	const int d = freeCuboid.depth- placedCuboid.depth;

	// Placing placedCuboid into freeCuboid results in an L-shaped free area, which must be split into
	// two disjoint cuboidangles. This can be achieved with by splitting the L-shape using a single line.
	// We have two choices: horizontal or vertical.

	// Use the given heuristic to decide which choice to make.

	bool splitHorizontal;
	switch(method)
	{
	case SplitShorterLeftoverAxis:
		// Split along the shorter leftover axis.
		splitHorizontal = (w <= d);
		break;
	case SplitLongerLeftoverAxis:
		// Split along the longer leftover axis.
		splitHorizontal = (w > d);
		break;
	case SplitShorterAxis:
		// Split along the shorter total axis.
		splitHorizontal = (freeCuboid.width <= freeCuboid.depth);
		break;
	case SplitLongerAxis:
		// Split along the longer total axis.
		splitHorizontal = (freeCuboid.width > freeCuboid.depth);
		break;
	default:
		splitHorizontal = true;
		assert(false);
	}

	// Perform the actual split.
	splitFreeCuboidAlongAxis(freeCuboid, placedCuboid, splitHorizontal);
}

int Guillotine3d::scoreMinHeight(int width, int height, int depth,
		const Cuboid& freeCuboid)
{
	int filledBinHeight = freeCuboid.y;
	return filledBinHeight + height;
}

Cuboid Guillotine3d::insertBestGlobal(std::vector<Cuboid> cuboids, Guillotine3d guillotine,
		GuillotineSplitHeuristic splitMethod)
{
	vector<Cuboid> possibleMoves;

	for (unsigned i = 0; i < cuboids.size(); ++i)
	{
		Cuboid nextCuboid = cuboids[i];

		vector<Cuboid> cuboidsToPut;
		// copy all cuboids without nextCuboid
		for (unsigned j = 0; j < cuboids.size(); ++j)
		{
			if (j != i)
				cuboidsToPut.push_back(cuboids[j]);
		}

		Guillotine3d guillotine_copy = guillotine;

		nextCuboid = guillotine_copy.insert(nextCuboid.width, nextCuboid.height, nextCuboid.depth,
				Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
		for (Cuboid c : cuboidsToPut)
		{
			Cuboid newCuboid = guillotine_copy.insert(c.width, c.height, c.depth,
					Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
		}

		int score = guillotine_copy.getFilledBinHeight();
		nextCuboid.score = score;
		possibleMoves.push_back(nextCuboid);
	}

	// find best cuboid with miminal score
	Cuboid best = possibleMoves[0];
	for (Cuboid c : possibleMoves)
	{
		if (c.score < best.score)
			best = c;
	}
	return best;



//	for (Cuboid c : cuboids)
//	{
//		//vector<Cuboid> cuboidMoves = movePossibilities(c, splitMethod);
//		//possibleMoves.insert(possibleMoves.end(), cuboidMoves.begin(), cuboidMoves.end());
//		Cuboid nextCuboid = c;
//		for (Cuboid cc : cuboids)
//		Guillotine3d g_copy = guillotine;
//
//		Cuboid newCuboid = g_copy.insert(c.width, c.height, c.depth, Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
//
//	}
//
//	Cuboid best = possibleMoves[0];
//	for (Cuboid c : possibleMoves)
//	{
//		if (c.score < best.score)
//		{
//			best = c;
//		}
//	}
//
//	// Abort if we didn't have enough space in the bin
//	if (best.isPlaced == false)
//		return best;
//
//	// Remove the space that was just consumed by the new cuboid
//	splitFreeCuboidByHeuristic(freeCuboids[best.freeCuboidIndex], best, splitMethod);
//	freeCuboids.erase(freeCuboids.begin() + best.freeCuboidIndex);
//
//	// Remember the new used cuboid
//	usedCuboids.push_back(best);
}

int Guillotine3d::getFilledBinHeight()
{
	vector<Cuboid> usedCuboids = getUsedCuboids();
	int max = -1;
	for (Cuboid c : usedCuboids)
	{
		int height = c.y + c.height;
		if (height > max)
			max = height;
	}
	return max;
}

std::vector<Cuboid> Guillotine3d::movePossibilities(Cuboid cuboid,
		GuillotineSplitHeuristic splitMethod)
{
	int width = cuboid.width;
	int height = cuboid.height;
	int depth = cuboid.depth;
	vector<Cuboid> possibleMoves;
	// Try each free cuboid to find the best one for placement
	for (unsigned i = 0; i < freeCuboids.size(); ++i)
	{
		if (width <= freeCuboids[i].width && height <= freeCuboids[i].height && depth <= freeCuboids[i].depth)
		{
			int score = scoreMinHeight(width, height, depth, freeCuboids[i]);
			Cuboid newCuboid;

			newCuboid.isPlaced = true;
			newCuboid.x = freeCuboids[i].x;
			newCuboid.y = freeCuboids[i].y;
			newCuboid.z = freeCuboids[i].z;
			newCuboid.width = width;
			newCuboid.height = height;
			newCuboid.depth = depth;
			newCuboid.score = score;
			newCuboid.freeCuboidIndex = i;

			possibleMoves.push_back(newCuboid);
		}
	}
	return possibleMoves;

}

void Guillotine3d::splitFreeCuboidAlongAxis(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, bool splitHorizontal)
{
	// Form the three new cuboids.
	Cuboid bottom;
	bottom.x = freeCuboid.x;
	bottom.y = freeCuboid.y;
	bottom.z = freeCuboid.z + placedCuboid.depth;
	bottom.depth= freeCuboid.depth- placedCuboid.depth;
	bottom.height = placedCuboid.height;

	Cuboid right;
	right.x = freeCuboid.x + placedCuboid.width;
	right.y = freeCuboid.y;
	right.z = freeCuboid.z;
	right.width = freeCuboid.width - placedCuboid.width;
	right.height = placedCuboid.height;

	Cuboid top;
	top.x = freeCuboid.x;
	top.y = freeCuboid.y + placedCuboid.height;
	top.z = freeCuboid.z;
	if (freeCuboid.height == std::numeric_limits<int>::max())
	{
		top.height = freeCuboid.height;
	}
	else
	{
		top.height = freeCuboid.height - top.y;
	}
	top.width = freeCuboid.width;
	top.depth = freeCuboid.depth;

	if (splitHorizontal)
	{
		bottom.width = freeCuboid.width;
		right.depth = placedCuboid.depth;
	}
	else // Split vertically
	{
		bottom.width = placedCuboid.width;
		right.depth = freeCuboid.depth;
	}

	// Add the new cuboids into the free cuboid
	if (bottom.width > 0 && bottom.depth > 0)
		freeCuboids.push_back(bottom);
	if (right.width > 0 && right.depth > 0)
		freeCuboids.push_back(right);
	freeCuboids.push_back(top);

}
