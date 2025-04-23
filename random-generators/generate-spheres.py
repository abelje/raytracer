import random

text = """# created by generate-spheres.py
# textures 
texture red solid (1 0 0)
texture green solid (0 1 0)
texture blue solid (0 0 1)
texture white solid (1 1 1)

# materials
material red lambertian red false
material green lambertian green false
material blue lambertian blue false
material light diffuse white true

# rendering
threads 20
rays 10 100
camera (0 -50 0) (0 0 0) (0 0 1) 45
pixels 1280 720
output bvh.png

"""

def random_sphere():
    x = 40 * (random.random() - 0.5)
    y = 40 * random.random()
    z = 20 * (random.random() - 0.5)

    radius = random.random()*3 + 0.1
    material = random.choice(["red", "green", "blue", "light", "light"])
    if material == "light":
        radius /= 2

    line = f'sphere ({x} {y} {z}) {radius} {material}\n'
    return line


num_spheres = 1000

filename = f'input-spheres-{num_spheres}'
with open(filename, 'w') as f:
    f.write(text)
    for _ in range(num_spheres):
        line = random_sphere()
        f.write(line)

print(f'Wrote input file: {filename}')
