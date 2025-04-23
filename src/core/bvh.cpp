#include "bvh.h"
#include "constants.h"
#include "random.h"
#include "hit.h"
#include <algorithm>

BVH::BVH(const std::vector<std::unique_ptr<Object>>& objects) {
    // Nodes of the BVH only need a pointer to the object, not to maintain ownership.
    // Build a list of raw object pointers
    std::vector<Object*> object_pointers;
    for (const auto& obj : objects) {
        object_pointers.push_back(obj.get());
    }
    root = build_tree(object_pointers);
}

std::optional<Hit> BVH::find_nearest(const Ray& ray) const {
    // test intersection with the root node
    // if you hit something, then construct a hit on the object
    // otherwise no hit

}


BVH::Node* BVH::build_tree(std::vector<Object*> objects) const {
    // Recursively build a tree
    // at leaf nodes, set the node object and bounding box
    // for all others partition and recursively call build_tree

    // make sure to handle surrounding boxes at non-leaf nodes

}


bool BVH::compare_boxes(Object* a, Object* b, int axis) const {
    // For the given axis (x=0, y=1, z=2) returns whether a's box is less than b's

}

std::pair<std::vector<Object*>, std::vector<Object*>> BVH::partition_along_random_axis(std::vector<Object*> objects) const {
    // sort objects along a random axis
    // divide objects vector into two vectors

}

std::pair<Object*, std::optional<double>> BVH::intersect(Node* n, const Ray& ray) const {
    // First handle the ray not intersection the box, then handle leaf nodes
    
    // Remember that boxes can overlap, so you must test both left and right boxes.
    // Given the results of hitting possible left or right or both, return the closest

}
