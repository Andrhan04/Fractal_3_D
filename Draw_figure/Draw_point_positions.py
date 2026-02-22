import os
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.pyplot as plt
id_pole = 0

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

fig = plt.figure()
axes = fig.add_subplot(projection='3d')

axes.set_xlabel('x')
axes.set_ylabel('y')
axes.set_zlabel('z')

experiments_dir = "Experiments"
if os.path.isdir(experiments_dir):
    filename = "exp_6.txt"
    filepath = os.path.join(experiments_dir, filename)
    if os.path.isfile(filepath):
        x, y, z = [], [], []
        get_data(x, y, z, filepath)
        axes.scatter(x, y, z, color='g')
        
# Draw lines from Sowing\Bufer\ and Sowing\Fractal\
Bufer = "Sowing\\Bufer"
if os.path.isdir(Bufer):
    fname = "Bufer_0.txt"
    if fname.lower().endswith(".txt"):
        x, y, z = [], [], []
        get_data(x, y, z, os.path.join(Bufer, fname))
        # Build cylinder with radius 100 between consecutive centers
        radius = 100
        for i in range(len(x) - 1):
            # Start and end centers
            start = np.array([x[i], y[i], z[i]])
            end = np.array([x[i+1], y[i+1], z[i+1]])
            # Cylinder axis vector
            axis = end - start
            length = np.linalg.norm(axis)
            if length == 0:
                continue
            axis = axis / length
            # Orthonormal basis
            not_axis = np.array([1, 0, 0])
            if np.allclose(axis, not_axis) or np.allclose(axis, -not_axis):
                not_axis = np.array([0, 1, 0])
            perp1 = np.cross(axis, not_axis)
            perp1 = perp1 / np.linalg.norm(perp1)
            perp2 = np.cross(axis, perp1)
            # Generate circle points
            theta = np.linspace(0, 2*np.pi, 16, endpoint=True)
            circle = np.array([radius * (np.cos(theta)[:, None] * perp1 + np.sin(theta)[:, None] * perp2)])
            # Bottom and top circles
            bottom_circle = start + circle[0]
            top_circle = end + circle[0]
            # Create side faces
            verts = []
            for j in range(len(theta) - 1):
                verts.append([bottom_circle[j], bottom_circle[j+1], top_circle[j+1], top_circle[j]])
            # Add last quad closing the cylinder
            verts.append([bottom_circle[-1], bottom_circle[0], top_circle[0], top_circle[-1]])
            # Draw side surface
            axes.add_collection3d(Poly3DCollection(verts, facecolors='r', edgecolor='none', alpha=0.1))
            # Draw bottom cap
            axes.add_collection3d(Poly3DCollection([bottom_circle], facecolors='r', edgecolor='none', alpha=0.1))
            # Draw top cap
            axes.add_collection3d(Poly3DCollection([top_circle], facecolors='r', edgecolor='none', alpha=0.1))

Fractal = "Sowing\\Fractal"
if os.path.isdir(Fractal):
    fname = "Pole_0.txt"
    if fname.lower().endswith(".txt"):
        x, y, z = [], [], []
        get_data(x, y, z, os.path.join(Fractal, fname))
        # Build cylinder with radius 100 between consecutive centers
        radius = 100
        for i in range(len(x) - 1):
            # Start and end centers
            start = np.array([x[i], y[i], z[i]])
            end = np.array([x[i+1], y[i+1], z[i+1]])
            # Cylinder axis vector
            axis = end - start
            length = np.linalg.norm(axis)
            if length == 0:
                continue
            axis = axis / length
            # Orthonormal basis
            not_axis = np.array([1, 0, 0])
            if np.allclose(axis, not_axis) or np.allclose(axis, -not_axis):
                not_axis = np.array([0, 1, 0])
            perp1 = np.cross(axis, not_axis)
            perp1 = perp1 / np.linalg.norm(perp1)
            perp2 = np.cross(axis, perp1)
            # Generate circle points
            theta = np.linspace(0, 2*np.pi, 16, endpoint=True)
            circle = np.array([radius * (np.cos(theta)[:, None] * perp1 + np.sin(theta)[:, None] * perp2)])
            # Bottom and top circles
            bottom_circle = start + circle[0]
            top_circle = end + circle[0]
            # Create side faces
            verts = []
            for j in range(len(theta) - 1):
                verts.append([bottom_circle[j], bottom_circle[j+1], top_circle[j+1], top_circle[j]])
            # Add last quad closing the cylinder
            verts.append([bottom_circle[-1], bottom_circle[0], top_circle[0], top_circle[-1]])
            # Draw side surface
            axes.add_collection3d(Poly3DCollection(verts, facecolors='b', edgecolor='none', alpha=0.1))
            # Draw bottom cap
            axes.add_collection3d(Poly3DCollection([bottom_circle], facecolors='b', edgecolor='none', alpha=0.1))
            # Draw top cap
            axes.add_collection3d(Poly3DCollection([top_circle], facecolors='b', edgecolor='none', alpha=0.1))

plt.show()