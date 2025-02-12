#include "camera.h"
#include "sphere.h"
#include "pixels.h"

int main() {
    // create spheres, one is small sphere, the other is the ground
    Sphere ground{{0, 0, -100}, 100};
    Sphere sphere{{0,0,1}, 1};

    //image size
    Pixels pixels {1280, 720};

    //create the camera
    Vector3D position{0, -10, 10}, up{0,0,1};
}