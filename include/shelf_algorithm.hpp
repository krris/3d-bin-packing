#ifndef SHELF_ALGORITHM_HPP
#define SHELF_ALGORITHM_HPP

#include "forward_declarations.hpp"
#include "Rect.hpp"
#include "cuboid.hpp"


class ShelfAlgorithm{
public:
	ShelfAlgorithm() : binWidth(0), binHeight(0), currentY(0), usedSurfaceArea(0) {}
	ShelfAlgorithm(int width, int height);

	void init(int width, int height);
	enum ShelfChoiceHeuristic
	{
		ShelfNextFit, ///< -NF: We always put the new rectangle to the last open shelf.
		ShelfFirstFit, ///< -FF: We test each rectangle against each shelf in turn and pack it to the first where it fits.
		ShelfBestAreaFit, ///< -BAF: Choose the shelf with smallest remaining shelf area.
		ShelfWorstAreaFit, ///< -WAF: Choose the shelf with the largest remaining shelf area.
		ShelfBestHeightFit, ///< -BHF: Choose the smallest shelf (height-wise) where the rectangle fits.
		ShelfBestWidthFit, ///< -BWF: Choose the shelf that has the least remaining horizontal shelf space available after packing.
		ShelfWorstWidthFit,
	};

	Rect insert(int width, int heigh, ShelfChoiceHeuristic method);

private:
	int binWidth;
	int binHeight;

	/* Stores the starting y-coordinate of the latest(topmost) shelf. */
	int currentY;

	/* Tracks the total consumed surface area */
	unsigned long usedSurfaceArea;

	struct Shelf
	{
		int currentX;
		int startY;
		int height;
		std::vector<Rect> usedRectangles;
	};

	std::vector<Shelf> shelves;

	/**
	 * @param canResize If true, shelf height may be increased to fit the object
	 */
	bool fitsOnShelf(const Shelf& shelf, int width, int height, bool canResize) const;

	void rotateToShelf(const Shelf& shelf, int width, int height) const;

	void addToShelf(Shelf& shelf, int width, int height, Rect& newNode);

	bool canStartNewShelf(int height) const;

	void startNewShelf(int startingHeight);
    
};

#endif
