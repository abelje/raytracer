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
    auto [obj, t] = intersect(root, ray);
    if (t) {
        return obj->construct_hit(ray, *t);
    }
    // otherwise no hit
    return {};
}


BVH::Node* BVH::build_tree(std::vector<Object*> objects) const {
    // Recursively build a tree
    Node* n = new Node{};

    // at leaf nodes, set the node object and bounding box
    if (objects.size() == 1) { // leaf
        n->object = objects[0];
        n->box = objects[0]->bounding_box();
        return n;
    }

    // for all others partition and recursively call build_tree
    else {
        auto [left, right] = partition_along_random_axis(objects);
        n->left = build_tree(left);
        n->right = build_tree(right);
        n->box = surrounding_box(n->left->box, n->right->box);
        return n;
    }

    // make sure to handle surrounding boxes at non-leaf nodes

}


bool BVH::compare_boxes(Object* a, Object* b, int axis) const {
    // For the given axis (x=0, y=1, z=2) returns whether a's box is less than b's
    auto box_a = a->bounding_box();
    auto box_b = b->bounding_box();
    if (axis == 0) {
        return box_a.minimum.x < box_b.minimum.x;
    }
    else if (axis == 1) {
        return box_a.minimum.y < box_b.minimum.y;
    }
    else { // axis == 2
        return box_a.minimum.z < box_b.minimum.z;
    }
}

std::pair<std::vector<Object*>, std::vector<Object*>> BVH::partition_along_random_axis(std::vector<Object*> objects) const {
    int axis = random_int(0, 2);

    std::sort(objects.begin(), objects.end(), [this, axis](Object* a, Object* b) {
        return compare_boxes(a, b, axis);
    });

    size_t size = objects.size() / 2;
    std::vector<Object*> left(objects.begin(), objects.begin() + size);
    std::vector<Object*> right(objects.begin() + size, objects.end());

    return {left, right};
}

std::pair<Object*, std::optional<double>> closer_hit(const std::pair<Object*, std::optional<double>>& a, const std::pair<Object*, std::optional<double>>& b) {
    if (a.second && (!b.second || *a.second < *b.second)) { // if a hits, and b either misses or is less then a
        return a;
    }
    return b;
}

std::pair<Object*, std::optional<double>> BVH::intersect(Node* n, const Ray& ray) const {
    // First handle the ray not intersection the box, then handle leaf nodes
    if (!n->box.intersect(ray)) {
        return {};
    }
    if (!n->left && !n->right) {  // Leaf node
        auto time = n->object->intersect(ray);
        return {n->object, time};
    }
    // Remember that boxes can overlap, so you must test both left and right boxes.
    auto lhit = intersect(n->left, ray);
    auto rhit = intersect(n->right, ray);
    // Given the results of hitting possible left or right or both, return the closest
    return closer_hit(lhit, rhit);
}
