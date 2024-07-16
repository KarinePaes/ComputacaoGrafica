import numpy as np
from PIL import Image, ImageDraw, ImageSequence


def normalize(vector):
    return vector / np.linalg.norm(vector)

def intersect_ray_sphere(origin, direction, sphere_center, sphere_radius):
    oc = origin - sphere_center
    a = np.dot(direction, direction)
    b = 2.0 * np.dot(oc, direction)
    c = np.dot(oc, oc) - sphere_radius * sphere_radius
    discriminant = b * b - 4 * a * c

    if discriminant < 0:
        return False, None

    t1 = (-b - np.sqrt(discriminant)) / (2.0 * a)
    t2 = (-b + np.sqrt(discriminant)) / (2.0 * a)
    
    if t1 < 0 and t2 < 0:
        return False, None

    t = min(t for t in [t1, t2] if t > 0)
    hit_point = origin + t * direction
    return True, hit_point

def trace_ray(origin, direction, sphere_center, sphere_radius, light_position, ambient_light):
    hit, hit_point = intersect_ray_sphere(origin, direction, sphere_center, sphere_radius)
    
    if not hit:
        return np.array([255, 255, 255])
    
    normal = normalize(hit_point - sphere_center)
    light_dir = normalize(light_position - hit_point)
    intensity = np.dot(normal, light_dir)
    intensity = np.clip(intensity, 0, 1)

    color = intensity * np.array([255, 255, 255])
    color = color + ambient_light
    color = np.clip(color, 0, 255)
    
    return color

width, height = 400, 300
camera = np.array([0, 0, 0])
sphere_center = np.array([0, 0, -3])
sphere_radius = 1
ambient_light = np.array([50, 50, 50]) 

images = []

for i in range(20):
    light_position = np.array([2, 5 - i * 0.5, -3])   # Movendo a luz de cima para baixo
  
    image = Image.new("RGB", (width, height))
    pixels = image.load()

    for y in range(height):
        for x in range(width):
            screen_x = (x - width / 2) / width
            screen_y = (height / 2 - y) / height
            direction = normalize(np.array([screen_x, screen_y, -1]))
            
            color = trace_ray(camera, direction, sphere_center, sphere_radius, light_position, ambient_light)
            pixels[x, y] = tuple(color.astype(int))

    images.append(image)

images[0].save('ray_tracing_output.gif', save_all=True, append_images=images[1:], duration=100, loop=0)

images[0].show()
