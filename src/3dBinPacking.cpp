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

#include "../include/cuboid.hpp"
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

std::vector<Cuboid> loadXml(const char* filename)
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



int main(int argc, char* argv[])
{
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
		else if (arg1 == "-f" &&
			(alg == "-shelf" || alg == "-guillotine" || alg == "-global_guillotine" ))
		{
			string filename = argv[2];
			int width = atoi(argv[4]);
			int depth = atoi(argv[5]);
			cout << "-f " << filename << " " << alg  << " width: " << width << " depth: " << depth << endl;
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


	loadXml("/home/krris/workspace/3dBinPacking/visualization/cuboids.xml");
	return 0;
}
