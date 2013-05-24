/*
 * 2dGuillotine.cpp
 *
 *  Created on: 24-05-2013
 *      Author: krris
 */

#include "../include/Guillotine2d.hpp"

Guillotine2d::Guillotine2d()
{
}

Guillotine2d::Guillotine2d(int widht, int height)
{
}

void Guillotine2d::init(int width, int height)
{
}

Rect Guillotine2d::insert(int width, int height,
		FreeRectChoiceHeuristic rectChoice,
		GuillotineSplitHeuristic splitMethod)
{
}

void Guillotine2d::insert(std::vector<RectSize>& rects,
		FreeRectChoiceHeuristic rectChoice,
		GuillotineSplitHeuristic splitMethod)
{
}

Rect Guillotine2d::findPositionForNewNode(int width, int height,
		FreeRectChoiceHeuristic rectChoice, int* nodeIndex)
{
}

int Guillotine2d::scoreByHeuristic(int width, int height, const Rect& freeRect,
		FreeRectChoiceHeuristic rectChoice)
{
}

int Guillotine2d::scoreBestAreaFit(int width, int height,
		const Rect& freeRect)
{
}

int Guillotine2d::scoreBestShortSideFit(int width, int height,
		const Rect& freeRect)
{
}

void Guillotine2d::SplitFreeRectByHeuristic(const Rect& freeRect,
		const Rect& placedRect, GuillotineSplitHeuristic method)
{
}

void Guillotine2d::SplitFreeRectAlongAxis(const Rect& freeRect,
		const Rect& placedRect, bool splitHorizontal)
{
}
