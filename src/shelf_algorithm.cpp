#include <cassert>

#include "../include/shelf_algorithm.hpp"

using namespace std;

ShelfAlgorithm::ShelfAlgorithm(int width, int depth)
{
	init(width,  depth);
}

void ShelfAlgorithm::init(int width, int depth)
{
	binWidth = width;
	binHeight = numeric_limits<int>::max();
	binDepth = depth;

	currentY = 0;

	shelves.clear();
	startNewShelf(0);
}

Cuboid ShelfAlgorithm::insert(Cuboid cuboid, ShelfChoiceHeuristic method)
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
			if (fittingCuboid.isPlaced == true)
			{
				addToShelf(shelves[i], fittingCuboid);
				return fittingCuboid;
			}
		}
		break;
	}

	// The rectangle did not fit on any of the shelves. Open a new shelf.
	int width = cuboid.width;
	int height = cuboid.height;
	// Flip the rectangle so that the long side is horizontal.
	if (width < height && height <= binWidth)
		swap(width, height);

	if (canStartNewShelf(height))
	{
		startNewShelf(height);
		auto fits = fitsOnShelf(shelves.back(), cuboid);
		assert(fits.isPlaced);
		addToShelf(shelves.back(), cuboid);
		return cuboid;
	}

	// The rectangle didn't fit.
	return cuboid;
}

Cuboid ShelfAlgorithm::fitsOnShelf(const Shelf& shelf, Cuboid cuboid) const
{
	int width = cuboid.width;
	int height = cuboid.height;
	int depth = cuboid.depth;

	// Check if there is a place on current shelf
	vector<int> edges = {width, height, depth};
	sort(edges.begin(), edges.end());
	int max = edges[2];
	int middle = edges[1];
	int min = edges[0];

	// Set cuboid's longest egde vertically
	bool fits = shelf.guillotine.fits(middle, min, Guillotine2d::RectBestAreaFit);
	if (fits)
	{
		Cuboid c(middle, max, min);
		c.isPlaced = true;
		return c;
	}

	// Set cuboid's second longest egde vertically
	fits = shelf.guillotine.fits(min, max, Guillotine2d::RectBestAreaFit);
	if (fits)
	{
		Cuboid c(min, middle, max);
		c.isPlaced = true;
		return c;
	}

	// Set cuboid's smallest egde vertically
	fits = shelf.guillotine.fits(middle, max, Guillotine2d::RectBestAreaFit);
	if (fits)
	{
		Cuboid c (middle, min, max);
		c.isPlaced = true;
		return c;
	}

	cuboid.isPlaced = false;
	return cuboid;
}

void ShelfAlgorithm::addToShelf(Shelf& shelf, Cuboid& newCuboid)
{
	// Add the cuboid to the shelf.
	newCuboid.y = shelf.startY;

	Rect cuboidBase = shelf.guillotine.insert(newCuboid.width, newCuboid.depth,
			Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);

	newCuboid.x = cuboidBase.x;
	newCuboid.z = cuboidBase.y;

	newCuboid.isPlaced = cuboidBase.isPlaced;

	// Grow the shelf height.
	shelf.height = max(shelf.height, newCuboid.height);
	assert(shelf.height <= binHeight);

}

bool ShelfAlgorithm::canStartNewShelf(int height) const
{
	return shelves.back().startY + shelves.back().height + height < binHeight;
}

void ShelfAlgorithm::startNewShelf(int startingHeight)
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
