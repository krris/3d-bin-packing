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

vector<Cuboid> generateRandomCuboids(int number)
{
	const int maxSize = 100;
	vector<Cuboid> cuboids;
	for (int i = 0; i < number; ++i)
	{
		int width = rand() % maxSize + 1;
		int height = rand() % maxSize + 1;
		int depth = rand() % maxSize + 1;
		Cuboid c(width, height, depth);
		cuboids.push_back(c);
	}
	return cuboids;
}



vector<Cuboid> transform(vector<Cuboid> cuboids)
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
	sort(cuboids.begin(), cuboids.end(), &Cuboid::compareVolume);
	shelfAlg.insert(cuboids, ShelfAlgorithm::ShelfFirstFit);
	vector<Cuboid> placedCuboids = shelfAlg.getUsedCuboids();
	vector<Cuboid> newCuboids = transform(placedCuboids);
	Rect base;
	base.width = binWidth;
	base.height = binDepth;
	saveXml(newCuboids, base, filename.c_str());
	cout << "Bin height: " << shelfAlg.getFilledBinHeight() << endl;
}

void guillotineAlgorithm(int binWidth, int binDepth, vector<Cuboid> cuboids, string filename)
{
	Guillotine3d guillotineAlg(binWidth, binDepth);
	sort(cuboids.begin(), cuboids.end(), &Cuboid::compareMaxEdge);
	guillotineAlg.insertVector(cuboids, Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerLeftoverAxis);
	vector<Cuboid> placedCuboids = guillotineAlg.getUsedCuboids();
	vector<Cuboid> newCuboids = transform(placedCuboids);
	Rect base;
	base.width = binWidth;
	base.height = binDepth;;
	saveXml(newCuboids, base, filename.c_str());
	cout << "Bin height: " << guillotineAlg.getFilledBinHeight() << endl;
}

void guillotineGlobalAlgorithm(int binWidth, int binDepth, vector<Cuboid> cuboids, string filename)
{
	Guillotine3d guillotineAlg(binWidth, binDepth);
	guillotineAlg.insertBestGlobalVector(cuboids, Guillotine3d::SplitLongerLeftoverAxis);
	vector<Cuboid> placedCuboids = guillotineAlg.getUsedCuboids();
	vector<Cuboid> newCuboids = transform(placedCuboids);
	Rect base;
	base.width = binWidth;
	base.height = binDepth;
	saveXml(newCuboids, base, filename.c_str());
	cout << "Bin height: " << guillotineAlg.getFilledBinHeight() << endl;
}

int frequency_of_primes (int n) {
  int i,j;
  int freq=n-1;
  for (i=2; i<=n; ++i) for (j=sqrt(i);j>1;--j) if (i%j==0) {--freq; break;}
  return freq;
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
	//string outputFilename = "/home/krris/workspace/3dBinPacking/visual/app/cuboids_to_render.xml";
	//string inputFilename = "/home/krris/workspace/3dBinPacking/visual/app/cuboids_input.xml";
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
		if (outFile.empty())
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
	}




//	if (argc < 6 || argc > 7)
//		usage();
//	// Without time measurement
//	else if (argc == 6)
//	{
//		string arg1 = argv[1];
//		string alg = argv[3];
//		// Generete random cuboids
//		if (arg1 == "-r" &&
//		   (alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
//		{
//			int number = atoi(argv[2]);
//			int width = atoi(argv[4]);
//			int depth = atoi(argv[5]);
//			cout << "-r " << number << " " << alg << " width: " << width << " depth: " << depth << endl;
//			if (alg == "-shelf")
//			{
//				vector<Cuboid> cuboids = generateRandomCuboids(number);
//				shelfAlgorithm(width, depth, cuboids, outputFilename);
//			}
//			else if (alg == "-guillotine")
//			{
//				vector<Cuboid> cuboids = generateRandomCuboids(number);
//				guillotineAlgorithm(width, depth, cuboids, outputFilename);
//			}
//			else if (alg == "-global_guillotine")
//			{
//				vector<Cuboid> cuboids = generateRandomCuboids(number);
//				guillotineGlobalAlgorithm(width, depth, cuboids, outputFilename);
//			}
//
//		}
//		// Load cuboids from xml file.
//		else if (arg1 == "-f" &&
//			(alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
//		{
//			string filename = argv[2];
//			int width = atoi(argv[4]);
//			int depth = atoi(argv[5]);
//			cout << "-f " << filename << " " << alg  << " width: " << width << " depth: " << depth << endl;
//			if (alg == "-shelf")
//			{
//				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
//				shelfAlgorithm(width, depth, cuboids, outputFilename);
//			}
//			else if (alg == "-guillotine")
//			{
//				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
//				guillotineAlgorithm(width, depth, cuboids, outputFilename);
//			}
//			else if (alg == "-global_guillotine")
//			{
//				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
//				guillotineGlobalAlgorithm(width, depth, cuboids, outputFilename);
//			}
//
//		}
//		else
//			usage();
//	}
//	// With time measurement
//	else if (argc == 7)
//	{
//		string arg1(argv[1]);
//		string arg2 = argv[2];
//		string alg = argv[4];
//		// Generate random cuboids
//		if (arg1 == "-t" && arg2 == "-r" &&
//		   (alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
//		{
//			int number = atoi(argv[3]);
//			int width = atoi(argv[5]);
//			int depth = atoi(argv[6]);
//			cout << "-t -r " << number << " " << alg << " width: " << width << " depth: " << depth << endl;
//			clock_t time = clock();
//			if (alg == "-shelf")
//			{
//				vector<Cuboid> cuboids = generateRandomCuboids(number);
//				shelfAlgorithm(width, depth, cuboids, outputFilename);
//				time = clock() - time;
//			}
//			else if (alg == "-guillotine")
//			{
//				vector<Cuboid> cuboids = generateRandomCuboids(number);
//				guillotineAlgorithm(width, depth, cuboids, outputFilename);
//				time = clock() - time;
//			}
//			else if (alg == "-global_guillotine")
//			{
//				vector<Cuboid> cuboids = generateRandomCuboids(number);
//				guillotineGlobalAlgorithm(width, depth, cuboids, outputFilename);
//				time = clock() - time;
//			}
//			double duration = time / (double) CLOCKS_PER_SEC;
//			cout << "It took me: " << time << " clicks ("<<duration<< " seconds)" << endl;
//		}
//		// Load cuboids from xml file
//		else if (arg1 == "-t" && arg2 == "-f" &&
//		   (alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
//		{
//			string filename = argv[3];
//			int width = atoi(argv[5]);
//			int depth = atoi(argv[6]);
//			cout << "-t -f " << filename << " " << alg << " width: " << width << "depth: " << depth << endl;
//			clock_t time = clock();
//			if (alg == "-shelf")
//			{
//				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
//				shelfAlgorithm(width, depth, cuboids, outputFilename);
//				time = clock() - time;
//			}
//			else if (alg == "-guillotine")
//			{
//				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
//				guillotineAlgorithm(width, depth, cuboids, outputFilename);
//				time = clock() - time;
//			}
//			else if (alg == "-global_guillotine")
//			{
//				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
//				guillotineGlobalAlgorithm(width, depth, cuboids, outputFilename);
//				time = clock() - time;
//			}
//			double duration = time / (double) CLOCKS_PER_SEC;
//			cout << "It took me: " << time << " clicks ("<<duration<< " seconds)" << endl;
//		}
//		else
//			usage();
//	}


//	cout << "Works!" << endl;
	return 0;
}
