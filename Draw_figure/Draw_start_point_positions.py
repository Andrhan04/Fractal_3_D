import os
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.pyplot as plt
import json

def get_data(arr_x: list, arr_y: list, arr_z: list, path: str):
    with open(file=path, mode='r') as file:
        for line in file:
            parts = line.split()
            if len(parts) < 3:
                continue
            x, y, z = parts[0], parts[1], parts[2]
            arr_x.append(float(x))
            arr_y.append(float(y))
            arr_z.append(float(z))

def draw_point(color : str, exp_id : int):
    global axes
    experiments_dir = "Sowing\\Particle"
    if os.path.isdir(experiments_dir):
        filename = f"points_{exp_id}.txt"
        filepath = os.path.join(experiments_dir, filename)
        if os.path.isfile(filepath):
            x, y, z = [], [], []
            get_data(x, y, z, filepath)
            axes.scatter(x, y, z, color=color, s=1)

def draw_pole(floder : str, fname : str, color : str, radius : int):
    global axes
    if os.path.isdir(floder):
        if fname.lower().endswith(".txt"):
            x, y, z = [], [], []
            get_data(x, y, z, os.path.join(floder, fname))
            for i in range(len(x) - 1):
                # Начальная и конечная точки центров
                start = np.array([x[i], y[i], z[i]])
                end = np.array([x[i+1], y[i+1], z[i+1]])
                # Вектор оси цилиндра
                axis = end - start
                length = np.linalg.norm(axis)
                if length == 0:
                    continue
                axis = axis / length
                # Базис для окружности, перпендикулярной оси OX
                perp1 = np.array([0, 1, 0])
                perp2 = np.array([0, 0, 1])
                # Генерация точек окружности
                theta = np.linspace(0, 2*np.pi, 16, endpoint=True)
                circle = radius * (np.cos(theta)[:, None] * perp1 + np.sin(theta)[:, None] * perp2)
                # Нижняя и верхняя окружности
                bottom_circle = start + circle
                top_circle = end + circle
                # Создание боковых граней
                verts = []
                for j in range(len(theta) - 1):
                    verts.append([bottom_circle[j], bottom_circle[j+1], top_circle[j+1], top_circle[j]])
                # Добавление последнего четырёхугольника для замыкания цилиндра
                verts.append([bottom_circle[-1], bottom_circle[0], top_circle[0], top_circle[-1]])
                # Отрисовка боковой поверхности
                axes.add_collection3d(Poly3DCollection(verts, facecolors=color, edgecolor='none', alpha=0.1))
                # Отрисовка нижнего основания
                axes.add_collection3d(Poly3DCollection([bottom_circle], facecolors=color, edgecolor='none', alpha=0.1))
                # Отрисовка верхнего основания
                axes.add_collection3d(Poly3DCollection([top_circle], facecolors=color, edgecolor='none', alpha=0.1))
                
fig = plt.figure()
axes = fig.add_subplot(projection='3d')
axes.set_xlabel('x')
axes.set_ylabel('y')
axes.set_zlabel('z')
axes.set_xlim(0, 1000)
axes.set_ylim(-500, 500)
axes.set_zlim(-500, 500)


with open(file="Draw_figure\\draw_point.json", mode='r') as file:
    data = json.load(file)
    exp_id = data["exp_id"]
    try_id = int(data["try_id"])

with open(file=f"Results\\experiment_{exp_id}.json", mode='r') as file:
    data = json.load(file)
    conf_id = data[try_id]["conf_id"]
    result = data[try_id]["result"]
    particles_count = data[try_id]["particles_count"]

with open(file=f"Sowing\\Config\\conf_{conf_id}.json", mode='r') as file:
    data = json.load(file)
    Bufer = data["Bufer"]
    Fractal = data["Fractal"]
    radius = data["Size"]
    points_start = data["Point"]
    
info  = {
    "exp_id": f"Results\\experiment_{exp_id}.json",
    "conf_id": f"Sowing\\Config\\conf_{conf_id}.json",
    "result": f"Experiments\\exp_{result}.txt",
    "Bufer": f"Sowing\\Bufer\\Bufer_{Bufer}.txt",
    "Fractal": f"Sowing\\Fractal\\Pole_{Fractal}.txt",
    "particles_start_position": f"Sowing\\Particle\\points_{points_start}.txt",
    "Size": radius,
    "particles_count": particles_count,
}

for key, value in info.items():
    print(f"{key:<30} : {value}")

draw_pole("Sowing\\Bufer", f"Bufer_{Bufer}.txt", 'r', radius)
draw_pole("Sowing\\Fractal", f"Pole_{Fractal}.txt", 'b', radius)
draw_point('g', points_start)

output_dir = f"images/Experiment_{exp_id}/im"
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, f"start_point_{try_id}.png")
plt.savefig(output_path)
plt.show()