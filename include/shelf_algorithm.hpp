#ifndef SHELF_ALGORITHM_HPP
#define SHELF_ALGORITHM_HPP

#include "forward_declarations.hpp"
#include "Rect.hpp"
#include "cuboid.hpp"
#include "Guillotine2d.hpp"


class ShelfAlgorithm{
public:
	ShelfAlgorithm() : binWidth(0), binHeight(0), binDepth(0), currentY(0), usedSurfaceArea(0) {}
	ShelfAlgorithm(int width, int height, int depth);

	void init(int width, int height, int depth);
	enum ShelfChoiceHeuristic
	{
		ShelfNextFit, //NF: We always put the new rectangle to the last open shelf.
		ShelfFirstFit, //FF: We test each rectangle against each shelf in turn and pack it to the first where it fits.
	};

	Cuboid insert(int width, int heigh, int depth, ShelfChoiceHeuristic method);

private:
	int binWidth;
	int binHeight;
	int binDepth;

	/* Stores the starting y-coordinate of the latest(topmost) shelf. */
	int currentY;

	/* Tracks the total consumed surface area */
	unsigned long usedSurfaceArea;

	struct Shelf
	{
		int startY;
		int height;
		Guillotine2d guillotine;
	};

	std::vector<Shelf> shelves;

	/**
	 * @param canResize If true, shelf height may be increased to fit the object
	 * @return true and 2 edges if fits, flase otherwise
	 */
	std::tuple<bool, int, int> fitsOnShelf(const Shelf& shelf, int width, int height, int depth, bool canResize) const;

	//void rotateToShelf(const Shelf& shelf, int width, int height) const;

	void addToShelf(Shelf& shelf, int width, int height, int depth, Cuboid& newNode);

	bool canStartNewShelf(int height) const;

	void startNewShelf(int startingHeight);
    
};

#endif
