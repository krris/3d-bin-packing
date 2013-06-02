//============================================================================
// Name        : ishelf_algorithm.cpp
// Author      : krris 
// Version     : 1.0
//============================================================================
#ifndef SHELF_ALGORITHM_HPP
#define SHELF_ALGORITHM_HPP

#include "rect.hpp"
#include "cuboid.hpp"
#include "guillotine2d.hpp"


class ShelfAlgorithm{
public:
	ShelfAlgorithm() : binWidth(0), binHeight(std::numeric_limits<float>::max()),
		binDepth(0), currentY(0) {}
	ShelfAlgorithm(float width, float depth);

	void init(float width, float depth);
	enum ShelfChoiceHeuristic
	{
		ShelfNextFit, // We always put the new cuboid to the last open shelf.
		ShelfFirstFit, // We test each cuboid against each shelf in turn and
					   // pack it to the first where it fits.
	};

	Cuboid insert(const Cuboid& cuboid, ShelfChoiceHeuristic method);
	std::vector<Cuboid> insert(const std::vector<Cuboid>& cuboids,
			ShelfChoiceHeuristic method);

	float getFilledBinHeight();
	std::vector<Cuboid> getUsedCuboids() { return usedCuboids; }


private:
	float binWidth;
	float binHeight;
	float binDepth;

	/* Stores the starting y-coordinate of the latest(topmost) shelf. */
	float currentY;

	struct Shelf
	{
		float startY;
		float height;
		Guillotine2d guillotine;
	};

	std::vector<Shelf> shelves;
	std::vector<Cuboid> usedCuboids;

	Cuboid putOnShelf(Shelf& shelf, const Cuboid& cuboid);

	void addToShelf(Shelf& shelf, Cuboid newCuboid);

	/**
	 * Check if is it possible to start new shelf.
	 */
	bool canStartNewShelf(float height) const;

	void startNewShelf(float startingHeight);
    
};

#endif
