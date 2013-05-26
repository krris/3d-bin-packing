/*
 * guillotine3d.hpp
 *
 *  Created on: 24-05-2013
 *      Author: krris
 */

#ifndef GUILLOTINE3D_HPP_
#define GUILLOTINE3D_HPP_

#include <vector>

#include "cuboid.hpp"

/**
 * 3D Guillotine bin packing.
 */
class Guillotine3d {
public:
	/* Set the initial bin size to (0, 0).
	 * Calls init() to set a proper bin size. */
	Guillotine3d() : binWidth(0), binHeight(0), binDepth(0) {}

	/* Initialize a new bin */
	Guillotine3d(int width, int height, int depth);

	/* Initialize an empty bin. */
	void init(int width, int height, int depth);

	enum FreeCuboidChoiceHeuristic
		{
			CuboidBestAreaFit, // BAF
			CuboidBestShortSideFit, // BSSF
			CuboidMinHeight
		};

	enum GuillotineSplitHeuristic
	{
		SplitShorterLeftoverAxis, // SLAS
		SplitLongerLeftoverAxis, // LLAS
		SplitMinimizeArea, // MINAS, Try to make a single big rectangle at the expense of making the other small.
		SplitMaximizeArea, // MAXAS, Try to make both remaining rectangles as even-sized as possible.
		SplitShorterAxis, // SAS
		SplitLongerAxis // LAS
	};

	Cuboid insert(int width, int height, int depth, FreeCuboidChoiceHeuristic cuboidChoice,
				GuillotineSplitHeuristic splitMethod);

	bool fits(int width, int heigh, int depth, FreeCuboidChoiceHeuristic cuboidChoice) const;


	std::vector<Cuboid>& getFreeCuboids() { return freeCuboids; }

	std::vector<Cuboid>& getUsedCuboids() { return usedCuboids; }

private:
	int binWidth;
	int binHeight;
	int binDepth;

	/* Stores rectangles that are packed so far */
	std::vector<Cuboid> usedCuboids;

	/* Stores rectangles that represents the free area of the bin; */
	std::vector<Cuboid> freeCuboids;

	/**
	 * Search through all free rectangles to find the best one to place
	 * a new rectangle.
	 */
	Cuboid findPositionForNewNode(int width, int height, int depth,
								FreeCuboidChoiceHeuristic cuboidChoice,
								int *nodeIndex) const;



	static int scoreByHeuristic(int width, int height, int depth, const Cuboid &freeCuboid,
								FreeCuboidChoiceHeuristic cuboidChoice);

	static int scoreBestAreaFit(int width, int height, int depth, const Cuboid &freeCuboid);
	static int scoreBestShortSideFit(int width, int height, int depth, const Cuboid &freeCuboid);
	static int scoreMinHeight(int width, int height, int depth, const Cuboid& freeCuboid);

	/* Splits the given L-shaped free rectangle into two new free rectangles
	 * after placedCuboid has been placed into it. Determines the split axis by
	 * using the given heuristic. */
	void splitFreeCuboidByHeuristic(const Cuboid &freeCuboid, const Cuboid &placedCuboid,
								  GuillotineSplitHeuristic method);

	/* Splits the given L-shaped free rectangle into two new free rectangles
	 * along the given fixed split axis. */
	void splitFreeCuboidAlongAxis(const Cuboid &freeCuboid, const Cuboid &placedCuboid,
								bool splitHorizontal);
};



#endif /* GUILLOTINE3D_HPP_ */
