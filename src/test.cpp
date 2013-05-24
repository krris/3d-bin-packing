#include <iostream>
#include <fstream>
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
//#include <boost/test/minimal.hpp>

#include "../include/forward_declarations.hpp"
#include "../include/cuboid.hpp"
#include "../include/vertex.hpp"
#include "../include/shelf_algorithm.hpp"
#include "../include/Guillotine2d.hpp"

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


int main(int argc, char* argv[])
{
    using namespace std;

    // Test serialization
    Cuboid cuboid_1(100, 100, 100);
    Cuboid cuboid_2(50, 50, 50);
    Cuboid cuboid_3(30, 30, 30);

    Guillotine2d guillotine(200, 200);
    Rect cub1_rect = guillotine.insert(cuboid_1.width, cuboid_1.height, Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);
    Rect cub2_rect = guillotine.insert(cuboid_2.width, cuboid_2.height, Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);
    Rect cub3_rect = guillotine.insert(cuboid_3.width, cuboid_3.height, Guillotine2d::RectBestAreaFit, Guillotine2d::SplitLongerLeftoverAxis);

    for (Rect rect : guillotine.GetUsedRectangles())
    {
    	cout << "Used rectangle:" << endl;
    	cout << "Width: " << rect.width << endl;
    	cout << "Height: " << rect.height << endl;
    	cout << "x: " << rect.x << endl;
    	cout << "y: " << rect.y << endl;
    	cout << "-------------------------------------" << endl;
    }

    cuboid_1.x = cub1_rect.x + (0.5 * cub1_rect.width);
    cuboid_1.z = cub1_rect.y + (0.5 * cub1_rect.height);
    cuboid_1.y += 0.5 * cuboid_1.height;

    cuboid_2.x = cub2_rect.x + (0.5 * cub2_rect.width);
    cuboid_2.z = cub2_rect.y + (0.5 * cub2_rect.height);
    cuboid_2.y += 0.5 * cuboid_2.height;

    cuboid_3.x = cub3_rect.x + (0.5 * cub3_rect.width);
    cuboid_3.z = cub3_rect.y + (0.5 * cub3_rect.height);
    cuboid_3.y += 0.5 * cuboid_3.height;

    vector<Cuboid> cuboids;
    cuboids.push_back(cuboid_1);
    cuboids.push_back(cuboid_2);
    cuboids.push_back(cuboid_3);

       // string filename(boost::archive::tmpdir());
    string filename = "/home/krris/workspace/3dBinPacking/visualization/cuboids.xml";
    saveXml(cuboids, filename.c_str());

 /*   vector<Cuboid> cuboids;
//    cuboids.push_back(cuboid_1);
//    cuboids.push_back(cuboid_2);
//    cuboids.push_back(cuboid_3);

   // string filename(boost::archive::tmpdir());
//    string filename = "/home/krris/workspace/3dBinPacking/visualization/cuboids.xml";
//    saveXml(cuboids, filename.c_str());


    // Test Node
    PNode node(new Node);
  //  BOOST_CHECK (node.isLeaf() == true);

    node->rc = Rectangle(0,0,200,200);
    node->insert(cuboid_1);
    node->insert(cuboid_2);
    node->insert(cuboid_3);

    node->cuboid->x = node->rc.left + 0.5 * node->cuboid->width;
    node->cuboid->y = node->rc.top + 0.5 * node->cuboid->depth;
    
    cuboids.push_back(*(node->cuboid));

    string filename = "/home/krris/workspace/3dBinPacking/visualization/cuboids.xml";
    saveXml(cuboids, filename.c_str());
    
    */

    
    cout << "works!"<<endl;
    return 0;
}
