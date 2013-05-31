//============================================================================
// Name        : 3dBinPacking.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "../include/forward_declarations.hpp"
#include "../include/cuboid.hpp"
#include "../include/shelf_algorithm.hpp"
#include "../include/guillotine2d.hpp"
#include "../include/guillotine3d.hpp"
#include "../include/rect.hpp"
using namespace std;

void saveXml(const std::vector<Cuboid>& cuboids, const Rect& base, const char* filename)
{
    std::ofstream ofs(filename);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);

    for (Cuboid cuboid : cuboids)
    {
        oa << boost::serialization::make_nvp("cuboid", cuboid);
    }
    oa << boost::serialization::make_nvp("base", base);
}

std::vector<Cuboid> loadCuboidsFromXml(const char* filename)
{
    std::ifstream ifs(filename);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);

    std::vector<Cuboid> loadedCuboids;
    try
    {
    	while (true)
        {
            Cuboid cuboid;
            ia >> boost::serialization::make_nvp("cuboid",cuboid);
            loadedCuboids.push_back(cuboid);
        }
    }
    catch(boost::archive::archive_exception const& e) { return loadedCuboids;}
    return loadedCuboids;
}

/**
 * Generate random cuboids.
 * @number Number of cuboids.
 * @parameter 3 - every edge size is the same
 * 			  2 - two edges have the same size
 * 			  1 - every edge size is different.
 */
vector<Cuboid> generateRandomCuboids(int number, int parameter)
{
	const int maxSize = 100;
	vector<Cuboid> cuboids;
	for (int i = 0; i < number; ++i)
	{
		int height;
		int depth;
		int width = rand() % maxSize + 1;
		if (parameter == 3)
		{
			height = width;
			depth = width;
		}
		else if (parameter == 2)
		{
			depth = width;
			height = rand() % maxSize + 1;
		}
		else
		{
			height = rand() % maxSize + 1;
			depth = rand() % maxSize + 1;
		}
		Cuboid c(width, height, depth);
		cuboids.push_back(c);
	}
	return cuboids;
}


/**
 * Transform cuboids before displaying in Three.js
 */
vector<Cuboid> transform(const vector<Cuboid>& cuboids)
{
	vector<Cuboid> tranformed;
	for (Cuboid c : cuboids)
	{
		Cuboid newCuboid;
	    newCuboid.x = c.x + (0.5 * c.width);
	    newCuboid.z = c.z + (0.5 * c.depth);
	    newCuboid.y = c.y + 0.5 * c.height;
	    newCuboid.width = c.width;
	    newCuboid.depth = c.depth;
	    newCuboid.height = c.height;
	    tranformed.push_back(newCuboid);
	}
	return tranformed;

}

void shelfAlgorithm(int binWidth, int binDepth, vector<Cuboid> cuboids, string filename)
{
	ShelfAlgorithm shelfAlg(binWidth, binDepth);

	// Sort cuboids
	sort(cuboids.begin(), cuboids.end(), &Cuboid::compareVolume);

	// Insert cuboids
	shelfAlg.insert(cuboids, ShelfAlgorithm::ShelfFirstFit);

	vector<Cuboid> placedCuboids = shelfAlg.getUsedCuboids();
	vector<Cuboid> transformedCuboids = transform(placedCuboids);

	// Save the output xml
	Rect base(binWidth, binDepth);
	saveXml(transformedCuboids, base, filename.c_str());
	cout << "Bin height: " << shelfAlg.getFilledBinHeight() << endl;
}

void guillotineAlgorithm(int binWidth, int binDepth, vector<Cuboid> cuboids, string filename)
{
	Guillotine3d guillotineAlg(binWidth, binDepth);

	// Sort cuboids
	sort(cuboids.begin(), cuboids.end(), &Cuboid::compareMaxEdge);

	// Insert cuboids
	guillotineAlg.insertVector(cuboids, Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerLeftoverAxis);

	vector<Cuboid> placedCuboids = guillotineAlg.getUsedCuboids();
	vector<Cuboid> newCuboids = transform(placedCuboids);

	// Save the output
	Rect base(binWidth, binDepth);
	saveXml(newCuboids, base, filename.c_str());
	cout << "Bin height: " << guillotineAlg.getFilledBinHeight() << endl;
}

