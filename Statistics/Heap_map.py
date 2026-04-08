import json
import matplotlib.pyplot as plt
import numpy as np
import os
import math

mx_step : int # максимальное количество шагов, которое было сделано в эксперименте, для которого строится гистограмма
num_categories : int # количество категорий (интервалов) для гистограммы, которое задается в input.json
conf_id : int # id конфигурации, для которой строится гистограмма
experiment_id : int # id эксперимента, для которого строится гистограмма
particles_count : int # количество частиц, которое было использовано в эксперименте, для которого строится гистограмма
Bufer : int # id буфера, который был использован в эксперименте, для которого строится гистограмма
Fractal : int # id фрактала, который был использован в эксперименте, для которого строится гистограмма
all_try : list # все id запусков эксперимента в папке Experiments к примеру финальное расположение точек с id запуска 1 будет в файле ../Experiments/exp_1.txt, а всех шагов в папке ../Experiments/Experiment_1/step_0.txt, ../Experiments/Experiment_1/step_1.txt 
mx_x : float # максимальное значение x, которое может принимать частица
mx_y : float # максимальное значение y, которое может принимать частица
mx_z : float # максимальное значение z, которое может принимать частица
mn_x : float # минимальное значение x, которое может принимать частица
mn_y : float # минимальное значение y, которое может принимать частица
mn_z : float # минимальное значение z, которое может принимать частица


def get_parametrs():
    global num_categories, conf_id, experiment_id, particles_count, Bufer, Fractal, all_try, mx_step, mx_x, mx_y, mx_z, mn_x, mn_y, mn_z
    with open("input.json", 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    exp_id = data["exp_id"]
    num_categories = data["num_categories"]
    
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
    
    with open(f"../Sowing/Fractal/Pole_{Fractal}.txt", 'r', encoding='utf-8') as file:
        data = file.readlines()
    
    mn_x = float(data[0].split(' ')[0])
    mn_y = float(data[0].split(' ')[1]) - sz
    mn_z = float(data[0].split(' ')[2]) - sz
    mx_x = float(data[0].split(' ')[0])
    mx_y = float(data[0].split(' ')[1]) + sz
    mx_z = float(data[0].split(' ')[2]) + sz
    
    for i in range(len(data)):
        mn_x = min(mn_x ,float(data[i].split(' ')[0]))
        mn_y = min(mn_y ,float(data[i].split(' ')[1]) - sz)
        mn_z = min(mn_z ,float(data[i].split(' ')[2]) - sz)
        mx_x = max(mx_x ,float(data[i].split(' ')[0]))
        mx_y = max(mx_y ,float(data[i].split(' ')[1]) + sz)
        mx_z = max(mx_z ,float(data[i].split(' ')[2]) + sz)


def calculate_bins_count(min_val, max_val, num_categories):
    """
    Рассчитывает размер шага для гистограммы
    на основе количества категорий num_categories
    """
    range_size = max_val - min_val
    if range_size <= 0 or num_categories <= 0:
        return range_size / 10.0  # значение по умолчанию, если данные некорректны
    
    # Размер шага = диапазон / количество категорий
    step_size = range_size / num_categories
    return step_size


def create_heap_map(step):
    """
    Создает тепловую карту распределения частиц для шага step
    """
    # Читаем данные частиц для указанного шага
    all_x, all_y, all_z = [], [], []
    
    for try_id in all_try:
        if step == mx_step:  # Финальное положение частиц
            file_path = f"../Experiments/exp_{try_id}.txt"
        else:
            file_path = f"../Experiments/Experiment_{try_id}/step_{step}.txt"
        if not os.path.exists(file_path):
            continue
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.split()
                if len(parts) < 3:
                    continue
                try:
                    x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                    all_x.append(x)
                    all_y.append(y)
                    all_z.append(z)
                except ValueError:
                    continue
    
    if not all_x:
        print(f"Нет данных для шага {step}")
        return
    
    # Создаем фигуру с тремя подграфиками для разных плоскостей
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))
    fig.suptitle(f'Тепловые карты распределения частиц (Шаг {step})', fontsize=16)
    
    # Создаем тепловые карты для трех плоскостей
    create_heatmap_2d(axes[0], all_x, all_y, mn_x, mx_x, mn_y, mx_y, num_categories, 'XY')
    create_heatmap_2d(axes[1], all_x, all_z, mn_x, mx_x, mn_z, mx_z, num_categories, 'XZ')
    create_heatmap_2d(axes[2], all_y, all_z, mn_y, mx_y, mn_z, mx_z, num_categories, 'YZ')
    
    # Сохраняем тепловую карту в указанной структуре папок
    save_dir = f"../images/Experiment_{experiment_id}/Heap_map/"
    os.makedirs(save_dir, exist_ok=True)
    save_path = os.path.join(save_dir, f'step_{step}.png')
    plt.tight_layout()
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    
    # Дополнительно сохраняем последнюю итерацию в корневую папку
    if step == mx_step:
        final_save_dir = f"../images/Experiment_{experiment_id}/"
        os.makedirs(final_save_dir, exist_ok=True)
        final_save_path = os.path.join(final_save_dir, 'heatmap.png')
        plt.savefig(final_save_path, dpi=150, bbox_inches='tight')
        print(f"Тепловая карта последней итерации сохранена: {final_save_path}")
    
    plt.close()
    
    print(f"Тепловая карта для шага {step} сохранена: {save_path}")


def create_heatmap_2d(ax, data_x, data_y, min_x, max_x, min_y, max_y, num_bins, plane_name):
    """
    Создает тепловую карту для 2D плоскости
    """
    # Создаем 2D гистограмму
    heatmap, xedges, yedges = np.histogram2d(data_x, data_y, 
                                             bins=num_bins, 
                                             range=[[min_x, max_x], [min_y, max_y]])
    
    # Нормализуем данные для лучшего визуального представления
    if heatmap.max() > 0:
        heatmap = heatmap / heatmap.max()
    
    # Отображаем тепловую карту
    im = ax.imshow(heatmap.T, origin='lower', aspect='auto', extent=[min_x, max_x, min_y, max_y], 
                   cmap='hot_r', interpolation='nearest')
    
    ax.set_xlabel(f'{plane_name[0]} координата')
    ax.set_ylabel(f'{plane_name[1]} координата')
    ax.set_title(f'Плоскость {plane_name}')
    
    # Добавляем цветовую шкалу
    plt.colorbar(im, ax=ax, label='Относительная плотность')
    
    # Добавляем сетку
    ax.grid(True, alpha=0.3)
    

def create_all_heatmaps():
    """
    Создает тепловые карты для всех шагов эксперимента
    """
    get_parametrs()
    
    print(f"Создание тепловых карт для эксперимента {experiment_id}")
    print(f"Максимальное количество шагов: {mx_step}")
    print(f"Количество запусков: {len(all_try)}")
    print(f"Количество категорий (num_categories): {num_categories}")
    print(f"Диапазоны: X=[{mn_x:.2f}, {mx_x:.2f}], Y=[{mn_y:.2f}, {mx_y:.2f}], Z=[{mn_z:.2f}, {mx_z:.2f}]")
    
    # Создаем тепловые карты для всех шагов
    # Шаги начинаются с 0 и идут до mx_step (включительно)
    for step in range(100000, mx_step + 1, 100000):
        try:
            create_heap_map(step)
        except Exception as e:
            print(f"Ошибка при создании тепловой карты для шага {step}: {e}")
    
    print("Создание всех тепловых карт завершено!")


if __name__ == "__main__":
    create_all_heatmaps()
