/*
 * guillotine3d.cpp
 *
 *  Created on: 24-05-2013
 *      Author: krris
 */

#include "../include/guillotine3d.hpp"

Guillotine3d::Guillotine3d(int width, int height, int depth)
{
	init(width, height, depth);
}

void Guillotine3d::init(int width, int height, int depth)
{
	binWidth = width;
	binHeight = height;
	binDepth = depth;

	usedCuboids.clear();

	// Start with a single big free cuboidangle that spans the whole bin
	Cuboid n;
	n.x = 0;
	n.y = 0;
	n.z = 0;
	n.width = width;
	n.height = height;
	n.depth = depth;

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
	//if (newCuboid.height == 0)
	//	return newCuboid;

	// Remove the space that was just consumed by the new cuboidangle.
	splitFreeCuboidByHeuristic(freeCuboids[freeNodeIndex], newCuboid, splitMethod);
	freeCuboids.erase(freeCuboids.begin() + freeNodeIndex);

	// Remember the new used cuboid
	usedCuboids.push_back(newCuboid);

	return newCuboid;
}

bool Guillotine3d::fits(int width, int height, int depth,
		FreeCuboidChoiceHeuristic cuboidChoice) const
{
	int nodeIndex;
	Cuboid cuboid = findPositionForNewNode(width, height, depth, cuboidChoice, &nodeIndex);
	if (cuboid.isPlaced == true)
		return true;
	else
		return false;
}

Cuboid Guillotine3d::findPositionForNewNode(int width, int height, int depth,
		FreeCuboidChoiceHeuristic cuboidChoice, int* nodeIndex) const
{
	Cuboid bestNode;

	int bestScore = std::numeric_limits<int>::max();

	// Try each free cuboid to find the best one for placement
	for (unsigned i = 0; i < freeCuboids.size(); ++i)
	{
		// If this is a perfect fit upright, choose it immediately.
		if (width == freeCuboids[i].width &&
			height == freeCuboids[i].height && depth == freeCuboids[i].depth
			)
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
				//bestScore = std::numeric_limits<int>::min();
				bestScore = score;
				*nodeIndex = i;
			}
			break;
		}
//		else if (width == freeCuboids[i].width && depth == freeCuboids[i].height)
//		{
//			bestNode.isPlaced = true;
//			bestNode.x = freeCuboids[i].x;
//			bestNode.y = freeCuboids[i].y;
//			bestNode.z = freeCuboids[i].z;
//			bestNode.width = width;
//			bestNode.height = depth;
//			bestNode.depth = height;
//			bestScore = std::numeric_limits<int>::min();
//			*nodeIndex = i;
//			break;
//		}
//		// D H W
//		else if (depth == freeCuboids[i].width && height == freeCuboids[i].height)
//		{
//			bestNode.isPlaced = true;
//			bestNode.x = freeCuboids[i].x;
//			bestNode.y = freeCuboids[i].y;
//			bestNode.z = freeCuboids[i].z;
//			bestNode.width = depth;
//			bestNode.height = height;
//			bestNode.depth = width;
//			bestScore = std::numeric_limits<int>::min();
//			*nodeIndex = i;
//			break;
//		}
//		// D W H
//		else if (depth == freeCuboids[i].width && width == freeCuboids[i].height)
//		{
//			bestNode.isPlaced = true;
//			bestNode.x = freeCuboids[i].x;
//			bestNode.y = freeCuboids[i].y;
//			bestNode.z = freeCuboids[i].z;
//			bestNode.width = depth;
//			bestNode.height = width;
//			bestNode.depth = height;
//			bestScore = std::numeric_limits<int>::min();
//			*nodeIndex = i;
//			break;
//		}
//		// H D W
//		else if (height == freeCuboids[i].width && depth == freeCuboids[i].height)
//		{
//			bestNode.isPlaced = true;
//			bestNode.x = freeCuboids[i].x;
//			bestNode.y = freeCuboids[i].y;
//			bestNode.z = freeCuboids[i].z;
//			bestNode.width = height;
//			bestNode.height = depth;
//			bestNode.depth = width;
//			bestScore = std::numeric_limits<int>::min();
//			*nodeIndex = i;
//			break;
//		}
//		// H W D
//		else if (height == freeCuboids[i].width && width== freeCuboids[i].height)
//		{
//			bestNode.isPlaced = true;
//			bestNode.x = freeCuboids[i].x;
//			bestNode.y = freeCuboids[i].y;
//			bestNode.z = freeCuboids[i].z;
//			bestNode.width = height;
//			bestNode.height = width;
//			bestNode.depth = depth;
//			bestScore = std::numeric_limits<int>::min();
//			*nodeIndex = i;
//			break;
//		}
		// Does the cuboidangle fit upright?
		else if (width <= freeCuboids[i].width && height <= freeCuboids[i].height && depth <= freeCuboids[i].depth)
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
//		// Does the cuboidangle fit sideways?
//		else if (height <= freeCuboids[i].width && width <= freeCuboids[i].height)
//		{
//			int score = scoreByHeuristic(height, width, depth, freeCuboids[i], cuboidChoice);
//
//			if (score < bestScore)
//			{
//				bestNode.isPlaced = true;
//				bestNode.x = freeCuboids[i].x;
//				bestNode.y = freeCuboids[i].y;
//				bestNode.z = freeCuboids[i].z;
//				bestNode.width = height;
//				bestNode.height = width;
//				bestNode.depth = depth;
//				bestScore = score;
//				*nodeIndex = i;
//			}
//		}
	}
	return bestNode;
}

