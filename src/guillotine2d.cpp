/*
 * 2dGuillotine.cpp
 *
 *  Created on: 24-05-2013
 *      Author: krris
 */

//#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include "../include/guillotine2d.hpp"


Guillotine2d::Guillotine2d(int width, int height)
{
	init(width, height);
}

void Guillotine2d::init(int width, int height)
{
	binWidth = width;
	binHeight = height;

	usedRectangles.clear();

	// Start with a single big free rectangle that spans the whole bin
	Rect n;
	n.x = 0;
	n.y = 0;
	n.width = width;
	n.height = height;

	freeRectangles.clear();
	freeRectangles.push_back(n);
}

Rect Guillotine2d::insert(int width, int height,
		FreeRectChoiceHeuristic rectChoice,
		GuillotineSplitHeuristic splitMethod)
{
	// Find where to put the new rectangle
	int freeNodeIndex = 0;
	Rect newRect = findPositionForNewNode(width, height, rectChoice, &freeNodeIndex);

	// Abort if we didn't have enough space in the bin
	if (newRect.isPlaced == false)
		return newRect;
	//if (newRect.height == 0)
	//	return newRect;

	// Remove the space that was just consumed by the new rectangle.
	splitFreeRectByHeuristic(freeRectangles[freeNodeIndex], newRect, splitMethod);
	freeRectangles.erase(freeRectangles.begin() + freeNodeIndex);

	// Remember the new used rectangle
	usedRectangles.push_back(newRect);

	return newRect;
}

void Guillotine2d::insert(std::vector<RectSize>& rects,
		FreeRectChoiceHeuristic rectChoice,
		GuillotineSplitHeuristic splitMethod)
{
}

Rect Guillotine2d::fits(int width, int height,
		FreeRectChoiceHeuristic rectChoice) const
{
	int nodeIndex;
	Rect rect = findPositionForNewNode(width, height, rectChoice, &nodeIndex);
	if (rect.isPlaced == true)
		return rect;
	else
		return rect;
}


Rect Guillotine2d::findPositionForNewNode(int width, int height,
		FreeRectChoiceHeuristic rectChoice, int* nodeIndex) const
{
	Rect bestNode;

	int bestScore = std::numeric_limits<int>::max();

	// Try each free rectangle to find the best one for placement
	for (unsigned i = 0; i < freeRectangles.size(); ++i)
	{
		// If this is a perfect fit upright, choose it immediately.
		if (width == freeRectangles[i].width && height == freeRectangles[i].height)
		{
			bestNode.isPlaced = true;
			bestNode.x = freeRectangles[i].x;
			bestNode.y = freeRectangles[i].y;
			bestNode.width = width;
			bestNode.height = height;
			bestScore = std::numeric_limits<int>::min();
			*nodeIndex = i;
			break;
		}
		// If this is a perfect fit sideways, choose it.
		else if (height == freeRectangles[i].width && width == freeRectangles[i].height)
		{
			bestNode.isPlaced = true;
			bestNode.x = freeRectangles[i].x;
			bestNode.y = freeRectangles[i].y;
			bestNode.width = height;
			bestNode.height = width;
			bestScore = std::numeric_limits<int>::min();
			*nodeIndex = i;
			break;
		}
		// Does the rectangle fit upright?
		else
		if (width <= freeRectangles[i].width && height <= freeRectangles[i].height)
		{
			int score = scoreByHeuristic(width, height, freeRectangles[i], rectChoice);

			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeRectangles[i].x;
				bestNode.y = freeRectangles[i].y;
				bestNode.width = width;
				bestNode.height = height;
				bestScore = score;
				*nodeIndex = i;
			}
		}
		// Does the rectangle fit sideways?
		else if (height <= freeRectangles[i].width && width <= freeRectangles[i].height)
		{
			int score = scoreByHeuristic(height, width, freeRectangles[i], rectChoice);

			if (score < bestScore)
			{
				bestNode.isPlaced = true;
				bestNode.x = freeRectangles[i].x;
				bestNode.y = freeRectangles[i].y;
				bestNode.width = height;
				bestNode.height = width;
				bestScore = score;
				*nodeIndex = i;
			}
		}
	}
	return bestNode;
}

int Guillotine2d::scoreByHeuristic(int width, int height, const Rect& freeRect,
		FreeRectChoiceHeuristic rectChoice)
{
	switch(rectChoice)
	{
		case RectBestAreaFit: return scoreBestAreaFit(width, height, freeRect);
		case RectBestShortSideFit: return scoreBestShortSideFit(width, height, freeRect);
		default: assert(false); return std::numeric_limits<int>::max();
	}
}

int Guillotine2d::scoreBestAreaFit(int width, int height,
		const Rect& freeRect)
{
	return freeRect.width * freeRect.height - width * height;
}

int Guillotine2d::scoreBestShortSideFit(int width, int height,
		const Rect& freeRect)
{
	int leftoverHoriz = std::abs(freeRect.width - width);
	int leftoverVert = std::abs(freeRect.height - height);
	int leftover = std::min(leftoverHoriz, leftoverVert);
	return leftover;
}

void Guillotine2d::splitFreeRectByHeuristic(const Rect &freeRect, const Rect &placedRect, GuillotineSplitHeuristic method)
{
	// Compute the lengths of the leftover area.
	const int w = freeRect.width - placedRect.width;
	const int h = freeRect.height - placedRect.height;

	// Placing placedRect into freeRect results in an L-shaped free area, which must be split into
	// two disjoint rectangles. This can be achieved with by splitting the L-shape using a single line.
	// We have two choices: horizontal or vertical.

	// Use the given heuristic to decide which choice to make.

	bool splitHorizontal;
	switch(method)
	{
	case SplitShorterLeftoverAxis:
		// Split along the shorter leftover axis.
		splitHorizontal = (w <= h);
		break;
	case SplitLongerLeftoverAxis:
		// Split along the longer leftover axis.
		splitHorizontal = (w > h);
		break;
	case SplitShorterAxis:
		// Split along the shorter total axis.
		splitHorizontal = (freeRect.width <= freeRect.height);
		break;
	case SplitLongerAxis:
		// Split along the longer total axis.
		splitHorizontal = (freeRect.width > freeRect.height);
		break;
	default:
		splitHorizontal = true;
		assert(false);
	}

	// Perform the actual split.
	splitFreeRectAlongAxis(freeRect, placedRect, splitHorizontal);
}


void Guillotine2d::splitFreeRectAlongAxis(const Rect& freeRect,
		const Rect& placedRect, bool splitHorizontal)
{
	// Form the two new rectangles.
	Rect bottom;
	bottom.x = freeRect.x;
	bottom.y = freeRect.y + placedRect.height;
	bottom.height = freeRect.height - placedRect.height;

	Rect right;
	right.x = freeRect.x + placedRect.width;
	right.y = freeRect.y;
	right.width = freeRect.width - placedRect.width;

	if (splitHorizontal)
	{
		bottom.width = freeRect.width;
		right.height = placedRect.height;
	}
	else // Split vertically
	{
		bottom.width = placedRect.width;
		right.height = freeRect.height;
	}

	// Add the new rectangles into the free rectangle pool if they weren't degenerate.
	if (bottom.width > 0 && bottom.height > 0)
		freeRectangles.push_back(bottom);
	if (right.width > 0 && right.height > 0)
		freeRectangles.push_back(right);

}
