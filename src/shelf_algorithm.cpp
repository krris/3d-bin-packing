#include <cassert>

#include "../include/shelf_algorithm.hpp"

using namespace std;

ShelfAlgorithm::ShelfAlgorithm(int width, int height, int depth)
{
	init(width, height, depth);
}

void ShelfAlgorithm::init(int width, int height, int depth)
{
	binWidth = width;
	binHeight = height;
	binDepth = depth;

	currentY = 0;
	usedSurfaceArea = 0;

	shelves.clear();
	startNewShelf(0);
}

Cuboid ShelfAlgorithm::insert(Cuboid cuboid, ShelfChoiceHeuristic method)
{
	Cuboid newNode;
	int width = cuboid.width;
	int height = cuboid.height;
	int depth = cuboid.depth;


	switch(method)
	{
	case ShelfNextFit:
/*		if (fitsOnShelf(shelves.back(), width, height, depth, true))
		{
			addToShelf(shelves.back(), width, height, depth, newNode);
			return newNode;
		}
		break; */
	case ShelfFirstFit:

		for(size_t i = 0; i < shelves.size(); ++i)
		{
			auto fits = fitsOnShelf(shelves[i], cuboid, i == shelves.size()-1);
			if (get<0>(fits) == true)
			{
				int width_ = get<1>(fits);
				int depth_ = get<2>(fits);
				int height_ = get<3>(fits);
				addToShelf(shelves[i], width_, height_, depth_, newNode);
				return newNode;
			}
		}
		break;
	}

	// The rectangle did not fit on any of the shelves. Open a new shelf.

	// Flip the rectangle so that the long side is horizontal.
	if (width < height && height <= binWidth)
		swap(width, height);

	if (canStartNewShelf(height))
	{
		startNewShelf(height);
		auto fits = fitsOnShelf(shelves.back(), cuboid, true);
		assert(get<0>(fits));
		addToShelf(shelves.back(), width, height, depth, newNode);
		return newNode;
	}

	// The rectangle didn't fit.
	return newNode;
}

tuple<bool, int, int, int> ShelfAlgorithm::fitsOnShelf(const Shelf& shelf, Cuboid cuboid,
		bool canResize) const
{
	//const int shelfHeight = canResize ? (binHeight - shelf.startY) : shelf.height;
	int width = cuboid.width;
	int height = cuboid.height;
	int depth = cuboid.depth;

	// Check if there is a place on current shelf

	vector<int> edges = {width, height, depth};
	sort(edges.begin(), edges.end());
	int max = edges[2];
	int middle = edges[1];
	int min = edges[0];

	bool fits;
	// Set cuboid's longest egde vertically
	fits = shelf.guillotine.fits(middle, min, Guillotine2d::RectBestAreaFit);
	if (fits)
		return make_tuple(true, middle, min, max);

	// Set cuboid's second longest egde vertically
	fits = shelf.guillotine.fits(min, max, Guillotine2d::RectBestAreaFit);
	if (fits)
		return make_tuple(true, min, max, middle);

	// Set cuboid's smallest egde vertically
	fits = shelf.guillotine.fits(middle, max, Guillotine2d::RectBestAreaFit);
	if (fits)
		return make_tuple(true, middle, max, min);

	return make_tuple(false, 0, 0, 0);
}

/*void ShelfAlgorithm::rotateToShelf(const Shelf& shelf, int width,
		int height) const
{
	if ((width > height && width > binWidth - shelf.currentX) ||
			(width > height && width < shelf.height) ||
			(width < height && height > shelf.height && height <= binWidth - shelf.currentX))
			swap(width, height);

}*/

void ShelfAlgorithm::addToShelf(Shelf& shelf, int width, int height, int depth,
		Cuboid& newCuboid)
{
	// Swap width and height if the rect fits better that way.
	//rotateToShelf(shelf, width, height);

	// Add the rectangle to the shelf.
	//newRect.x = shelf.currentX;
	newCuboid.y = shelf.startY;
	newCuboid.width = width;
	newCuboid.height = height;
	newCuboid.depth = depth;

	Rect cuboidBase = shelf.guillotine.insert(width, depth, Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);

	newCuboid.x = cuboidBase.x;
	newCuboid.z = cuboidBase.y;

	newCuboid.isPlaced = cuboidBase.isPlaced;

	// Advance the shelf end position horizontally.
	//shelf.currentX += width;
	//assert(shelf.currentX <= binWidth);

	// Grow the shelf height.
	shelf.height = max(shelf.height, height);
	assert(shelf.height <= binHeight);

	usedSurfaceArea += width * height;
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
//	shelf.currentX = 0;
	shelf.height = startingHeight;
	shelf.startY = currentY;

	shelf.guillotine.init(binWidth, binDepth);

	assert(shelf.startY + shelf.height <= binHeight);
	shelves.push_back(shelf);
}
