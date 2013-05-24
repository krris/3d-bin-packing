#include "../include/shelf_algorithm.hpp"

Node::Node()
{
}


Rectangle::Rectangle(int left, int top, int right, int bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;

    this->width = std::abs(right - left);
    this->depth = std::abs(top - bottom);
}


PNode Node::insert(const Cuboid& cuboid_to_insert)
{
    // If we are not a leaf
    if (this->isLeaf() == false)
    {
        // insert into first child
        PNode newNode = leftChild->insert(cuboid_to_insert);
        if (newNode != nullptr)
            return newNode;

        // no room, insert into second
        return rightChild->insert(cuboid_to_insert);
    }
    else
    {
        // if there is already a cuboid here, return
        if (this->cuboid != nullptr)
            return nullptr;

        // if we're to small, return
        if (this->fits(cuboid_to_insert) == false)
            return nullptr;

        // if we're just right, accept
        if (this->fits(cuboid_to_insert) == true)
        {
            return shared_from_this();
        }

        // otherwise, split this node and create some kids
        this->leftChild = std::make_shared<Node>();
        this->rightChild = std::make_shared<Node>();


        // decide which way to split
        int dw = rc.width - cuboid_to_insert.width;
        int dd = rc.depth - cuboid_to_insert.depth;

        if (dw > dd)
        {
            leftChild->rc = Rectangle(rc.left, 
                                        rc.top,
                                        rc.left + cuboid_to_insert.width - 1,
                                        rc.bottom);
            rightChild->rc = Rectangle(rc.left + cuboid_to_insert.width, 
                                        rc.top,
                                        rc.right,
                                        rc.bottom);
        }
        else
        {
            leftChild->rc = Rectangle(rc.left, 
                                        rc.top,
                                        rc.right,
                                        rc.top + cuboid_to_insert.depth -1);
            rightChild->rc = Rectangle(rc.left, 
                                        rc.top + cuboid_to_insert.height,
                                        rc.right,
                                        rc.bottom);
        }

        //insert into first child we created
        return this->leftChild->insert(cuboid_to_insert);
    }

}


bool Node::fits(const Cuboid& cuboid)
{
    // check if fits in rectangle
	if (rc.width >= cuboid.width)
		return true;
	if (rc.depth >= cuboid.depth)
		return true;
    return false;
}


bool Node::isLeaf()
{
    if (leftChild != nullptr)
        return false;
    if (rightChild != nullptr)
        return false;
    return true;
}
