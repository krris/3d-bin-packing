/*
 * guillotine3d.cpp
 *
 *  Created on: 24-05-2013
 *      Author: krris
 */

#include "guillotine3d.hpp"

Guillotine3d::Guillotine3d(int width, int height, int depth) {
}

void Guillotine3d::init(int width, int height, int depth) {
}

Cuboid Guillotine3d::insert(int width, int height,
		FreeCuboidChoiceHeuristic cuboidChoice,
		GuillotineSplitHeuristic splitMethod) {
}

bool Guillotine3d::fits(int width, int heigh,
		FreeCuboidChoiceHeuristic cuboidChoice) const {
}

Cuboid Guillotine3d::findPositionForNewNode(int width, int height,
		FreeCuboidChoiceHeuristic cuboidChoice, int* nodeIndex) const {
}

int Guillotine3d::scoreByHeuristic(int width, int height,
		const Cuboid& freeCuboid, FreeCuboidChoiceHeuristic cuboidChoice) {
}

int Guillotine3d::scoreBestAreaFit(int width, int height,
		const Cuboid& freeCuboid) {
}

int Guillotine3d::scoreBestShortSideFit(int width, int height,
		const Cuboid& freeCuboid) {
}

void Guillotine3d::splitFreeCuboidByHeuristic(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, GuillotineSplitHeuristic method) {
}

void Guillotine3d::splitFreeCuboidAlongAxis(const Cuboid& freeCuboid,
		const Cuboid& placedCuboid, bool splitHorizontal) {
}
