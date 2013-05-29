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

Cuboid Guillotine3d::insert(const Cuboid& cuboid,
		FreeCuboidChoiceHeuristic cuboidChoice,
		GuillotineSplitHeuristic splitMethod)
{
	// Find where to put the new cuboid
	int freeNodeIndex = 0;
	Cuboid newCuboid = findPositionForNewNode(cuboid, cuboidChoice, &freeNodeIndex);

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

Cuboid Guillotine3d::findPositionForNewNode(const Cuboid& cuboid,
		FreeCuboidChoiceHeuristic cuboidChoice, int* nodeIndex) const
{
	int width = cuboid.width;
	int height = cuboid.height;
	int depth = cuboid.depth;
	Cuboid bestNode;

	int bestScore = std::numeric_limits<int>::max();

	// Try each free cuboid to find the best one for placement a given cuboid

	/*
	 * W H D
	 * W D H
	 * D H W
	 * D W H
	 * H W D
	 * H D W
	 */

	for (unsigned i = 0; i < freeCuboids.size(); ++i)
	{
		// W H D
		if (width <= freeCuboids[i].width &&
			height <= freeCuboids[i].height &&
			depth <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

		// W D H
		if (width <= freeCuboids[i].width &&
			depth <= freeCuboids[i].height &&
			height <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeCuboids[i].x;
				bestNode.y = freeCuboids[i].y;
				bestNode.z = freeCuboids[i].z;
				bestNode.width = width;
				bestNode.height = depth;
				bestNode.depth = height;
				bestScore = score;
				*nodeIndex = i;
			}
		}

		// D H W
		if (depth <= freeCuboids[i].width &&
			height <= freeCuboids[i].height &&
			width <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeCuboids[i].x;
				bestNode.y = freeCuboids[i].y;
				bestNode.z = freeCuboids[i].z;
				bestNode.width = depth;
				bestNode.height = height;
				bestNode.depth = width;
				bestScore = score;
				*nodeIndex = i;
			}
		}

		// D W H
		if (depth <= freeCuboids[i].width &&
			width <= freeCuboids[i].height &&
			height <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeCuboids[i].x;
				bestNode.y = freeCuboids[i].y;
				bestNode.z = freeCuboids[i].z;
				bestNode.width = depth;
				bestNode.height = width;
				bestNode.depth = height;
				bestScore = score;
				*nodeIndex = i;
			}
		}

		// H W D
		if (height <= freeCuboids[i].width &&
			width <= freeCuboids[i].height &&
			depth <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeCuboids[i].x;
				bestNode.y = freeCuboids[i].y;
				bestNode.z = freeCuboids[i].z;
				bestNode.width = height;
				bestNode.height = width;
				bestNode.depth = depth;
				bestScore = score;
				*nodeIndex = i;
			}
		}

		// H D W
		if (height <= freeCuboids[i].width &&
			width <= freeCuboids[i].height &&
			depth <= freeCuboids[i].depth)
		{
			int score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeCuboids[i].x;
				bestNode.y = freeCuboids[i].y;
				bestNode.z = freeCuboids[i].z;
				bestNode.width = height;
				bestNode.height = depth;
				bestNode.depth = width;
				bestScore = score;
				*nodeIndex = i;
			}
		}
	}
	return bestNode;
}

int Guillotine3d::scoreByHeuristic(const Cuboid& cuboid,
		const Cuboid& freeCuboid,
		FreeCuboidChoiceHeuristic cuboidChoice)
{
	switch (cuboidChoice)
	{
	case CuboidMinHeight:
	default:
		return scoreMinHeight(cuboid, freeCuboid);
		break;
	}
	return 0;
}


