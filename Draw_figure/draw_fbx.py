import os
import json
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.pyplot as plt

# Для Python 3.14 используем альтернативный способ работы с FBX через pyfbx
# (pip install pyfbx) или, если её нет, пишем в формате OBJ
HAVE_FBX = False
try:
    # Попытка импорта pyfbx (если установлена)
    import pyfbx
    from pyfbx import FBXManager, FBXScene, FBXMesh, FBXNode, FBXVector4
    HAVE_FBX = True
except ImportError:
    pass


def get_project_paths():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(base_dir)
    return base_dir, project_root


def get_data(arr_x, arr_y, arr_z, path):
    with open(path, mode="r") as file:
        for line in file:
            parts = line.split()
            if len(parts) < 3:
                continue
            x, y, z = parts[0], parts[1], parts[2]
            arr_x.append(float(x))
            arr_y.append(float(y))
            arr_z.append(float(z))


def draw_point(color, exp_id, axes, points_store, project_root):
    experiments_dir = os.path.join(project_root, "Experiments")
    if os.path.isdir(experiments_dir):
        filename = f"exp_{exp_id}.txt"
        filepath = os.path.join(experiments_dir, filename)
        if os.path.isfile(filepath):
            x, y, z = [], [], []
            get_data(x, y, z, filepath)
            axes.scatter(x, y, z, color=color, s=5)
            for px, py, pz in zip(x, y, z):
                points_store.append((px, py, pz))


def draw_pole(folder, fname, color, radius, axes, cylinders_store, project_root):
    folder_path = os.path.join(project_root, folder)
    if not os.path.isdir(folder_path):
        return
    if not fname.lower().endswith(".txt"):
        return
    x, y, z = [], [], []
    get_data(x, y, z, os.path.join(folder_path, fname))
    for i in range(len(x) - 1):
        start = np.array([x[i], y[i], z[i]])
        end = np.array([x[i + 1], y[i + 1], z[i + 1]])
        axis = end - start
        length = np.linalg.norm(axis)
        if length == 0:
            continue
        axis = axis / length
        perp1 = np.array([0.0, 1.0, 0.0])
        perp2 = np.array([0.0, 0.0, 1.0])
        theta = np.linspace(0.0, 2.0 * np.pi, 16, endpoint=True)
        circle = radius * (np.cos(theta)[:, None] * perp1 + np.sin(theta)[:, None] * perp2)
        bottom_circle = start + circle
        top_circle = end + circle
        cylinders_store.append((bottom_circle, top_circle))
        verts = []
        for j in range(len(theta) - 1):
            verts.append(
                [
                    bottom_circle[j],
                    bottom_circle[j + 1],
                    top_circle[j + 1],
                    top_circle[j],
                ]
            )
        verts.append(
            [
                bottom_circle[-1],
                bottom_circle[0],
                top_circle[0],
                top_circle[-1],
            ]
        )
        axes.add_collection3d(Poly3DCollection(verts, facecolors=color, edgecolor="none", alpha=0.1))
        axes.add_collection3d(Poly3DCollection([bottom_circle], facecolors=color, edgecolor="none", alpha=0.1))
        axes.add_collection3d(Poly3DCollection([top_circle], facecolors=color, edgecolor="none", alpha=0.1))


def export_fbx(path, cylinders, points):
    if not HAVE_FBX:
        # Если pyfbx не установлена, экспортируем в OBJ
        export_obj(path.replace(".fbx", ".obj"), cylinders, points)
        return
    manager = FBXManager()
    scene = FBXScene.Create(manager, "Scene")
    mesh = FBXMesh.Create(manager, "SceneMesh")
    node = FBXNode.Create(manager, "MeshNode")
    node.SetNodeAttribute(mesh)
    scene.GetRootNode().AddChild(node)
    vertices = []
    for bottom, top in cylinders:
        for v in bottom:
            vertices.append(v)
        for v in top:
            vertices.append(v)
    for px, py, pz in points:
        vertices.append(np.array([px, py, pz]))
    mesh.InitControlPoints(len(vertices))
    for i, v in enumerate(vertices):
        mesh.SetControlPointAt(FBXVector4(float(v[0]), float(v[1]), float(v[2])), i)
    index_offset = 0
    for bottom, top in cylinders:
        n = bottom.shape[0]
        for j in range(n - 1):
            mesh.BeginPolygon()
            mesh.AddPolygon(index_offset + j)
            mesh.AddPolygon(index_offset + j + 1)
            mesh.AddPolygon(index_offset + n + j + 1)
            mesh.AddPolygon(index_offset + n + j)
            mesh.EndPolygon()
        mesh.BeginPolygon()
        mesh.AddPolygon(index_offset + n - 1)
        mesh.AddPolygon(index_offset + 0)
        mesh.AddPolygon(index_offset + n + 0)
        mesh.AddPolygon(index_offset + n + n - 1)
        mesh.EndPolygon()
        index_offset += 2 * n
    os.makedirs(os.path.dirname(path), exist_ok=True)
    scene.Save(path)
    manager.Destroy()


def export_obj(path, cylinders, points):
    # Экспорт в OBJ-формате, если pyfbx не установлена
    vertices = []
    faces = []
    for bottom, top in cylinders:
        n = bottom.shape[0]
        base = len(vertices) + 1
        for v in bottom:
            vertices.append(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}")
        for v in top:
            vertices.append(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}")
        for j in range(n - 1):
            faces.append(f"f {base + j} {base + j + 1} {base + n + j + 1} {base + n + j}")
        faces.append(f"f {base + n - 1} {base + 0} {base + n + 0} {base + n + n - 1}")
    for px, py, pz in points:
        vertices.append(f"v {px:.6f} {py:.6f} {pz:.6f}")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write("\n".join(vertices) + "\n")
        f.write("\n".join(faces) + "\n")


def main():
    base_dir, project_root = get_project_paths()
    fig = plt.figure()
    axes = fig.add_subplot(projection="3d")
    axes.set_xlabel("x")
    axes.set_ylabel("y")
    axes.set_zlabel("z")
    axes.set_xlim(0, 1000)
    axes.set_ylim(-500, 500)
    axes.set_zlim(-500, 500)
    draw_point_config = os.path.join(base_dir, "draw_point.json")
    with open(draw_point_config, mode="r") as file:
        data = json.load(file)
        exp_id = data["exp_id"]
        try_id = int(data["try_id"])
    results_path = os.path.join(project_root, "Results", f"experiment_{exp_id}.json")
    with open(results_path, mode="r") as file:
        data = json.load(file)
        conf_id = data[try_id]["conf_id"]
        result = data[try_id]["result"]
    config_path = os.path.join(project_root, "Sowing", "Config", f"conf_{conf_id}.json")
    with open(config_path, mode="r") as file:
        data = json.load(file)
        bufer_id = data["Bufer"]
        fractal_id = data["Fractal"]
        radius = data["Size"]
    cylinders = []
    points = []
    draw_pole(os.path.join("Sowing", "Bufer"), f"Bufer_{bufer_id}.txt", "r", radius, axes, cylinders, project_root)
    draw_pole(os.path.join("Sowing", "Fractal"), f"Pole_{fractal_id}.txt", "b", radius, axes, cylinders, project_root)
    draw_point("g", result, axes, points, project_root)
    images_dir = os.path.join(project_root, "images", f"Experiment_{exp_id}")
    output_path = os.path.join(images_dir, f"try_{try_id}.fbx")
    export_fbx(output_path, cylinders, points)
    plt.show()


if __name__ == "__main__":
    main()
