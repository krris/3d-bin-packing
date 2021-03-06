//============================================================================
// Name        : guillotine2d.hpp
// Author      : krris 
// Version     : 1.0
//============================================================================

#ifndef GUILLOTINE2D_HPP_
#define GUILLOTINE2D_HPP_

#include <vector>

#include "rect.hpp"

/**
 * 2D Guillotine bin packing - algorithm for packing rectangles.
 */
class Guillotine2d {
public:
	/* Set the initial bin size to (0, 0).
	 * Calls init() to set a proper bin size. */
	Guillotine2d() : binWidth(0), binHeight(0) {}

	/* Initialize a new bin */
	Guillotine2d(int width, int height);

	/* Initialize an empty bin. */
	void init(int width, int height);

	enum FreeRectChoiceHeuristic
		{
			RectBestAreaFit,
			RectBestShortSideFit,
		};

	enum GuillotineSplitHeuristic
	{
		SplitShorterLeftoverAxis,
		SplitLongerLeftoverAxis,
		SplitShorterAxis,
		SplitLongerAxis
	};

	Rect insert(const Rect& rect, FreeRectChoiceHeuristic rectChoice,
				GuillotineSplitHeuristic splitMethod);

	void insertOnPosition(const Rect& rect, GuillotineSplitHeuristic splitMethod);


	std::vector<Rect>& getFreeRectangles() { return freeRectangles; }

	std::vector<Rect>& getUsedRectangles() { return usedRectangles; }

	bool isEmpty() const { return usedRectangles.empty();}

private:
	int binWidth;
	int binHeight;

	/* Stores rectangles that are packed so far */
	std::vector<Rect> usedRectangles;

	/* Stores rectangles that represents the free area of the bin; */
	std::vector<Rect> freeRectangles;

	/**
	 * Search through all free rectangles to find the best one to place
	 * a new rectangle.
	 */
	Rect findPositionForNewRect(const Rect& rect,
			FreeRectChoiceHeuristic rectChoice) const;



	static int scoreByHeuristic(const Rect& rect, const Rect &freeRect,
								FreeRectChoiceHeuristic rectChoice);

	static int scoreBestAreaFit(const Rect& rect, const Rect &freeRect);
	static int scoreBestShortSideFit(const Rect& rect, const Rect &freeRect);

	/* Splits the given L-shaped free rectangle into two new free rectangles
	 * after placedRect has been placed into it. Determines the split axis by
	 * using the given heuristic. */
	void splitFreeRectByHeuristic(const Rect& freeRect, const Rect& placedRect,
								  GuillotineSplitHeuristic method);

	/* Splits the given L-shaped free rectangle into two new free rectangles
	 * along the given fixed split axis. */
	void splitFreeRectAlongAxis(const Rect& freeRect, const Rect& placedRect,
								bool splitHorizontal);
};


#endif /* GUILLOTINE2D_HPP_ */
