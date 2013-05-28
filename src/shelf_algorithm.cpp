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
	Rect fits = shelf.guillotine.fits(middle, min, Guillotine2d::RectBestAreaFit);
	if (shelf.height > 0)
		if (max < shelf.height)
			fits.isPlaced = false;
	if (fits.isPlaced)
	{
		Cuboid c(fits.width, max, fits.height);
		c.isPlaced = true;
		return c;
	}

	// Set cuboid's second longest egde vertically
	fits = shelf.guillotine.fits(min, max, Guillotine2d::RectBestAreaFit);
	if (shelf.height > 0)
		if (middle < shelf.height)
			fits.isPlaced = false;
	if (fits.isPlaced)
	{
		Cuboid c(fits.width, middle, fits.height);
		c.isPlaced = true;
		return c;
	}

	// Set cuboid's smallest egde vertically
	fits = shelf.guillotine.fits(middle, max, Guillotine2d::RectBestAreaFit);
	if (shelf.height > 0)
		if (min < shelf.height)
			fits.isPlaced = false;
	if (fits.isPlaced)
	{
		Cuboid c (fits.width, min, fits.height);
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

	if (newCuboid.isPlaced == true)
		usedCuboids.push_back(newCuboid);

	// Grow the shelf height.
	shelf.height = max(shelf.height, newCuboid.height);
	assert(shelf.height <= binHeight);

}

bool ShelfAlgorithm::canStartNewShelf(int height) const
{
	return shelves.back().startY + shelves.back().height + height < binHeight;
}

int ShelfAlgorithm::getFilledBinHeight()
{
	int max = -1;
	for (Cuboid c : usedCuboids)
	{
		int height = c.y + c.height;
		if (height > max)
			max = height;
	}
	return max;
}

std::vector<Cuboid> ShelfAlgorithm::insert(std::vector<Cuboid> cuboids, ShelfChoiceHeuristic choice)
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
