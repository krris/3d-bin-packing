#include <cassert>

#include "../include/shelf_algorithm.hpp"

using namespace std;

ShelfAlgorithm::ShelfAlgorithm(int width, int height)
{
	init(width, height);
}

void ShelfAlgorithm::init(int width, int height)
{
	binWidth = width;
	binHeight = height;

	currentY = 0;
	usedSurfaceArea = 0;

	shelves.clear();
	startNewShelf(0);
}

Rect ShelfAlgorithm::insert(int width, int height, ShelfChoiceHeuristic method)
{
	Rect newNode;


	switch(method)
	{
	case ShelfNextFit:
		if (fitsOnShelf(shelves.back(), width, height, true))
		{
			addToShelf(shelves.back(), width, height, newNode);
			return newNode;
		}
		break;
	case ShelfFirstFit:
		for(size_t i = 0; i < shelves.size(); ++i)
			if (fitsOnShelf(shelves[i], width, height, i == shelves.size()-1))
			{
				addToShelf(shelves[i], width, height, newNode);
				return newNode;
			}
		break;

	case ShelfBestAreaFit:
		{
			// Best Area Fit rule: Choose the shelf with smallest remaining shelf area.
			Shelf *bestShelf = 0;
			unsigned long bestShelfSurfaceArea = (unsigned long)-1;
			for(size_t i = 0; i < shelves.size(); ++i)
			{
				// Pre-rotate the rect onto the shelf here already so that the area fit computation
				// is done correctly.
				rotateToShelf(shelves[i], width, height);
				if (fitsOnShelf(shelves[i], width, height, i == shelves.size()-1))
				{
					unsigned long surfaceArea = (binWidth - shelves[i].currentX) * shelves[i].height;
					if (surfaceArea < bestShelfSurfaceArea)
					{
						bestShelf = &shelves[i];
						bestShelfSurfaceArea = surfaceArea;
					}
				}
			}

			if (bestShelf)
			{
				addToShelf(*bestShelf, width, height, newNode);
				return newNode;
			}
		}
		break;

	case ShelfWorstAreaFit:
		{
			// Worst Area Fit rule: Choose the shelf with smallest remaining shelf area.
			Shelf *bestShelf = 0;
			int bestShelfSurfaceArea = -1;
			for(size_t i = 0; i < shelves.size(); ++i)
			{
				// Pre-rotate the rect onto the shelf here already so that the area fit computation
				// is done correctly.
				rotateToShelf(shelves[i], width, height);
				if (fitsOnShelf(shelves[i], width, height, i == shelves.size()-1))
				{
					int surfaceArea = (binWidth - shelves[i].currentX) * shelves[i].height;
					if (surfaceArea > bestShelfSurfaceArea)
					{
						bestShelf = &shelves[i];
						bestShelfSurfaceArea = surfaceArea;
					}
				}
			}

			if (bestShelf)
			{
				addToShelf(*bestShelf, width, height, newNode);
				return newNode;
			}
		}
		break;

	case ShelfBestHeightFit:
		{
			// Best Height Fit rule: Choose the shelf with best-matching height.
			Shelf *bestShelf = 0;
			int bestShelfHeightDifference = 0x7FFFFFFF;
			for(size_t i = 0; i < shelves.size(); ++i)
			{
				// Pre-rotate the rect onto the shelf here already so that the height fit computation
				// is done correctly.
				rotateToShelf(shelves[i], width, height);
				if (fitsOnShelf(shelves[i], width, height, i == shelves.size()-1))
				{
					int heightDifference = max(shelves[i].height - height, 0);
					assert(heightDifference >= 0);

					if (heightDifference < bestShelfHeightDifference)
					{
						bestShelf = &shelves[i];
						bestShelfHeightDifference = heightDifference;
					}
				}
			}

			if (bestShelf)
			{
				addToShelf(*bestShelf, width, height, newNode);
				return newNode;
			}
		}
		break;

	case ShelfBestWidthFit:
		{
			// Best Width Fit rule: Choose the shelf with smallest remaining shelf width.
			Shelf *bestShelf = 0;
			int bestShelfWidthDifference = 0x7FFFFFFF;
			for(size_t i = 0; i < shelves.size(); ++i)
			{
				// Pre-rotate the rect onto the shelf here already so that the height fit computation
				// is done correctly.
				rotateToShelf(shelves[i], width, height);
				if (fitsOnShelf(shelves[i], width, height, i == shelves.size()-1))
				{
					int widthDifference = binWidth - shelves[i].currentX - width;
					assert(widthDifference >= 0);

					if (widthDifference < bestShelfWidthDifference)
					{
						bestShelf = &shelves[i];
						bestShelfWidthDifference = widthDifference;
					}
				}
			}

			if (bestShelf)
			{
				addToShelf(*bestShelf, width, height, newNode);
				return newNode;
			}
		}
		break;

	case ShelfWorstWidthFit:
		{
			// Worst Width Fit rule: Choose the shelf with smallest remaining shelf width.
			Shelf *bestShelf = 0;
			int bestShelfWidthDifference = -1;
			for(size_t i = 0; i < shelves.size(); ++i)
			{
				// Pre-rotate the rect onto the shelf here already so that the height fit computation
				// is done correctly.
				rotateToShelf(shelves[i], width, height);
				if (fitsOnShelf(shelves[i], width, height, i == shelves.size()-1))
				{
					int widthDifference = binWidth - shelves[i].currentX - width;
					assert(widthDifference >= 0);

					if (widthDifference > bestShelfWidthDifference)
					{
						bestShelf = &shelves[i];
						bestShelfWidthDifference = widthDifference;
					}
				}
			}

			if (bestShelf)
			{
				addToShelf(*bestShelf, width, height, newNode);
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
		assert(fitsOnShelf(shelves.back(), width, height, true));
		addToShelf(shelves.back(), width, height, newNode);
		return newNode;
	}

	// The rectangle didn't fit.
	//memset(&newNode, 0, sizeof(Rect));
	return newNode;
}

bool ShelfAlgorithm::fitsOnShelf(const Shelf& shelf, int width, int height,
		bool canResize) const
{
	const int shelfHeight = canResize ? (binHeight - shelf.startY) : shelf.height;
	if ((shelf.currentX + width <= binWidth && height <= shelfHeight) ||
		(shelf.currentX + height <= binWidth && width <= shelfHeight))
		return true;
	else
		return false;
}

void ShelfAlgorithm::rotateToShelf(const Shelf& shelf, int width,
		int height) const
{
	if ((width > height && width > binWidth - shelf.currentX) ||
			(width > height && width < shelf.height) ||
			(width < height && height > shelf.height && height <= binWidth - shelf.currentX))
			swap(width, height);
}

void ShelfAlgorithm::addToShelf(Shelf& shelf, int width, int height,
		Rect& newNode)
{
	// Swap width and height if the rect fits better that way.
	rotateToShelf(shelf, width, height);

	// Add the rectangle to the shelf.
	newNode.x = shelf.currentX;
	newNode.y = shelf.startY;
	newNode.width = width;
	newNode.height = height;
	shelf.usedRectangles.push_back(newNode);

	// Advance the shelf end position horizontally.
	shelf.currentX += width;
	assert(shelf.currentX <= binWidth);

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
		currentY =+ shelves.back().height;
		assert(currentY < binHeight);
	}

	Shelf shelf;
	shelf.currentX = 0;
	shelf.height = startingHeight;
	shelf.startY = currentY;

	assert(shelf.startY + shelf.height <= binHeight);
	shelves.push_back(shelf);
}
