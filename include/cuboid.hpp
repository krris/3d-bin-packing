#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <vector>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>

class Cuboid {
public:
	Cuboid() {}
    Cuboid(float w, float h, float d) :
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

    float width;
    float height;
    float depth;
    float x = 0;
    float y = 0;
    float z = 0;
    bool isPlaced = false;

    /* Variable useful in global guillotine algorithm to store a score
     * of choosing this cuboid. */
    float score = std::numeric_limits<float>::max();

    static bool compareMaxEdge(Cuboid i,Cuboid j)
    {
    	float i_max = std::max({i.width, i.height, i.depth});
    	float j_max = std::max({j.width, j.height, j.depth});
    	return i_max > j_max;
    }

    static bool compareVolume(Cuboid i,Cuboid j)
    {
    	return ((i.width * i.height * i.depth) > (j.width * j.height * j.depth));
    }

    friend bool operator<(const Cuboid& lhs,const Cuboid& rhs){
        return (lhs.width * lhs.height * lhs.depth) < (rhs.width * rhs.depth * rhs.height);
    }

};

#endif
