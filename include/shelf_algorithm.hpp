#ifndef SHELF_ALGORITHM_HPP
#define SHELF_ALGORITHM_HPP

#include <boost/smart_ptr.hpp>
#include "forward_declarations.hpp"
#include "cuboid.hpp"

class Shelf{
};

struct Rectangle{
    Rectangle() {}
    Rectangle(int left, int top, int right, int bottom);
    int left;
    int right;
    int top;
    int bottom;

    int width;
    int depth;
};

struct Node{
    Node();
    PNode insert(const Cuboid& cuboid);
    bool isLeaf();
    bool fits(const Cuboid& cuboid);

    PNode leftChild;
    PNode rightChild;
    Rectangle rc;
    PCuboid cuboid;
};



class ShelfAlgorithm{
public:
private:

    
};

#endif
