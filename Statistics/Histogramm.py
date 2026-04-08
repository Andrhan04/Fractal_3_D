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


def create_histogram(step):
    """
    Строит гистограмму для указанного шага эксперимента
    """
    # Создаем директорию для сохранения, если её нет
    save_dir = f"../images/Experiment_{experiment_id}/Hist"
    os.makedirs(save_dir, exist_ok=True)
    
    # Собираем все координаты частиц для данного шага по всем запускам
    all_x = []
    all_y = []
    all_z = []
    
    for try_id in all_try:
        # Определяем путь к файлу с данными
        if step == mx_step:  # Финальное положение частиц
            file_path = f"../Experiments/exp_{try_id}.txt"
        else:
            file_path = f"../Experiments/Experiment_{try_id}/step_{step}.txt"
        
        # Читаем координаты частиц
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
        
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
    
    print("----------------------------------------------------")
    
    # Рассчитываем размер шага для каждой оси
    x_step_size = (mx_x - mn_x) / num_categories
    y_step_size = (mx_y - mn_y) / num_categories
    z_step_size = (mx_z - mn_z) / num_categories
    
    # Создаем массивы для подсчета частиц в каждом интервале
    x_bins = [0.0] * num_categories
    y_bins = [0.0] * num_categories
    z_bins = [0.0] * num_categories
    
    # Заполняем бины
    for i in range(len(all_x)):
        try:
            x, y, z = all_x[i], all_y[i], all_z[i]
            
            # Определяем индекс бина для каждой координаты
            x_bin_idx = min(int((x - mn_x) / x_step_size), num_categories - 1)
            y_bin_idx = min(int((y - mn_y) / y_step_size), num_categories - 1)
            z_bin_idx = min(int((z - mn_z) / z_step_size), num_categories - 1)
            
            x_bins[x_bin_idx] += 1.0
            y_bins[y_bin_idx] += 1.0
            z_bins[z_bin_idx] += 1.0
            
        except Exception as e:
            print(e)
            print(x, y, z)
            print(mn_x, mn_y, mn_z)
            print(f"X step size: {x_step_size}, Y step size: {y_step_size}, Z step size: {z_step_size}")
            print(int((x - mn_x) / x_step_size), int((y - mn_y) / y_step_size), int((z - mn_z) / z_step_size))
            raise ValueError("Invalid coordinate values")
    
    # return
    # Создаем фигуру с тремя подграфиками
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    if step == mx_step:
        fig.suptitle(f'Эксперимент {experiment_id}, Финальный шаг (шаг {step})\nЧастицы: {particles_count}, Запуски: {len(all_try)}', 
                     fontsize=16)
    else:
        fig.suptitle(f'Эксперимент {experiment_id}, Шаг {step}\nЧастицы: {particles_count}, Запуски: {len(all_try)}', 
                     fontsize=16)
    
    # axes[0].hist(all_x, bins=x_bins_count, alpha=0.7, color='blue', edgecolor='black', density=True)
    for i in x_bins:
        i /= 20.0
    for i in y_bins:
        i /= 20.0
    for i in z_bins:
        i /= 20.0
        
        
    # Создаем границы интервалов для оси X
    x_bin_edges = [mn_x + i * x_step_size for i in range(num_categories + 1)]
    x_bin_centers = [(x_bin_edges[i] + x_bin_edges[i+1]) / 2 for i in range(num_categories)]
    x_bin_labels = [f'{x_bin_centers[i]:.1f}' for i in range(num_categories)]  
    
    # Строим гистограмму для X координат с правильными интервалами
    axes[0].bar(x_bin_centers, x_bins, width=x_step_size * 0.8, color='blue', edgecolor='black', alpha=0.7)
    axes[0].set_xlabel('Диапазон координаты X')
    axes[0].set_ylabel('Количество частиц')
    axes[0].set_title(f'Распределение X ({num_categories} интервалов)')
    axes[0].grid(True, alpha=0.3)
    # Устанавливаем метки интервалов на оси X
    axes[0].set_xticks(x_bin_centers)
    axes[0].set_xticklabels(x_bin_labels, rotation=45, ha='right')
    
    # Создаем границы интервалов для оси Y
    y_bin_edges = [mn_y + i * y_step_size for i in range(num_categories + 1)]
    y_bin_centers = [(y_bin_edges[i] + y_bin_edges[i+1]) / 2 for i in range(num_categories)]
    y_bin_labels = [f'{y_bin_centers[i]:.1f}' for i in range(num_categories)]
    
    # Строим гистограмму для Y координат с правильными интервалами
    axes[1].bar(y_bin_centers, y_bins, width=y_step_size * 0.8, color='green', edgecolor='black', alpha=0.7)
    axes[1].set_xlabel('Диапазон координаты Y')
    axes[1].set_ylabel('Количество частиц')
    axes[1].set_title(f'Распределение Y ({num_categories} интервалов)')
    axes[1].grid(True, alpha=0.3)
    # Устанавливаем метки интервалов на оси X
    axes[1].set_xticks(y_bin_centers)
    axes[1].set_xticklabels(y_bin_labels, rotation=45, ha='right')
    
    # Создаем границы интервалов для оси Z
    z_bin_edges = [mn_z + i * z_step_size for i in range(num_categories + 1)]
    z_bin_centers = [(z_bin_edges[i] + z_bin_edges[i+1]) / 2 for i in range(num_categories)]
    z_bin_labels = [f'{z_bin_centers[i]:.1f}' for i in range(num_categories)]
    
    # Строим гистограмму для Z координат с правильными интервалами
    axes[2].bar(z_bin_centers, z_bins, width=z_step_size * 0.8, color='red', edgecolor='black', alpha=0.7)
    axes[2].set_xlabel('Диапазон координаты Z')
    axes[2].set_ylabel('Количество частиц')
    axes[2].set_title(f'Распределение Z ({num_categories} интервалов)')
    axes[2].grid(True, alpha=0.3)
    # Устанавливаем метки интервалов на оси X
    axes[2].set_xticks(z_bin_centers)
    axes[2].set_xticklabels(z_bin_labels, rotation=45, ha='right')
    
    # Добавляем информацию о количестве частиц и размере интервала
    total_particles = len(all_x)
    expected_particles = particles_count
    
    info_text = (f'Всего: {total_particles}\n'
                 f'Ожидалось: {expected_particles}\n'
                 f'Количество категорий: {num_categories}')
    
    axes[0].text(0.02, 0.98, info_text, 
                 transform=axes[0].transAxes, verticalalignment='top',
                 bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
    
    plt.tight_layout()
    
    # Сохраняем гистограмму
    if step == mx_step:
        save_path = os.path.join(f"../images/Experiment_{experiment_id}/", f'final_dist.png')
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        save_path = os.path.join(save_dir, f'step_{step}.png')
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
    else:
        save_path = os.path.join(save_dir, f'step_{step}.png')
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
    
    plt.close()
    print(f"Гистограмма для шага {step} сохранена: {save_path}")
    print(f"  Интервалы: X={num_categories}, Y={num_categories}, Z={num_categories}")


def create_all_histograms():
    """
    Строит гистограммы для всех шагов эксперимента
    """
    get_parametrs()
    
    print(f"Построение гистограмм для эксперимента {experiment_id}")
    print(f"Максимальное количество шагов: {mx_step}")
    print(f"Количество запусков: {len(all_try)}")
    print(f"Количество категорий (num_categories): {num_categories}")
    print(f"Диапазоны: X=[{mn_x:.2f}, {mx_x:.2f}], Y=[{mn_y:.2f}, {mx_y:.2f}], Z=[{mn_z:.2f}, {mx_z:.2f}]")
    
    # Строим гистограммы для всех шагов
    # Шаги начинаются с 0 и идут до mx_step (включительно)
    for step in range(100000, mx_step + 1, 100000):
        try:
            create_histogram(step)
        except Exception as e:
            print(f"Ошибка при создании гистограммы для: {e}")
    
    print("Построение всех гистограмм завершено!")


if __name__ == "__main__":
    create_all_histograms()