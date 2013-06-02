//============================================================================
// Name        : shelf_algorihtm.cpp
// Author      : krris 
// Version     : 1.0
//============================================================================

#include <cassert>

#include "../include/shelf_algorithm.hpp"

using namespace std;

ShelfAlgorithm::ShelfAlgorithm(float width, float depth)
{
	init(width,  depth);
}

void ShelfAlgorithm::init(float width, float depth)
{
	binWidth = width;
	binHeight = numeric_limits<float>::max();
	binDepth = depth;

	currentY = 0;

	shelves.clear();
	startNewShelf(0);
}

Cuboid ShelfAlgorithm::insert(const Cuboid& cuboid, ShelfChoiceHeuristic method)
{
	Cuboid fittingCuboid;
	switch(method)
	{
	case ShelfNextFit:
		fittingCuboid = putOnShelf(shelves.back(), cuboid);
		if (fittingCuboid.isPlaced == true)
		{
			addToShelf(shelves.back(), cuboid);
			return fittingCuboid;
		}
		break;

	case ShelfFirstFit:
		for(size_t i = 0; i < shelves.size(); ++i)
		{
			fittingCuboid = putOnShelf(shelves[i], cuboid);
			if (fittingCuboid.isPlaced == true)
			{
				addToShelf(shelves[i], fittingCuboid);
				return fittingCuboid;
			}
		}
		break;
	}

	// The rectangle did not fit on any of the shelves. Open a new shelf.

	// Sort edges in decreasing order
	vector<float> edges = {cuboid.width, cuboid.height, cuboid.depth};
	sort(edges.begin(), edges.end());
	float max = edges[2];
	float middle = edges[1];
	float min = edges[0];
	Cuboid newCuboid(middle, max, min);

	if (canStartNewShelf(newCuboid.height))
	{
		startNewShelf(newCuboid.height);
		auto fits = putOnShelf(shelves.back(), newCuboid);
		assert(fits.isPlaced);
		addToShelf(shelves.back(), fits);
		return fits;
	}

	// The rectangle didn't fit.
	return cuboid;
}

Cuboid ShelfAlgorithm::putOnShelf(Shelf& shelf, const Cuboid& cuboid)
{
	float width = cuboid.width;
	float height = cuboid.height;
	float depth = cuboid.depth;
	Cuboid cuboidNotPlaced;
	cuboidNotPlaced.isPlaced = false;

	// Sort edges in decreasing order
	vector<float> edges = {width, height, depth};
	sort(edges.begin(), edges.end());
	float max = edges[2];
	float middle = edges[1];
	float min = edges[0];

	// Set cuboid's longest egde vertically
	if (shelf.height > 0 && max > shelf.height)
	{
		// pass
	}
	else
	{
		Rect maxVerticalRect(middle, min);
		Rect placement = shelf.guillotine.insert(maxVerticalRect,
				Guillotine2d::RectBestAreaFit,
				Guillotine2d::SplitLongerLeftoverAxis);
		if (placement.isPlaced)
		{
			Cuboid c(placement.width, max, placement.height);
			shelf.guillotine.insertOnPosition(placement,
					Guillotine2d::SplitLongerLeftoverAxis);
			c.isPlaced = true;
			c.x = placement.x;
			c.z = placement.y;
			return c;
		}
	}


	// Set cuboid's second longest egde vertically

	if (shelf.height > 0 && middle > shelf.height)
	{
		// pass
	}
	else
	{
		Rect middleVerticalRect(min, max);
		Rect placement = shelf.guillotine.insert(middleVerticalRect,
				Guillotine2d::RectBestAreaFit,
				Guillotine2d::SplitLongerLeftoverAxis);
		if (placement.isPlaced)
		{

			Cuboid c(placement.width, middle, placement.height);
			shelf.guillotine.insertOnPosition(placement,
					Guillotine2d::SplitLongerLeftoverAxis);
			c.isPlaced = true;
			c.x = placement.x;
			c.z = placement.y;
			return c;
		}
	}

	// Set cuboid's smallest egde vertically

	if (shelf.height > 0 && min > shelf.height)
	{
		// pass
	}
	else
	{
		Rect minVerticalRect(middle, max);
		Rect placement = shelf.guillotine.insert(minVerticalRect,
				Guillotine2d::RectBestAreaFit,
				Guillotine2d::SplitLongerLeftoverAxis);
		if (placement.isPlaced)
		{
			Cuboid c (placement.width, min, placement.height);
			shelf.guillotine.insertOnPosition(placement,
					Guillotine2d::SplitLongerLeftoverAxis);
			c.isPlaced = true;
			c.x = placement.x;
			c.z = placement.y;
			return c;
		}
	}


	return cuboidNotPlaced;
}

void ShelfAlgorithm::addToShelf(Shelf& shelf, Cuboid newCuboid)
{
	// Add the cuboid to the shelf.
	newCuboid.y = shelf.startY;

	assert(newCuboid.isPlaced);
	usedCuboids.push_back(newCuboid);

	// Grow the shelf height.
	shelf.height = max(shelf.height, newCuboid.height);
	assert(shelf.height <= binHeight);

}

bool ShelfAlgorithm::canStartNewShelf(float height) const
{
	return shelves.back().startY + shelves.back().height + height < binHeight;
}

float ShelfAlgorithm::getFilledBinHeight()
{
	float max = -1;
	for (Cuboid c : usedCuboids)
	{
		float height = c.y + c.height;
		if (height > max)
			max = height;
	}
	return max;
}

std::vector<Cuboid> ShelfAlgorithm::insert(const std::vector<Cuboid>& cuboids,
		ShelfChoiceHeuristic choice)
{
	vector<Cuboid> placedCuboids;
    for (Cuboid c : cuboids)
    {
    	Cuboid placed = insert(c, choice);
    	if (placed.isPlaced == true)
    	{
    		placedCuboids.push_back(placed);
    	}
    	else
    	{
    		cout << "Place not found!:" << endl;
        	cout << "Width: " << c.width << endl;
        	cout << "Height: " << c.height<< endl;
        	cout << "Depth: " << c.depth<< endl;

    	}
    }
    return placedCuboids;
}

void ShelfAlgorithm::startNewShelf(float startingHeight)
{
	if (shelves.size() > 0)
	{
		assert(shelves.back().height != 0);
		currentY += shelves.back().height;
		assert(currentY < binHeight);
	}

	Shelf shelf;
	shelf.height = startingHeight;
	shelf.startY = currentY;

	shelf.guillotine.init(binWidth, binDepth);

	assert(shelf.startY + shelf.height <= binHeight);
	shelves.push_back(shelf);
}
