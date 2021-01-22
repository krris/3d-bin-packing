//============================================================================
// Name        : guillotine3d.cpp
// Author      : krris 
// Version     : 1.0
//============================================================================


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
	float width = cuboid.width;
	float height = cuboid.height;
	float depth = cuboid.depth;
	Cuboid bestNode;

	float bestScore = std::numeric_limits<float>::max();

	// Try each free cuboid to find the best one for placement a given cuboid.
	// Rotate a cuboid in every possible way and find which choice is the best.

	for (unsigned i = 0; i < freeCuboids.size(); ++i)
	{
		// Width x Height x Depth
		if (width <= freeCuboids[i].width &&
			height <= freeCuboids[i].height &&
			depth <= freeCuboids[i].depth)
		{
			float score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

		// Width x Depth x Height
		if (width <= freeCuboids[i].width &&
			depth <= freeCuboids[i].height &&
			height <= freeCuboids[i].depth)
		{
			float score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

		// Depth x Height x Width
		if (depth <= freeCuboids[i].width &&
			height <= freeCuboids[i].height &&
			width <= freeCuboids[i].depth)
		{
			float score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

		// Depth x Width x Height
		if (depth <= freeCuboids[i].width &&
			width <= freeCuboids[i].height &&
			height <= freeCuboids[i].depth)
		{
			float score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

		// Height x Width x Depth
		if (height <= freeCuboids[i].width &&
			width <= freeCuboids[i].height &&
			depth <= freeCuboids[i].depth)
		{
			float score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

		// Height x Depth x Width
		if (height <= freeCuboids[i].width &&
			depth <= freeCuboids[i].height &&
			width <= freeCuboids[i].depth)
		{
			float score = scoreByHeuristic(cuboid, freeCuboids[i], cuboidChoice);
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

float Guillotine3d::scoreByHeuristic(const Cuboid& cuboid,
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

float Guillotine3d::scoreMinHeight(const Cuboid& cuboid, const Cuboid& freeCuboid)
{
	return freeCuboid.y + cuboid.height;
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

vector<Cuboid> Guillotine3d::insertVector(const std::vector<Cuboid>& cuboids,
		FreeCuboidChoiceHeuristic cuboidChoice,
		GuillotineSplitHeuristic splitMethod)
{
    vector<Cuboid> foundPlaces;
    for (Cuboid c : cuboids)
    {
    	Cuboid place = insert(c, cuboidChoice, splitMethod);
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

void Guillotine3d::insertBestGlobal(std::vector<Cuboid> cuboids,
		FreeCuboidChoiceHeuristic cuboidChoice,
		GuillotineSplitHeuristic splitMethod)
{
	sort(cuboids.begin(), cuboids.end());
	int bestScore = numeric_limits<int>::max();
	while(next_permutation(cuboids.begin(), cuboids.end()))
	{
		Guillotine3d guillotine(this->binWidth, this->binDepth);
		guillotine.insertVector(cuboids,
				cuboidChoice, splitMethod);
		int score = guillotine.getFilledBinHeight();
		if (score < bestScore)
		{
			bestScore = score;
			this->freeCuboids = guillotine.getFreeCuboids();
			this->usedCuboids = guillotine.getUsedCuboids();
		}
	}
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
		top.height = freeCuboid.height - placedCuboid.height;
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
