# CSC 410 Raytracer Final Project - OBJ Files
My Plan for this final project is to implement OBJ files into the raytracer.

## What is an OBJ File?
Wavefront OBJ stores 3D vector files to generate in the raytracer. 

This is explained on [Fileformat.info](https://www.fileformat.info/format/wavefrontobj/egff.htm).

To keep it simple initially, lets start with a simple obj file.

```
# Simple Wavefront file (Creates a triangle)
v 0.0 0.0 0.0
v 0.0 1.0 0.0
v 1.0 0.0 0.0
f 1 2 3
```
v refers to the points in 3d space for a face, and f describes the face with the order of the points.

### Code for Simple Wavefront file
Using this file:
```
# Simple Wavefront file (Creates a triangle)
v 0.0 0.0 0.0
v 0.0 1.0 0.0
v 1.0 0.0 0.0
f 1 2 3
```
We can set up part of the parser to create the points then create the triangle. Maybe I can make it so the amount of points on the face varies if I use a triangle object or a rectangle object.



## 3D Models
This Website has many created .obj files: [Common-3d-test-models](https://github.com/alecjacobson/common-3d-test-models?tab=readme-ov-file).

