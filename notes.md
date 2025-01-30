# Raytracer Project
## Generating test files
Right Click on CMake Lists and choose generate CMake.
## Dot Product
This version of the code returns the dot product of a and b.
I learned about dot product more thoroughly on 
[Dot_Product Math_is_fun](https://www.mathsisfun.com/algebra/vectors-dot-product.html)
This project assumes a right-handed coordinate system.
```c++
double dot(const Vector3D& a, const Vector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
```

If I needed to get the angle, I could do this:
```c++
double dot(const Vector3D& a, const Vector3D& b) {
     double dot_product = a.x * b.x + a.y * b.y + a.z * b.z;
     double dis_a = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
     double dis_b = std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
     return 1/cos(dot_product/(dis_a * dis_b));
}
```

## Cross Product
Learned extra information about Cross Product on 
[Cross_Product Math_is_fun](https://www.mathsisfun.com/algebra/vectors-cross-product.html)

## Unit
We have to check if the unit is equal to zero and throw an overflow error if it is, because it is used in division.
```c++
Vector3D unit(const Vector3D& v) {
    if (length(v) == 0) {
        throw std::overflow_error("Unit Length = 0!");
    }
    double b{1/length(v)};
    return Vector3D{b, b, b};
}
```
Test Code:
```c++
SUBCASE( "unit" ) {
    Vector3D a{1, 1, 1};
    double b{1/std::sqrt(3)};
    CHECK( unit(a) == Vector3D{b, b, b} );
    CHECK_THROWS_AS(unit({0, 0, 0}), std::overflow_error);
  }
```
## Making Color Codes
Colors are created using RGB values such as the example of Black below.
```c++
const Color Black = {0, 0, 0};
```