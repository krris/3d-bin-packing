//============================================================================
// Name        : 3dBinPacking.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "../include/forward_declarations.hpp"
#include "../include/cuboid.hpp"
#include "../include/shelf_algorithm.hpp"
#include "../include/guillotine2d.hpp"
#include "../include/guillotine3d.hpp"
using namespace std;

void saveXml(const std::vector<Cuboid>& cuboids, const char* filename)
{
    std::ofstream ofs(filename);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    for (Cuboid cuboid : cuboids)
    {
        oa << boost::serialization::make_nvp("cuboid", cuboid);
    }
}

std::vector<Cuboid> loadCuboidsFromXml(const char* filename)
{

    std::ifstream ifs(filename);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);

    std::vector<Cuboid> loadedCuboids;
    try
    {
        for(;;)
        {
            Cuboid cuboid;
            ia >> boost::serialization::make_nvp("cuboid",cuboid);
            //your logic
            loadedCuboids.push_back(cuboid);
        }
    }
    catch(boost::archive::archive_exception const& e) { return loadedCuboids;}
    return loadedCuboids;
}

vector<Cuboid> generateRandomCuboids(int number)
{
	const int maxSize = 200;
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

void usage()
{
	cout << "Usage: 3dBinPacking [-t] [-f file | -r n] [-shelf | -guillotine | -global_guillotine] width depth" << endl;
	cout << "Options:" << endl;
	cout << "-t \t: Time measurement enabled." << endl;
	cout << "-r \t: Generate n random cuboids." << endl;
	cout << "-shelf \t: Shelf algorithm + guillotine algorithm (with initial cuboids sorting)" << endl;
	cout << "-guillotine \t: Guillotine algorithm (with initial cuboids sorting)"<< endl;
	cout << "-global_guillotine \t: Global guillotine algorithm (without initial sorting)" << endl;
	cout << "-f \t: Load information about cuboids from xml file." << endl;
}

vector<Cuboid> transform(vector<Cuboid> cuboids)
{
	vector<Cuboid> tranformed;
	for (Cuboid c : cuboids)
	{
//    	cout << "Width: " << c.width << endl;
//    	cout << "Height: " << c.height<< endl;
//    	cout << "Depth: " << c.depth<< endl;
//    	cout << "x: " << c.x<< endl;
//    	cout << "y: " << c.y<< endl;
//    	cout << "z: " << c.z<< endl;
//    	cout << "----------------" << endl;
		Cuboid newCuboid;
	    newCuboid.x = c.x + (0.5 * c.width);
	    newCuboid.z = c.z + (0.5 * c.depth);
	    newCuboid.y = c.y + 0.5 * c.height;
	    newCuboid.width = c.width;
	    newCuboid.depth = c.depth;
	    newCuboid.height = c.height;
	    tranformed.push_back(newCuboid);
//	        	cout << "Width: " << c.width << endl;
//	        	cout << "Height: " << c.height<< endl;
//	        	cout << "Depth: " << c.depth<< endl;
//	        	cout << "x: " << c.x<< endl;
//	        	cout << "y: " << c.y<< endl;
//	        	cout << "z: " << c.z<< endl;
//	        	cout << "----------------" << endl;
	}

	for (Cuboid c : tranformed)
	{
	        	cout << "Width: " << c.width << endl;
	        	cout << "Height: " << c.height<< endl;
	        	cout << "Depth: " << c.depth<< endl;

	        	cout << "x: " << c.x<< endl;
	        	cout << "y: " << c.y<< endl;
	        	cout << "z: " << c.z<< endl;
	        	cout << "----------------" << endl;
	}
	return tranformed;

}


void shelfAlgorithm(int binWidth, int binDepth, vector<Cuboid> cuboids, string filename)
{
	ShelfAlgorithm shelfAlg(binWidth, binDepth);
	/*vector<Cuboid> placedCuboids =*/ shelfAlg.insert(cuboids, ShelfAlgorithm::ShelfFirstFit);
	vector<Cuboid> placedCuboids = shelfAlg.getUsedCuboids();
	vector<Cuboid> newCuboids = transform(placedCuboids);
	saveXml(newCuboids, filename.c_str());
}




//{
//	vector<Cuboid> placedCuboids;
//	ShelfAlgorithm shelfAlg(binWidth, binDepth);
//    for (Cuboid c : cuboids)
//    {
//    	Cuboid placed = shelfAlg.insert(c, ShelfAlgorithm::ShelfFirstFit);
//    	if (placed.isPlaced == true)
//    	{
//    		placedCuboids.push_back(c);
//    	}
//    	else
//    	{
//    		cout << "Place not found!:" << endl;
//        	cout << "Width: " << c.width << endl;
//        	cout << "Height: " << c.height<< endl;
//        	cout << "Depth: " << c.depth<< endl;
//
//    	}
//    }
//    return placedCuboids;
//}






int main(int argc, char* argv[])
{
	string outputFilename = "/home/krris/workspace/3dBinPacking/visualization/cuboids.xml";
	string inputFilename = "/home/krris/workspace/3dBinPacking/visualization/cuboids_input.xml";
	// set seed
	srand (time(NULL));

	if (argc < 6 || argc > 7)
		usage();
	else if (argc == 6)
	{
		string arg1 = argv[1];
		string alg = argv[3];
		if (arg1 == "-r" &&
		   (alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
		{
			int number = atoi(argv[2]);
			int width = atoi(argv[4]);
			int depth = atoi(argv[5]);
			cout << "-r " << number << " " << alg << " width: " << width << " depth: " << depth << endl;
		}
		// Load cuboids from xml file.
		else if (arg1 == "-f" &&
			(alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
		{
			string filename = argv[2];
			int width = atoi(argv[4]);
			int depth = atoi(argv[5]);
			cout << "-f " << filename << " " << alg  << " width: " << width << " depth: " << depth << endl;
			if (alg == "-shelf")
			{
				vector<Cuboid> cuboids = loadCuboidsFromXml(filename.c_str());
				shelfAlgorithm(width, depth, cuboids, outputFilename);
			}

		}
		else
			usage();
	}
	else if (argc == 7)
	{
		string arg1(argv[1]);
		string arg2 = argv[2];
		string alg = argv[4];
		if (arg1 == "-t" && arg2 == "-r" &&
		   (alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
		{
			int number = atoi(argv[3]);
			int width = atoi(argv[5]);
			int depth = atoi(argv[6]);
			cout << "-t -r " << number << " " << alg << " width: " << width << " depth: " << depth << endl;
		}
		else if (arg1 == "-t" && arg2 == "-f" &&
		   (alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
		{
			string filename = argv[3];
			int width = atoi(argv[5]);
			int depth = atoi(argv[6]);
			cout << "-t -f " << filename << " " << alg << "width: " << width << "depth: " << depth << endl;
		}
		else
			usage();
	}


	cout << "Works!" << endl;
	return 0;
}
