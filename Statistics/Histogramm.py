import json
import matplotlib.pyplot as plt
import numpy as np
import os
import math

mx_step : int # максимальное количество шагов, которое было сделано в эксперименте, для которого строится гистограмма
category_len : int # размер интервала для гистограммы, который задается в input.json и используется для всех шагов эксперимента, для которого строится гистограмма
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
    global category_len, conf_id, experiment_id, particles_count, Bufer, Fractal, all_try, mx_step, mx_x, mx_y, mx_z, mn_x, mn_y, mn_z
    with open("input.json", 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    exp_id = data["exp_id"]
    category_len = data["category_len"]
    
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
    mx_x = float(data[-1].split(' ')[0])
    mx_y = float(data[-1].split(' ')[1]) + sz
    mx_z = float(data[-1].split(' ')[2]) + sz


def calculate_bins_count(min_val, max_val, step_size):
    """
    Рассчитывает количество интервалов для гистограммы
    на основе размера шага category_len
    """
    range_size = max_val - min_val
    if range_size <= 0 or step_size <= 0:
        return 10  # значение по умолчанию, если данные некорректны
    
    # Количество интервалов = диапазон / размер шага (с округлением вверх)
    bins_count = math.ceil(range_size / step_size)
    return max(1, bins_count)  # минимум 1 интервал


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
    
    # Рассчитываем количество интервалов для каждой оси
    x_bins_count = calculate_bins_count(mn_x, mx_x, category_len)
    y_bins_count = calculate_bins_count(mn_y, mx_y, category_len)
    z_bins_count = calculate_bins_count(mn_z, mx_z, category_len)
    
    # Создаем фигуру с тремя подграфиками
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    if step == mx_step:
        fig.suptitle(f'Experiment {experiment_id}, Final Step (step {step})\nParticles: {particles_count}, Runs: {len(all_try)}', 
                     fontsize=16)
    else:
        fig.suptitle(f'Experiment {experiment_id}, Step {step}\nParticles: {particles_count}, Runs: {len(all_try)}', 
                     fontsize=16)
    
    # Строим гистограмму для X координат
    axes[0].hist(all_x, bins=x_bins_count, alpha=0.7, color='blue', edgecolor='black')
    axes[0].set_xlabel('X coordinate')
    axes[0].set_ylabel('Frequency')
    axes[0].set_title(f'X Distribution (bins: {x_bins_count})')
    axes[0].grid(True, alpha=0.3)
    
    # Строим гистограмму для Y координат
    axes[1].hist(all_y, bins=y_bins_count, alpha=0.7, color='green', edgecolor='black')
    axes[1].set_xlabel('Y coordinate')
    axes[1].set_ylabel('Frequency')
    axes[1].set_title(f'Y Distribution (bins: {y_bins_count})')
    axes[1].grid(True, alpha=0.3)
    
    # Строим гистограмму для Z координат
    axes[2].hist(all_z, bins=z_bins_count, alpha=0.7, color='red', edgecolor='black')
    axes[2].set_xlabel('Z coordinate')
    axes[2].set_ylabel('Frequency')
    axes[2].set_title(f'Z Distribution (bins: {z_bins_count})')
    axes[2].grid(True, alpha=0.3)
    
    # Добавляем информацию о количестве частиц и размере интервала
    total_particles = len(all_x)
    expected_particles = particles_count * len(all_try)
    
    info_text = (f'Total: {total_particles}\n'
                 f'Expected: {expected_particles}\n'
                 f'Step size: {category_len}')
    
    axes[0].text(0.02, 0.98, info_text, 
                 transform=axes[0].transAxes, verticalalignment='top',
                 bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
    
    plt.tight_layout()
    
    # Сохраняем гистограмму
    if step == mx_step:
        save_path = os.path.join(f"../images/Experiment_{experiment_id}/", f'final_dist.png')
        save_path = os.path.join(save_dir, f'step_{step}.png')
    else:
        save_path = os.path.join(save_dir, f'step_{step}.png')
    
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"Гистограмма для шага {step} сохранена: {save_path}")
    print(f"  Интервалы: X={x_bins_count}, Y={y_bins_count}, Z={z_bins_count}")


def create_all_histograms():
    """
    Строит гистограммы для всех шагов эксперимента
    """
    get_parametrs()
    
    print(f"Построение гистограмм для эксперимента {experiment_id}")
    print(f"Максимальное количество шагов: {mx_step}")
    print(f"Количество запусков: {len(all_try)}")
    print(f"Размер интервала (category_len): {category_len}")
    print(f"Диапазоны: X=[{mn_x:.2f}, {mx_x:.2f}], Y=[{mn_y:.2f}, {mx_y:.2f}], Z=[{mn_z:.2f}, {mx_z:.2f}]")
    
    # Строим гистограммы для всех шагов
    # Шаги начинаются с 0 и идут до mx_step (включительно)
    for step in range(mx_step + 1):
        try:
            create_histogram(step)
        except Exception as e:
            ''
    
    print("Построение всех гистограмм завершено!")


if __name__ == "__main__":
    create_all_histograms()