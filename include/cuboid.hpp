#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <vector>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "forward_declarations.hpp"
#include "vertex.hpp"

class Cuboid {
public:
    Cuboid(int w, int h, int d) :
        width(w), height(h), depth(d) {}

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_verision)
    {
        ar & boost::serialization::make_nvp("width", width);
        ar & boost::serialization::make_nvp("height", height);
        ar & boost::serialization::make_nvp("depth", depth);
        ar & boost::serialization::make_nvp("x", x);
        ar & boost::serialization::make_nvp("y", y);
        ar & boost::serialization::make_nvp("z", z);
    }

    const int width;
    const int height;
    const int depth;
    int x = 0;
    int y = 0;
    int z = 0;
private:
    std::vector<Vertex> vertices;


};

#endif
