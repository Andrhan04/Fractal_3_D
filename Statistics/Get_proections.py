import json
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import os

mx_step : int # максимальное количество шагов, которое было сделано в эксперименте, для которого строится гистограмма
conf_id : int # id конфигурации, для которой строится гистограмма
experiment_id : int # id эксперимента, для которого строится гистограмма
particles_count : int # количество частиц, которое было использовано в эксперименте, для которого строится гистограмма
Bufer : int # id буфера, который был использован в эксперименте, для которого строится гистограмма
Fractal : int # id фрактала, который был использован в эксперименте, для которого строится гистограмма
all_try : list # все id запусков эксперимента в папке Experiments к примеру финальное расположение точек с id запуска 1 будет в файle ../Experiments/exp_1.txt, а всех шагов в папке ../Experiments/Experiment_1/step_0.txt, ../Experiments/Experiment_1/step_1.txt 
mx_x : float # максимальное значение x, которое может принимать частица
mx_y : float # максимальное значение y, которое может принимать частица
mx_z : float # максимальное значение z, которое может принимать частица
mn_x : float # минимальное значение x, которое может принимать частица
mn_y : float # минимальное значение y, которое может принимать частица
mn_z : float # минимальное значение z, которое может принимать частица
rad : float # радиус окружности, в которой находится частица
linney : float # толщина линии для отрисовки фрактала и буфера

