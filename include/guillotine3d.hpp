//============================================================================
// Name        : guillotine3d.hpp
// Author      : krris 
// Version     : 1.0
//============================================================================

#ifndef GUILLOTINE3D_HPP_
#define GUILLOTINE3D_HPP_

#include <vector>

#include "cuboid.hpp"

/**
 * 3D Guillotine bin packing - packing cuboids into a bin.
 */
class Guillotine3d {
public:
	/* Initialize a new bin. Set the base area of cuboid. */
	Guillotine3d(int width, int depth);

	/* Initialize an empty bin. */
	void init(int width, int depth);

	enum FreeCuboidChoiceHeuristic
		{
			CuboidMinHeight
		};

	enum GuillotineSplitHeuristic
	{
		SplitShorterLeftoverAxis,
		SplitLongerLeftoverAxis,
		SplitShorterAxis,
		SplitLongerAxis
	};

	/**
	 * Insert a cuboid with a given size.
	 * @return Cuboid which has  cooridantes, where will be placed in a bin.
	 */
	Cuboid insert(const Cuboid& cuboid, FreeCuboidChoiceHeuristic cuboidChoice,
			GuillotineSplitHeuristic splitMethod);

	std::vector<Cuboid> insertVector(const std::vector<Cuboid>& cuboids,
			FreeCuboidChoiceHeuristic cuboidChoice,
			GuillotineSplitHeuristic splitMethod);

	void insertBestGlobal(std::vector<Cuboid> cuboids,
			FreeCuboidChoiceHeuristic cuboidChoice,
			GuillotineSplitHeuristic splitMethod);

	std::vector<Cuboid>& getFreeCuboids() { return freeCuboids; }

	std::vector<Cuboid>& getUsedCuboids() { return usedCuboids; }

	int getFilledBinHeight();

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
	Cuboid findPositionForNewNode(const Cuboid& cuboid,
			FreeCuboidChoiceHeuristic cuboidChoice,
			int *nodeIndex) const;

	static float scoreByHeuristic(const Cuboid& cuboid, const Cuboid& freeCuboid,
			FreeCuboidChoiceHeuristic cuboidChoice);

	static float scoreMinHeight(const Cuboid& cuboid, const Cuboid& freeCuboid);

	/* Splits the given L-shaped free rectangle into two new free rectangles
	 * after placedCuboid has been placed into it. Determines the split axis by
	 * using the given heuristic. */
	void splitFreeCuboidByHeuristic(const Cuboid& freeCuboid,
			const Cuboid& placedCuboid,
			GuillotineSplitHeuristic method);

	/* Splits the given L-shaped free rectangle into two new free rectangles
	 * along the given fixed split axis. */
	void splitFreeCuboidAlongAxis(const Cuboid &freeCuboid,
			const Cuboid &placedCuboid,
			bool splitHorizontal);
};

#endif /* GUILLOTINE3D_HPP_ */