void Guillotine3d::splitFreeCuboidByHeuristic(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, GuillotineSplitHeuristic method)
{
	// Compute the lengths of the leftover area.
	const int w = freeCuboid.width - placedCuboid.width;
	const int d = freeCuboid.depth- placedCuboid.depth;

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

int Guillotine3d::scoreMinHeight(const Cuboid& cuboid, const Cuboid& freeCuboid)
{
	int filledBinHeight = freeCuboid.y;
	return filledBinHeight + cuboid.height;
}

Cuboid Guillotine3d::insertBestGlobal(std::vector<Cuboid>& cuboids,
		const Guillotine3d& guillotine, GuillotineSplitHeuristic splitMethod)
{
	vector<Cuboid> possibleMoves;

	for (unsigned i = 0; i < cuboids.size(); ++i)
	{
		Cuboid nextCuboid = cuboids[i];

		vector<Cuboid> cuboidsToPut;
		// Copy all cuboids without nextCuboid
		for (unsigned j = 0; j < cuboids.size(); ++j)
		{
			if (j != i)
				cuboidsToPut.push_back(cuboids[j]);
		}

		Guillotine3d guillotine_copy = guillotine;

		nextCuboid = guillotine_copy.insert(nextCuboid,
				Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
		for (Cuboid c : cuboidsToPut)
		{
			guillotine_copy.insert(c, Guillotine3d::CuboidMinHeight,
					Guillotine3d::SplitLongerAxis);
		}

		int score = guillotine_copy.getFilledBinHeight();
		nextCuboid.score = score;
		possibleMoves.push_back(nextCuboid);
	}

	// Find best cuboid with miminal score
	Cuboid best = possibleMoves[0];
	for (Cuboid c : possibleMoves)
	{
		if (c.score < best.score)
			best = c;
	}
	return best;

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

std::vector<Cuboid> Guillotine3d::movePossibilities(const Cuboid& cuboid,
		GuillotineSplitHeuristic splitMethod)
{
	int width = cuboid.width;
	int height = cuboid.height;
	int depth = cuboid.depth;
	vector<Cuboid> possibleMoves;
	// Try each free cuboid to find the best one for placement
	for (unsigned i = 0; i < freeCuboids.size(); ++i)
	{
		if (width <= freeCuboids[i].width &&
			height <= freeCuboids[i].height &&
			depth <= freeCuboids[i].depth)
		{
			int score = scoreMinHeight(cuboid, freeCuboids[i]);
			Cuboid newCuboid;

			newCuboid.isPlaced = true;
			newCuboid.x = freeCuboids[i].x;
			newCuboid.y = freeCuboids[i].y;
			newCuboid.z = freeCuboids[i].z;
			newCuboid.width = width;
			newCuboid.height = height;
			newCuboid.depth = depth;
			newCuboid.score = score;

			possibleMoves.push_back(newCuboid);
		}
	}
	return possibleMoves;

}

vector<Cuboid> Guillotine3d::insertVector(const std::vector<Cuboid>& cuboids,
		FreeCuboidChoiceHeuristic cuboidChoice,
		GuillotineSplitHeuristic splitMethod)
{
    vector<Cuboid> foundPlaces;
    for (Cuboid c : cuboids)
    {
    	Cuboid place = insert(c, Guillotine3d::CuboidMinHeight,
    			Guillotine3d::SplitLongerAxis);
    	if (place.isPlaced == true)
    	{
    	    c.x = place.x + (0.5 * place.width);
    	    c.z = place.z + (0.5 * place.depth);
    	    c.y = place.y + 0.5 * place.height;
    	    c.width = place.width;
    	    c.height = place.height;
    	    c.depth = place.depth;
    		foundPlaces.push_back(c);
    	}
    	else
    	{
    		cout << "Place not found!:" << endl;
        	cout << "Width: " << c.width << endl;
        	cout << "Height: " << c.depth<< endl;

    	}
    }
    return foundPlaces;
}

std::vector<Cuboid> Guillotine3d::insertBestGlobalVector(
		std::vector<Cuboid> cuboids, GuillotineSplitHeuristic splitMethod)
{
    vector<Cuboid> foundPlaces;
    int size = cuboids.size();
    for (int i = 0; i < size; ++i)
    {
    	Cuboid place = insertBestGlobal(cuboids, *this, Guillotine3d::SplitLongerAxis);
    	Cuboid c;
    	if (place.isPlaced == true)
    	{
        	place = insert(place,
					Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
    		foundPlaces.push_back(place);
    	}
    	else
    	{
    		cout << "Place not found!:" << endl;
        	cout << "Width: " << c.width << endl;
        	cout << "Height: " << c.depth<< endl;

    	}

    	int index;
    	for (unsigned j = 0; j < cuboids.size(); j++)
    	{
    		if (cuboids[j].width == place.width &&
    			cuboids[j].height == place.height &&
    			cuboids[j].depth == place.depth)
    		{
    			index = j;
    			break;
    		}
    	}
    	cuboids.erase(cuboids.begin() + index);
    }
    return foundPlaces;
}

void Guillotine3d::splitFreeCuboidAlongAxis(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, bool splitHorizontal)
{
	/* Form the three new cuboids. Two of them (bottom and right) are placed on
	 * the same level as placed cuboid and their height is the same as for
	 * placedCuboid. The third one (top) is placed on top of bottom and right
	 * cuboid and has a base area the same as freeCuboid base. */

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

	// Add new free cuboids.
	freeCuboids.push_back(bottom);
	freeCuboids.push_back(right);
	freeCuboids.push_back(top);

}