int Guillotine3d::scoreByHeuristic(int width, int height, int depth,
		const Cuboid& freeCuboid, FreeCuboidChoiceHeuristic cuboidChoice)
{
	switch(cuboidChoice)
	{
		case CuboidBestAreaFit: return scoreBestAreaFit(width, height, depth, freeCuboid);
		case CuboidBestShortSideFit: return scoreBestShortSideFit(width, height, depth, freeCuboid);
		case CuboidMinHeight: return scoreMinHeight(width, height, depth, freeCuboid);
		default: assert(false); return std::numeric_limits<int>::max();
	}
}

int Guillotine3d::scoreBestAreaFit(int width, int height, int depth,
		const Cuboid& freeCuboid)
{
	return freeCuboid.width * freeCuboid.height * freeCuboid.depth - width * height * depth;
}

int Guillotine3d::scoreBestShortSideFit(int width, int height, int depth,
		const Cuboid& freeCuboid)
{
	int leftoverHoriz = std::abs(freeCuboid.width - width);
	int leftoverVert = std::abs(freeCuboid.height - height);
	int leftoverDepth = std::abs(freeCuboid.depth - depth);
	int leftover = std::min({leftoverHoriz, leftoverVert, leftoverDepth});
	return leftover;
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
	case SplitMinimizeArea:
		// Maximize the larger area == minimize the smaller area.
		// Tries to make the single bigger cuboidangle.
		splitHorizontal = (placedCuboid.width * d > w * placedCuboid.depth);
		break;
	case SplitMaximizeArea:
		// Maximize the smaller area == minimize the larger area.
		// Tries to make the cuboidangles more even-sized.
		splitHorizontal = (placedCuboid.width * d <= w * placedCuboid.depth);
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

void Guillotine3d::splitFreeCuboidAlongAxis(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, bool splitHorizontal)
{
	// Form the two new cuboids.
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
	top.height = binHeight - top.y;
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

	// Add the new cuboidangles into the free cuboidangle pool if they weren't degenerate.
	if (bottom.width > 0 && bottom.depth > 0)
		freeCuboids.push_back(bottom);
	if (right.width > 0 && right.depth > 0)
		freeCuboids.push_back(right);
	freeCuboids.push_back(top);

}
