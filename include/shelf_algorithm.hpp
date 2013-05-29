#ifndef SHELF_ALGORITHM_HPP
#define SHELF_ALGORITHM_HPP

#include "forward_declarations.hpp"
#include "rect.hpp"
#include "cuboid.hpp"
#include "guillotine2d.hpp"


class ShelfAlgorithm{
public:
	ShelfAlgorithm() : binWidth(0), binHeight(std::numeric_limits<int>::max()),
		binDepth(0), currentY(0) {}
	ShelfAlgorithm(int width, int depth);

	void init(int width, int depth);
	enum ShelfChoiceHeuristic
	{
		ShelfNextFit, // We always put the new cuboid to the last open shelf.
		ShelfFirstFit, // We test each cuboid against each shelf in turn and
					   // pack it to the first where it fits.
	};

	Cuboid insert(const Cuboid& cuboid, ShelfChoiceHeuristic method);
	std::vector<Cuboid> insert(const std::vector<Cuboid>& cuboids,
			ShelfChoiceHeuristic method);

	int getFilledBinHeight();
	std::vector<Cuboid> getUsedCuboids() { return usedCuboids; }


private:
	int binWidth;
	int binHeight;
	int binDepth;

	/* Stores the starting y-coordinate of the latest(topmost) shelf. */
	int currentY;

	struct Shelf
	{
		int startY;
		int height;
		Guillotine2d guillotine;
	};

	std::vector<Shelf> shelves;
	std::vector<Cuboid> usedCuboids;

	/**
	 * Check if cuboid fits on shelf. If it fits, cuboid.isPlaced will be set
	 * to true.
	 */
	Cuboid fitsOnShelf(const Shelf& shelf, Cuboid cuboid) const;

	void addToShelf(Shelf& shelf, Cuboid newCuboid);

	/**
	 * Check if is it possible to start new shelf.
	 */
	bool canStartNewShelf(int height) const;

	void startNewShelf(int startingHeight);
    
};

#endif
