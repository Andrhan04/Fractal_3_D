import matplotlib.pyplot as plt

id_pole = 0

def get_data(arr_x : list, arr_y : list, arr_z : list, path: str):
    with open(file=path, mode='r') as file:
        for line in file:
            x = line.split()[0]
            y = line.split()[1]
            z = line.split()[2]
            arr_x.append(float(x))
            arr_y.append(float(y))
            arr_z.append(float(z))

        


fig = plt.figure()
axes : plt = fig.add_subplot(projection='3d')

axes.set_xlabel('x')
axes.set_ylabel('y')
axes.set_zlabel('z')

x = []
y = []
z = []

get_data(x,y,z, "Sowing\\Fractal\\Pole_1.txt")
axes.plot(x,y,z, color = 'b')

x = []
y = []
z = []

get_data(x,y,z, "Sowing\\Bufer\\Bufer_0.txt")
axes.plot(x,y,z, color = 'r')

plt.show()