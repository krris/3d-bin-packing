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
		fittingCuboid = fitsOnShelf(shelves.back(), cuboid);
		if (fittingCuboid.isPlaced == true)
		{
			addToShelf(shelves.back(), cuboid);
			return fittingCuboid;
		}
		break;

	case ShelfFirstFit:
		for(size_t i = 0; i < shelves.size(); ++i)
		{
			fittingCuboid = fitsOnShelf(shelves[i], cuboid);
			{
			if (fittingCuboid.isPlaced == true)
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
		auto fits = fitsOnShelf(shelves.back(), newCuboid);
		assert(fits.isPlaced);
		addToShelf(shelves.back(), fits);
		return fits;
	}

	// The rectangle didn't fit.
	return cuboid;
}

Cuboid ShelfAlgorithm::fitsOnShelf(const Shelf& shelf, Cuboid cuboid) const
{
	float width = cuboid.width;
	float height = cuboid.height;
	float depth = cuboid.depth;

	// Sort edges in decreasing order
	vector<float> edges = {width, height, depth};
	sort(edges.begin(), edges.end());
	float max = edges[2];
	float middle = edges[1];
	float min = edges[0];

	// Set cuboid's longest egde vertically
	Rect placement = shelf.guillotine.fits(middle, min, Guillotine2d::RectBestAreaFit);
	if (shelf.height > 0)
		if (max > shelf.height)
			placement.isPlaced = false;
	if (placement.isPlaced)
	{
		Cuboid c(placement.width, max, placement.height);
		c.isPlaced = true;
		return c;
	}

	// Set cuboid's second longest egde vertically
	placement = shelf.guillotine.fits(min, max, Guillotine2d::RectBestAreaFit);
	if (shelf.height > 0)
		if (middle > shelf.height)
			placement.isPlaced = false;
	if (placement.isPlaced)
	{
		Cuboid c(placement.width, middle, placement.height);
		c.isPlaced = true;
		return c;
	}

	// Set cuboid's smallest egde vertically
	placement = shelf.guillotine.fits(middle, max, Guillotine2d::RectBestAreaFit);
	if (shelf.height > 0)
		if (min > shelf.height)
			placement.isPlaced = false;
	if (placement.isPlaced)
	{
		Cuboid c (placement.width, min, placement.height);
		c.isPlaced = true;
		return c;
	}

	cuboid.isPlaced = false;
	return cuboid;
}

void ShelfAlgorithm::addToShelf(Shelf& shelf, Cuboid newCuboid)
{
	// Add the cuboid to the shelf.
	newCuboid.y = shelf.startY;

	Rect cuboidBase = shelf.guillotine.insert(newCuboid.width, newCuboid.depth,
			Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);

	newCuboid.x = cuboidBase.x;
	newCuboid.z = cuboidBase.y;

	newCuboid.isPlaced = cuboidBase.isPlaced;

	if (newCuboid.isPlaced == true)
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
