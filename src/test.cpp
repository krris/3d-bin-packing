#include <iostream>
#include <fstream>
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
//#include <boost/test/minimal.hpp>

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

int findMaxHeight(const vector<Cuboid>& cuboids)
{
	int max = 0;
	for (Cuboid c : cuboids)
	{
		int height = c.y + 0.5 * c.height;
		if (height > max)
			max = height;
	}
	return max;
}


int main(int argc, char* argv[])
{
    using namespace std;

    // Test serialization
    Cuboid cuboid_1(100, 50, 50);
    Cuboid cuboid_2(100, 100, 100);
    Cuboid cuboid_3(50, 50, 50);
    Cuboid cuboid_4(30, 30, 30);
    Cuboid cuboid_5(30, 30, 30);
    Cuboid cuboid_6(10, 10, 10);
    Cuboid cuboid_7(30, 30, 30);
    Cuboid cuboid_8(30, 30, 30);
    Cuboid cuboid_9(10, 10, 10);
    Cuboid cuboid_10(10, 6, 5);

    vector<Cuboid> cuboids;
    cuboids.push_back(cuboid_1);
    cuboids.push_back(cuboid_2);
    cuboids.push_back(cuboid_3);
    cuboids.push_back(cuboid_4);
    cuboids.push_back(cuboid_5);
    cuboids.push_back(cuboid_6);
    cuboids.push_back(cuboid_7);
    cuboids.push_back(cuboid_8);
    cuboids.push_back(cuboid_9);
    cuboids.push_back(cuboid_10);


//    Guillotine2d guillotine(200, 200);
//    vector<Cuboid> foundPlaces;
//    for (Cuboid c : cuboids)
//    {
//    	Rect place = guillotine.insert(c.width, c.depth, Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);
//    	if (place.isPlaced == true)
//    	{
//    	    c.x = place.x + (0.5 * place.width);
//    	    c.z = place.y + (0.5 * place.height);
//    	    c.y += 0.5 * c.height;
//    		foundPlaces.push_back(c);
//    	}
//    	else
//    	{
//    		cout << "Place not found!:" << endl;
//        	cout << "Width: " << c.width << endl;
//        	cout << "Height: " << c.depth<< endl;
//
//    	}
//    }



//    ShelfAlgorithm shelfAlg(150, 1000, 100);
//    vector<Cuboid> foundPlaces;
//    for (Cuboid c : cuboids)
//    {
//    	Cuboid place = shelfAlg.insert(c.width, c.height, c.depth, ShelfAlgorithm::ShelfFirstFit);
//    	if (place.isPlaced == true)
//    	{
//    	    c.x = place.x + (0.5 * place.width);
//    	    c.z = place.z + (0.5 * place.depth);
//    	    c.y = place.y + 0.5 * place.height;
//    	    c.width = place.width;
//    	    c.height = place.height;
//    	    c.depth = place.depth;
//    		foundPlaces.push_back(c);
//        	cout << "Width: " << place.width << endl;
//        	cout << "Height: " << place.height<< endl;
//        	cout << "Depth: " << place.depth<< endl;
//        	cout << "x: " << place.x<< endl;
//        	cout << "y: " << place.y<< endl;
//        	cout << "z: " << place.z<< endl;
//        	cout << "----------------" << endl;
//    	}
//    	else
//    	{
//    		cout << "Place not found!:" << endl;
//        	cout << "Width: " << c.width << endl;
//        	cout << "Height: " << c.depth<< endl;
//
//    	}
//    }
//
//    cout << "Hight of a bin: " << findMaxHeight(foundPlaces) << endl;
//
//    // string filename(boost::archive::tmpdir());
//    string filename = "/home/krris/workspace/3dBinPacking/visualization/cuboids.xml";
//    saveXml(foundPlaces, filename.c_str());


//    Guillotine3d g3d(100, 100);
//    vector<Cuboid> foundPlaces;
//    for (Cuboid c : cuboids)
//    {
//    	Cuboid place = g3d.insert(c.width, c.height, c.depth, Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
//    	if (place.isPlaced == true)
//    	{
//    	    c.x = place.x + (0.5 * place.width);
//    	    c.z = place.z + (0.5 * place.depth);
//    	    c.y = place.y + 0.5 * place.height;
//    	    c.width = place.width;
//    	    c.height = place.height;
//    	    c.depth = place.depth;
//    		foundPlaces.push_back(c);
//        	cout << "Width: " << place.width << endl;
//        	cout << "Height: " << place.height<< endl;
//        	cout << "Depth: " << place.depth<< endl;
//        	cout << "x: " << place.x<< endl;
//        	cout << "y: " << place.y<< endl;
//        	cout << "z: " << place.z<< endl;
//        	cout << "----------------" << endl;
//    	}
//    	else
//    	{
//    		cout << "Place not found!:" << endl;
//        	cout << "Width: " << c.width << endl;
//        	cout << "Height: " << c.depth<< endl;
//
//    	}
//    }

    Guillotine3d g3d(100, 100);
    vector<Cuboid> foundPlaces;
    int size = cuboids.size();
    for (int i = 0; i < size; ++i)
    {
    	Cuboid place = g3d.insertBestGlobal(cuboids, g3d, Guillotine3d::SplitLongerAxis);
    	Cuboid c;
    	if (place.isPlaced == true)
    	{
        	place = g3d.insert(place,
					Guillotine3d::CuboidMinHeight, Guillotine3d::SplitLongerAxis);
    	    c.x = place.x + (0.5 * place.width);
    	    c.z = place.z + (0.5 * place.depth);
    	    c.y = place.y + 0.5 * place.height;
    	    c.width = place.width;
    	    c.height = place.height;
    	    c.depth = place.depth;
    		foundPlaces.push_back(c);
        	cout << "Width: " << place.width << endl;
        	cout << "Height: " << place.height<< endl;
        	cout << "Depth: " << place.depth<< endl;
        	cout << "x: " << place.x<< endl;
        	cout << "y: " << place.y<< endl;
        	cout << "z: " << place.z<< endl;
        	cout << "----------------" << endl;

    	}
    	else
    	{
    		cout << "Place not found!:" << endl;
        	cout << "Width: " << c.width << endl;
        	cout << "Height: " << c.depth<< endl;

    	}

    	int index;
    	for (unsigned j = 0; j < cuboids.size(); j++)
    	{
    		if (cuboids[j].width == place.width &&
    			cuboids[j].height == place.height &&
    			cuboids[j].depth == place.depth)
    		{
    			index = j;
    			break;
    		}
    	}


    	cuboids.erase(cuboids.begin() + index);
    }



//    cout << "Hight of a bin: " << findMaxHeight(foundPlaces) << endl;
    cout << "Hight of a bin: " << g3d.getFilledBinHeight() << endl;

    // string filename(boost::archive::tmpdir());
    string filename = "/home/krris/workspace/3dBinPacking/visualization/cuboids.xml";
    saveXml(foundPlaces, filename.c_str());


    
    cout << "works!"<<endl;
    return 0;
}
