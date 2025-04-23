import random
import math


text = """# created by generate-triangles.py
# textures
texture white solid (1 1 1)
texture red solid (1 0 0)
texture triangle-color solid (0.5 0.5 0.5)

# materials
material white-light diffuse white true
material red-light diffuse red true
material triangle lambertian triangle-color false

# lighting
sphere (0 0 0) 100 white-light
sphere (-110 0 0) 100 red-light


# rendering
threads 10
rays 10 10
camera (-5 -20 15) (0 0 0) (0 0 1) 30
pixels 1280 720
output bvh.png

"""


def random_vector(N):
    return [random.uniform(-1, 1) for _ in range(N)]

def random_unit():
    while True:
        x, y, z = random_vector(3)
        length_squared = x**2 + y**2 + z**2
        if 1e-160 < length_squared <= 1:
            length = math.sqrt(length_squared)
            x /= length
            y /= length
            z /= length
            return x, y, z

def dot(a, b):
    return sum(i*j for i, j in zip(a, b))

def generate_vertices(N):
    return [random_unit() for _ in range(N)]

def generate_triangles(vertices, N):
    if len(vertices) < 3:
        raise ValueError("Must provide 3 or more vertices to make a triangle")
    
    triangles = []
    for _ in range(N):
        i = random.randint(0, len(vertices)-1)
        j = random.randint(0, len(vertices)-1)
        while j == i:
            j = random.randint(0, len(vertices)-1)
            
        k = random.randint(0, len(vertices)-1)
        while k == i or k == j:
            k = random.randint(0, len(vertices)-1)
        triangles.append((i, j, k))

    return triangles


def write(filename, vertices, triangles):
    with open(filename, 'w') as f:
        f.write('vertices\n')
        for x, y, z in vertices:
            f.write('({} {} {})\n'.format(x, y, z))

        f.write('triangles\n')
        for i, j, k in triangles:
            f.write('{} {} {}\n'.format(i, j, k))

    print(f'Wrote mesh file: {filename}')


def completely_random(N):
    v = generate_vertices(N)
    # scale by R
    R = 3
    v = [(x*R, y*R, z*R) for x, y, z in v]
    
    t = generate_triangles(v, N//2)
    write(f'triangles-{N}', v, t)
    

def spiky(N):
    """This function can get stuck in an infinite loop if N < 20 or so"""
    inner = generate_vertices(2*N//3)
    outer = generate_vertices(N//3)
    size = 4
    outer = [(x*size, y*size, z*size) for x, y, z in outer]
    triangles = []
    for _ in range(N//6):
        i = random.randint(0, len(inner)-1)
        j = random.randint(0, len(inner)-1)
        while j == i or dot(inner[i], inner[j]) < 0:
            j = random.randint(0, len(inner)-1)
        
        k = random.randint(0, len(inner)-1)
        while k == j or k == i or dot(inner[i], inner[k]) < 0 or dot(inner[j], inner[k]) < 0:
            k = random.randint(0, len(inner)-1)

        n = random.randint(0, len(outer)-1)
        while dot(outer[n], inner[i]) < 0 or dot(outer[n], inner[j]) < 0 or dot(outer[n], inner[k]) < 0:
            n = random.randint(0, len(outer)-1)
        
        triangles.append((i, j, n + len(inner)))
        triangles.append((i, k, n + len(inner)))
        triangles.append((j, k, n + len(inner)))


    write(f'triangles-{N}', inner + outer, triangles)


def make_box(a, b):
    """To simplify constructing a box, we can interpret the x, y, z
coordinates as the binary number xyz where a=0 and b=1:

    011__________111
      /|        /|
     / |   101 / |
 001/_________/  |
    |  |_ _ _ | _|110
    |  /010   |  /
    | /       | /
    |/________|/
  000        100

    """

    # using a=0, b=1 select each corner's x, y, z coordinate
    vertices = [(a[0] if i & 4 == 0 else b[0],
                 a[1] if i & 2 == 0 else b[1],
                 a[2] if i & 1 == 0 else b[2])
                for i in range(8)]

    triangles = [
        # left
        (0, 1, 2),
        (2, 1, 3),
        # right
        (4, 5, 7),
        (7, 4, 6),
        # front
        (0, 1, 4),
        (4, 1, 5),
        # back
        (2, 6, 7),
        (2, 3, 7),
        # bottom
        (0, 2, 6),
        (0, 4, 6),
        # top
        (1, 3, 7),
        (1, 5, 7)
    ]
    return vertices, triangles

    
def boxes(N):
    """N is the number of boxes, so total triangles = 12*N"""
    vertices = []
    triangles = []
    for i in range(N):
        R = 5
        x = R*(random.random() - 0.5)
        y = R*(random.random() - 0.5)
        z = R*(random.random() - 0.5)
        
        size = random.random()
        v, t = make_box((x, y, z), (x+size, y+size, z+size))

        # need to shift triangle indices to account for previous vertices
        shift = 8*i # 8 per box
        t = [(i+shift, j+shift, k+shift) for i, j, k in t]

        vertices.extend(v)
        triangles.extend(t)
        

    write(f"triangles-{N}", vertices, triangles)
    
        
    

num_triangles = 100

filename = f'input-triangles-{num_triangles}'
with open(filename, 'w') as f:
    f.write(text)
    f.write(f'mesh (0 0 0) triangles-{num_triangles} triangle')

print(f'Wrote input file: {filename}')

# uncomment one below or make your own!
completely_random(num_triangles)
# spiky(num_triangles)
# boxes(num_triangles) # the argument is the number of boxes, so this generates 12*num_triangles triangles