void guillotineGlobalAlgorithm(int binWidth, int binDepth, vector<Cuboid> cuboids, string filename)
{
	Guillotine3d guillotineAlg(binWidth, binDepth);

	// Insert cuboids
	guillotineAlg.insertBestGlobalVector(cuboids, Guillotine3d::SplitLongerLeftoverAxis);

	vector<Cuboid> placedCuboids = guillotineAlg.getUsedCuboids();
	vector<Cuboid> newCuboids = transform(placedCuboids);

	// Save the output
	Rect base(binWidth, binDepth);
	saveXml(newCuboids, base, filename.c_str());
	cout << "Bin height: " << guillotineAlg.getFilledBinHeight() << endl;
}

void usage()
{
	cout << "Usage: 3dBinPacking [-shelf | -guillotine | -global_guillotine] width depth [-f in_file | -r arg arg] -o out_file [-t]" << endl<<endl;
	cout << "Options:" << endl;
	cout << "-shelf \t\t: Shelf algorithm + guillotine algorithm (with initial cuboids sorting)" << endl;
	cout << "-guillotine \t: Guillotine algorithm (with initial cuboids sorting)"<< endl;
	cout << "-global_guillotine: Global guillotine algorithm (without initial sorting)" << endl<<endl;
	cout << "width, depth\t: Size of a cuboid base."<< endl;
	cout << "-f \t: Input xml file with information about cuboids to put." << endl;
	cout << "-r \t: Generate n random cuboids, with parameter:" << endl;
	cout << "\t\t 1 - every edge has different length (default)" << endl;
	cout << "\t\t 2 - two edges have the same length" << endl;
	cout << "\t\t 3 - three edges have the same length" << endl;
	cout << "-o \t: Output file which will store information about placed cuboids." << endl;
	cout << "-t \t: Time measurement enabled." << endl << endl;

	cout << "Example:" << endl;
	cout << "./3dBinPacking -shelf 300 250 -f input_file.xml -o output_file.xml -t" << endl;
	cout << "./3dBinPacking -shelf 300 250 -r 1000 2 -o output_file.xml -t" << endl;
}

int main(int argc, char* argv[])
{
	// Set seed
	srand (time(NULL));

	if (argc < 8 || argc > 10)
		usage();
	else
	{
		string algorithm = "";
		string outFile = "";
		string inFile = "";
		int width = -1;
		int depth = -1;
		bool timeMeasurement = false;
		bool random = false;
		int numberOfRandCuboids = -1;
		int paramRandCuboids = 1;

		for (int i = 1; i < argc; ++i)
		{
			string arg = argv[i];
			// algorithm choice
			if (arg == "-shelf" || arg == "-guillotine" || arg == "-global_guillotine")
				algorithm = arg;

			if (i == 2)
				width = atoi(argv[2]);
			if (i == 3)
				depth = atoi(argv[3]);

			if (arg == "-r")
			{
				random = true;
				numberOfRandCuboids = atoi(argv[i + 1]);
				paramRandCuboids = atoi(argv[i + 2]);
			}

			if (arg == "-f")
				inFile = argv[i + 1];

			if (arg == "-o")
				outFile = argv[i + 1];

			if (arg == "-t")
				timeMeasurement = true;
		}

		if (algorithm.empty() || outFile.empty() || width == -1 || depth == -1)
		{
			usage();
			return 1;
		}
		if (inFile.empty())
		{
			if (random == false)
			{
				usage();
				return 1;
			}
		}

		cout << "algorithm: "<< algorithm << " outFile: " << outFile << " inFile: " << inFile
			<< " width: " << width << " depth: " << depth << " random: " << random << " noRandC: " <<numberOfRandCuboids <<
			" param: " << paramRandCuboids << " time: " << timeMeasurement << endl;


		// Run algorithms

		vector<Cuboid> cuboids;
		if (random)
			cuboids = generateRandomCuboids(numberOfRandCuboids, paramRandCuboids);
		else
			cuboids = loadCuboidsFromXml(inFile.c_str());

		clock_t time = clock();

		if (algorithm == "-shelf")
			shelfAlgorithm(width, depth, cuboids, outFile);
		else if (algorithm == "-guillotine")
			guillotineAlgorithm(width, depth, cuboids, outFile);
		else if (algorithm == "-global_guillotine")
			guillotineGlobalAlgorithm(width, depth, cuboids, outFile);


		if (timeMeasurement)
		{
			time = clock() - time;
			double duration = time / (double) CLOCKS_PER_SEC;
			cout << "It took me: " << time << " clicks ("<<duration<< " seconds)" << endl;
		}
	}

	return 0;
}
