import json
import matplotlib.pyplot as plt
import numpy as np
class Structure:
    trap_id : int
    trap_cnt : int
    time_live : list[float]
    alive_cnt : list[float]
    def __init__(self, trap_cnt : int, trap_id : int):
        self.trap_cnt = trap_cnt
        self.trap_id = trap_id
        self.time_live = []
        self.alive_cnt = []
    
    def add_time_live(self, time_live : float):
        self.time_live.append(time_live)
    
    def add_alive_cnt(self, alive_cnt : float):
        self.alive_cnt.append(alive_cnt)
    
    def get_avg_time_live(self):
        if(len(self.time_live) == 0):
            return None
        else:
            return float(sum(self.time_live) / len(self.time_live))
    
    def get_avg_alive_cnt(self):
        return float(sum(self.alive_cnt) / len(self.alive_cnt))
    
    def __eq__(self, value):
        return self.trap_cnt == value.trap_cnt
    
    def __lt__(self, value):
        return self.trap_cnt < value.trap_cnt
    
    def __str__(self):
        return f"trap_cnt: {self.trap_cnt}, time_live: {self.get_avg_time_live()}, alive_cnt: {self.get_avg_alive_cnt()}"
    
    def __gt__(self, other):
        return self.trap_cnt > other.trap_cnt

experimet_id : int = -1
all_particles_cnt : int = -1
steps : int = -1
degit_time_live : int = -1
configuration_field : int = -1

def get_parametrs() -> list[Structure]:
    global experimet_id, all_particles_cnt, steps, degit_time_live, configuration_field
    with open('input.json', 'r') as f:
        data = json.load(f)
    experimet_id = data['exp_id']
    
    with open(f"../Results_traps/experiment_{experimet_id}.json", 'r', encoding='utf-8') as file:
        data = json.load(file)
        
    all_particles_cnt = data[0]['particles_count']
    steps = data[0]['steps']
    degit_time_live = data[0]["N/e"]
    configuration_field = data[0]['conf_id']
    
    result : list[Structure] = []
    for exp in data:
        was_added = False
        for structure in result:
            if structure.trap_cnt == exp['Traps_count']:
                if(exp['time_live'] >= 0):
                    structure.add_time_live(exp['time_live'])
                structure.add_alive_cnt(exp['Alive'])
                was_added = True
                break
        if not was_added:
            trap_cnt = exp['Traps_count']
            time_live = exp['time_live']
            alive_cnt = exp['Alive']
            structure = Structure(trap_cnt, exp['Trap_id'])
            if(exp['time_live'] >= 0):
                structure.add_time_live(time_live)
            structure.add_alive_cnt(alive_cnt)
            result.append(structure)

    return result

def plot_time_live(data : list[Structure]):
    plt.figure(figsize=(12, 8))
    x = [structure.trap_cnt for structure in data]
    y = [structure.get_avg_time_live() for structure in data]
    plt.plot(x, y)
    plt.xlabel('Количество ловушек', fontsize=12)
    plt.ylabel('Среднее время жизни', fontsize=12)
    
    
    plt.ylim(0, 10**7)    
    # Создаем деления в линаричном масштабе
    y_ticks = [i * 5 * (10**5) for i in np.arange(0, 21)]
    # Форматируем метки для отображения
    y_tick_labels = []
    for tick in y_ticks:
        if tick % 10**6 == 0 and tick != 0:
            if (tick != 10**7):
                y_tick_labels.append(f'${tick // (10**6)} \\times 10^6$')
            else:
                y_tick_labels.append(f'$10^7$')
        else:
            y_tick_labels.append("")
    plt.yticks(y_ticks, y_tick_labels, fontsize=10)
        
    x_ticks = [structure.trap_cnt for structure in data]
    x_ticks_labels = [str(trap_cnt) if trap_cnt % 50 == 0 else "" for trap_cnt in x_ticks]
    plt.xticks(x_ticks, x_ticks_labels, fontsize=10)
    plt.xlim(min(x), max(x))
    
    # Добавляем дополнительное пространство для меток
    plt.title(f'Время жизни у частиц (с колличеством {all_particles_cnt}), \nв конфигузации {configuration_field}, эксперимент {experimet_id} на {steps} шагов', fontsize=14)
    plt.tight_layout()
    plt.grid(True, which="both", ls="--", linewidth=0.5)  # Добавляем сетку для лучшей читаемости
    plt.savefig(f"../image_depends/imperial/experiment_{experimet_id}_time_live.png", dpi=300)
    plt.close()
    #plt.show()

def plot_alive_cnt(data : list[Structure]):
    plt.figure(figsize=(12, 8))
    x = [structure.trap_cnt for structure in data]
    y = [structure.get_avg_alive_cnt() for structure in data]
    plt.plot(x, y)
    
    plt.ylabel('Среднее количество живых частиц', fontsize=12)
    plt.ylim(0, all_particles_cnt)  # Устанавливаем пределы от 0 до общего количества частиц
    y_ticks = [i * 100 for i in np.arange(0, all_particles_cnt//100)]
    y_ticks_labels = [str(cnt_alive) for cnt_alive in y_ticks]
    plt.yticks(y_ticks, y_ticks_labels, fontsize=10)
    
    plt.xlim(min(x), max(x))
    plt.xlabel('Количество ловушек', fontsize=12)
    x_ticks = [structure.trap_cnt for structure in data]
    x_ticks_labels = [str(trap_cnt) if trap_cnt % 50 == 0 else "" for trap_cnt in x_ticks]
    plt.xticks(x_ticks, x_ticks_labels, fontsize=10)
    
    plt.title(f'Количество живых частиц из {all_particles_cnt}, \nв конфигузации {configuration_field}, эксперимент {experimet_id} на {steps} шагов', fontsize=14)
    plt.grid(True, which="both", ls="--", linewidth=0.5)  # Добавляем сетку для лучшей читаемости
    plt.savefig(f"../image_depends/imperial/experiment_{experimet_id}_alive_cnt.png", dpi=300)
    plt.close()
    # plt.show()

def main():
    data = get_parametrs()
    for exp in data:
        print(exp)
    plot_time_live(data)    
    plot_alive_cnt(data)
    
    
if __name__ == "__main__":
    main()