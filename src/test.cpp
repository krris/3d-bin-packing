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
    cuboid_2.y = 75;
    cuboid_2.z = 50;
    Cuboid cuboid_3(50, 50, 50);
    cuboid_3.y = -75;

    vector<Cuboid> cuboids;
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
    
    
    cout << "works!"<<endl;
    return 0;
}