def get_parametrs():
    global conf_id, experiment_id, particles_count, Bufer, Fractal, all_try, mx_step, mx_x, mx_y, mx_z, mn_x, mn_y, mn_z, rad, linney
    with open("input.json", 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    exp_id = data["exp_id"]
    linney = data["linney"]
    
    with open(f"../Results/experiment_{exp_id}.json", 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    all_try = []
    for item in data:
        all_try.append(item["result"])
        conf_id = item["conf_id"]
        experiment_id = item["experiment_id"]
        particles_count = item["particles_count"]
        mx_step = item["steps"]
    
    with open(f"../Sowing/Config/Conf_{conf_id}.json", 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    Bufer = data["Bufer"]
    Fractal = data["Fractal"]
    sz = data["Size"]
    rad = sz
    # linney = sz
    
    with open(f"../Sowing/Fractal/Pole_{Fractal}.txt", 'r', encoding='utf-8') as file:
        data = file.readlines()
    
    mn_x = float(data[0].split(' ')[0])
    mn_y = float(data[0].split(' ')[1]) - sz
    mn_z = float(data[0].split(' ')[2]) - sz
    mx_x = float(data[-1].split(' ')[0])
    mx_y = float(data[-1].split(' ')[1]) + sz
    mx_z = float(data[-1].split(' ')[2]) + sz
    

def get_points(step : int):
    global experiment_id, mx_step
    if step == mx_step:  # Финальное положение частиц
        file_path = f"../Experiments/exp_{experiment_id}.txt"
    else:
        file_path = f"../Experiments/Experiment_{experiment_id}/step_{step}.txt"
    
    # Читаем координаты частиц
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    all_x = []
    all_y = []
    all_z = []
    # Извлекаем координаты (пропускаем заголовок, если он есть)
    for line in lines:
        parts = line.strip().split()
        if len(parts) >= 3:
            try:
                x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                all_x.append(x)
                all_y.append(y)
                all_z.append(z)
            except ValueError:
                continue
    return all_x, all_y, all_z

def get_Fractal_points():
    global Fractal
    with open(f"../Sowing/Fractal/Pole_{Fractal}.txt", 'r', encoding='utf-8') as file:
        data = file.readlines()
    all_x = []
    all_y = []
    all_z = []
    # Извлекаем координаты (пропускаем заголовок, если он есть)
    for line in data:
        parts = line.strip().split()
        if len(parts) >= 3:
            try:
                x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                all_x.append(x)
                all_y.append(y)
                all_z.append(z)
            except ValueError:
                continue
    return all_x, all_y, all_z

def get_Bufer_points():
    global Bufer
    with open(f"../Sowing/Bufer/Bufer_{Bufer}.txt", 'r', encoding='utf-8') as file:
        data = file.readlines()
    all_x = []
    all_y = []
    all_z = []
    # Извлекаем координаты (пропускаем заголовок, если он есть)
    for line in data:
        parts = line.strip().split()
        if len(parts) >= 3:
            try:
                x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                all_x.append(x)
                all_y.append(y)
                all_z.append(z)
            except ValueError:
                continue
    return all_x, all_y, all_z


def draw_cylinder(ax, start, end, radius, color, alpha=0.6):
    """
    Рисует цилиндр между двумя точками
    alpha: прозрачность (от 0.0 до 1.0)
    """
    # Вектор оси цилиндра
    axis = end - start
    length = np.linalg.norm(axis)
    if length == 0:
        return
    axis = axis / length
    
    # Базис для окружности, перпендикулярной оси
    # Создаем перпендикулярные векторы к оси
    if np.abs(axis[0]) > 0.5:
        perp1 = np.array([0, 1, 0])
    else:
        perp1 = np.array([1, 0, 0])
    
    perp1 = perp1 - np.dot(perp1, axis) * axis
    perp1 = perp1 / np.linalg.norm(perp1)
    perp2 = np.cross(axis, perp1)
    
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
    ax.add_collection3d(Poly3DCollection(verts, facecolors=color, edgecolor='none', alpha=alpha))
    
    # Отрисовка нижнего основания
    ax.add_collection3d(Poly3DCollection([bottom_circle], facecolors=color, edgecolor='none', alpha=alpha))
    
    # Отрисовка верхнего основания
    ax.add_collection3d(Poly3DCollection([top_circle], facecolors=color, edgecolor='none', alpha=alpha))


def draw_pole(ax, folder, filename, color, radius, is_3d=False, alpha=0.7):
    """
    Рисует все цилиндры из файла
    is_3d: True для 3D осей, False для 2D осей
    alpha: прозрачность (от 0.0 до 1.0)
    """
    filepath = os.path.join(folder, filename)
    if os.path.exists(filepath):
        points = []
        with open(filepath, 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.strip().split()
                if len(parts) >= 3:
                    try:
                        x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                        points.append(np.array([x, y, z]))
                    except ValueError:
                        continue
        
        # Для 2D осей рисуем простые линии между точками
        if not is_3d:
            x_coords = [p[0] for p in points]
            y_coords = [p[1] for p in points]
            z_coords = [p[2] for p in points]
            
            # Определяем какие оси использовать для отрисовки
            x_label = ax.get_xlabel().lower()
            y_label = ax.get_ylabel().lower()
            
            # Рисуем линии между последовательными точками
            for i in range(len(points) - 1):
                if 'x' in x_label and 'y' in y_label:
                    # Проекция OXY: используем X и Y - рисуем прямоугольник между точками с боковыми сторонами параллельными оси X
                    x1, y1 = x_coords[i], y_coords[i]
                    x2, y2 = x_coords[i+1], y_coords[i+1]
                    
                    # Рисуем прямоугольник между точками с горизонтальными боковыми сторонами
                    # Боковые стороны параллельны оси X
                    corners = np.array([
                        [x1, y1 - rad],    # левый нижний
                        [x1, y1 + rad],    # левый верхний
                        [x2, y2 + rad],    # правый верхний
                        [x2, y2 - rad]     # правый нижний
                    ])
                    
                    # Рисуем прямоугольник
                    ax.add_patch(plt.Polygon(corners, color=color, alpha=alpha*0.5, fill=True))
                elif 'x' in x_label and 'z' in y_label:
                    # Проекция OXZ: используем X и Z - рисуем прямоугольник между точками размером 2*rad
                    x1, z1 = x_coords[i], z_coords[i]
                    x2, z2 = x_coords[i+1], z_coords[i+1]
                    
                    # Рисуем прямоугольник между точками (боковая поверхность цилиндра)
                    # Вычисляем вектор направления и длину
                    dx = x2 - x1
                    dz = z2 - z1
                    length = np.sqrt(dx**2 + dz**2)
                    if length > 0:
                        # Нормализуем вектор
                        dx /= length
                        dz /= length
                        
                        # Перпендикулярный вектор для ширины прямоугольника
                        perp_dx = -dz * rad
                        perp_dz = dx * rad
                        
                        # Углы прямоугольника
                        corners = np.array([
                            [x1 - perp_dx, z1 - perp_dz],
                            [x1 + perp_dx, z1 + perp_dz],
                            [x2 + perp_dx, z2 + perp_dz],
                            [x2 - perp_dx, z2 - perp_dz]
                        ])
                        
                        # Рисуем прямоугольник
                        ax.add_patch(plt.Polygon(corners, color=color, alpha=alpha*0.5, fill=True))
                elif 'y' in x_label and 'z' in y_label:
                    # Проекция OYZ: используем Y и Z - рисуем окружности с радиусом rad и соединяем прямоугольниками
                    y1, z1 = y_coords[i], z_coords[i]
                    y2, z2 = y_coords[i+1], z_coords[i+1]
                    
                    # Рисуем окружности вокруг точек (закрашенные)
                    circle1 = plt.Circle((y1, z1), rad, color=color, fill=True, alpha=alpha*0.2)
                    circle2 = plt.Circle((y2, z2), rad, color=color, fill=True, alpha=alpha*0.2)
                    ax.add_patch(circle1)
                    ax.add_patch(circle2)
                    
                    # Рисуем прямоугольник между точками (боковая поверхность цилиндра)
                    # Вычисляем вектор направления и длину
                    dy = y2 - y1
                    dz = z2 - z1
                    length = np.sqrt(dy**2 + dz**2)
                    if length > 0:
                        # Нормализуем вектор
                        dy /= length
                        dz /= length
                        
                        # Перпендикулярный вектор для ширины прямоугольника
                        perp_dy = -dz * rad
                        perp_dz = dy * rad
                        
                        # Углы прямоугольника
                        corners = np.array([
                            [y1 - perp_dy, z1 - perp_dz],
                            [y1 + perp_dy, z1 + perp_dz],
                            [y2 + perp_dy, z2 + perp_dz],
                            [y2 - perp_dy, z2 - perp_dz]
                        ])
                        
                        # Рисуем прямоугольник
                        ax.add_patch(plt.Polygon(corners, color=color, alpha=alpha*0.5, fill=True))
        else:
            # Для 3D осей рисуем полноценные цилиндры
            for i in range(len(points) - 1):
                draw_cylinder(ax, points[i], points[i+1], radius, color, alpha)


def get_single_run_particles(step):
    """
    Получает координаты частиц из одного запуска эксперимента для указанного шага
    Использует первый запуск из массива all_try
    """
    global all_try, mx_step
    
    if not all_try:
        return [], [], []
    
    # Берем первый запуск из массива
    try_id = all_try[0]
    
    # Определяем путь к файлу с данными
    if step == mx_step:  # Финальное положение частиц
        file_path = f"../Experiments/exp_{try_id}.txt"
    else:
        file_path = f"../Experiments/Experiment_{try_id}/step_{step}.txt"
    
    all_x = []
    all_y = []
    all_z = []
    
    # Читаем координаты частиц
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
        
        for line in lines:
            parts = line.strip().split()
            if len(parts) >= 3:
                try:
                    x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                    all_x.append(x)
                    all_y.append(y)
                    all_z.append(z)
                except ValueError:
                    continue
    except FileNotFoundError:
        print(f"Файл не найден: {file_path}")
    
    return all_x, all_y, all_z


def create_projection_plots(step):
    """
    Создает проекции частиц, фрактала и буфера на плоскости OXY, OXZ, OYZ
    """
    global experiment_id, mx_step, rad, Bufer, Fractal
    
    # Создаем директорию для сохранения, если её нет
    save_dir = f"../images/Experiment_{experiment_id}/Projections"
    os.makedirs(save_dir, exist_ok=True)
    
    # Получаем координаты частиц из одного запуска
    part_x, part_y, part_z = get_single_run_particles(step)
    
    # Создаем фигуру с тремя подграфиками для проекций
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle(f'Проекции эксперимента {experiment_id}, Шаг {step}\nЧастицы: {len(part_x)}, Запуск: {all_try[0] if all_try else "N/A"}', 
                 fontsize=16)
    
    # Проекция OXY
    axes[0, 0].set_xlabel('Координата X')
    axes[0, 0].set_ylabel('Координата Y')
    axes[0, 0].set_title('Проекция OXY')
    axes[0, 0].grid(True, alpha=0.3)
    axes[0, 0].scatter(part_x, part_y, alpha=0.8, color='green', s=10, label='Частицы')
    # Рисуем фрактал и буфер как линии (2D) - Буфер красным, Фрактал синим
    draw_pole(axes[0, 0], "../Sowing/Bufer", f"Bufer_{Bufer}.txt", 'red', linney, is_3d=False, alpha=0.5)
    draw_pole(axes[0, 0], "../Sowing/Fractal", f"Pole_{Fractal}.txt", 'blue', linney, is_3d=False, alpha=0.5)
    axes[0, 0].legend()
    
    # Проекция OXZ
    axes[0, 1].set_xlabel('Координата X')
    axes[0, 1].set_ylabel('Координата Z')
    axes[0, 1].set_title('Проекция OXZ')
    axes[0, 1].grid(True, alpha=0.3)
    axes[0, 1].scatter(part_x, part_z, alpha=0.8, color='green', s=10, label='Частицы')
    draw_pole(axes[0, 1], "../Sowing/Bufer", f"Bufer_{Bufer}.txt", 'red', linney, is_3d=False, alpha=0.5)
    draw_pole(axes[0, 1], "../Sowing/Fractal", f"Pole_{Fractal}.txt", 'blue', linney, is_3d=False, alpha=0.5)
    axes[0, 1].legend()
    
    # Проекция OYZ
    axes[1, 0].set_xlabel('Координата Y')
    axes[1, 0].set_ylabel('Координата Z')
    axes[1, 0].set_title('Проекция OYZ')
    axes[1, 0].grid(True, alpha=0.3)
    axes[1, 0].scatter(part_y, part_z, alpha=0.8, color='green', s=10, label='Частицы')
    draw_pole(axes[1, 0], "../Sowing/Bufer", f"Bufer_{Bufer}.txt", 'red', linney, is_3d=False, alpha=0.5)
    draw_pole(axes[1, 0], "../Sowing/Fractal", f"Pole_{Fractal}.txt", 'blue', linney, is_3d=False, alpha=0.5)
    axes[1, 0].legend()
    
    # 3D проекция
    axes[1, 1].remove()  # Убираем четвертый subplot
    ax_3d = fig.add_subplot(2, 2, 4, projection='3d')
    ax_3d.scatter(part_x, part_y, part_z, alpha=0.8, color='green', s=10, label='Частицы')
    # Рисуем фрактал и буфер в 3D (полноценные цилиндры) с прозрачностью
    draw_pole(ax_3d, "../Sowing/Bufer", f"Bufer_{Bufer}.txt", 'red', rad, is_3d=True, alpha=0.3)
    draw_pole(ax_3d, "../Sowing/Fractal", f"Pole_{Fractal}.txt", 'blue', rad, is_3d=True, alpha=0.3)
    ax_3d.set_xlabel('Координата X')
    ax_3d.set_ylabel('Координата Y')
    ax_3d.set_zlabel('Координата Z')
    ax_3d.set_title('3D Проекция')
    ax_3d.legend()
    
    plt.tight_layout()
    
    # Сохраняем проекции
    if step == mx_step:
        save_path = os.path.join(f"../images/Experiment_{experiment_id}/", f'final_projections.png')
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        save_path = os.path.join(save_dir, f'step_{step}.png')
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
    else:
        save_path = os.path.join(save_dir, f'step_{step}.png')
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
    
    plt.close()
    print(f"Проекции для шага {step} сохранены: {save_path}")


def create_all_projections():
    """
    Создает проекции для всех шагов эксперимента
    """
    get_parametrs()
    
    print(f"Создание проекций для эксперимента {experiment_id}")
    print(f"Максимальное количество шагов: {mx_step}")
    print(f"Количество запусков: {len(all_try)}")
    
    # Создаем проекции для всех шагов
    for step in range(100000, mx_step + 1, 100000):
        try:
            create_projection_plots(step)
        except Exception as e:
            print(f"Ошибка при создании проекций для шага {step}: {e}")
    
    print("Создание всех проекций завершено!")


if __name__ == "__main__":
    create_all_projections()

