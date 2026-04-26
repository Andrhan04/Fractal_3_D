import json
import matplotlib.pyplot as plt
import numpy as np

''' Пример json файла
[
    {
        "exp_id": 1
    },
    {
        "exp_id": 2
    },
    {
        "exp_id": 3
    }
]
'''


class Structure:
    trap_id : int
    trap_cnt : int
    time_live : list[float]
    alive_cnt : list[float]
    def __init__(self, field_id : int, trap_cnt : int, trap_id : int):
        self.trap_cnt = trap_cnt
        self.trap_id = trap_id
        self.field_id = field_id
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
        avg_time_live = (self.get_avg_time_live())
        if avg_time_live != None:
            return f"trap_id: {self.trap_id:03} trap_cnt: {self.trap_cnt:04}, time_live({len(self.time_live):02}): {avg_time_live:8.3f}, alive_cnt: {self.get_avg_alive_cnt():.2f}"
        else:
            return f"trap_id: {self.trap_id:03} trap_cnt: {self.trap_cnt:04}, time_live({len(self.time_live):02}): {"None":11}, alive_cnt: {self.get_avg_alive_cnt():.2f}"
    
    def __gt__(self, other):
        return self.trap_cnt > other.trap_cnt

class Experiment:
    experimet_id : int = None
    all_particles_cnt : int = None
    steps : int = None
    degit_time_live : int = None
    configuration_field : int = None
    results : list[Structure]
    
    def __get_results__(self):
        self.results = []
    
    def __init__(self, experimet_id : int) -> None:
        self.experimet_id = experimet_id
        
        with open(f"../Results_traps/experiment_{experimet_id}.json", 'r', encoding='utf-8') as file:
            data = json.load(file)
    
        self.all_particles_cnt = data[0]['particles_count']
        self.steps = data[0]['steps']
        self.degit_time_live = data[0]["N/e"]
        self.configuration_field = data[0]['conf_id']
        self.results = []
        for exp in data:
            was_added = False
            for structure in self.results:
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
                structure = Structure(self.experimet_id, trap_cnt, exp['Trap_id'])
                if(exp['time_live'] >= 0):
                    structure.add_time_live(time_live)
                structure.add_alive_cnt(alive_cnt)
                self.results.append(structure)
    
    def __str__(self) -> str:
        if(self.experimet_id != None):
            return f"""
        Experiment id {self.experimet_id}, 
        particles {self.all_particles_cnt}, 
        steps {self.steps}, 
        degit_time_live {self.degit_time_live},
        configuration field {self.configuration_field}, 
        results  {len(self.results)}
        """
        else:
            return "Not create"

    def get_name_label(self) -> str:
        return f"Эксперимент {self.experimet_id}"
    
    def get_avg_time_live(self):
        x = []
        y = []
        for structure in self.results:
            avg = structure.get_avg_time_live()
            if avg != None:
                y.append(avg)
                x.append(structure.trap_cnt)
        return x,y
    
    def get_avg_alive(self):
        x = []
        y = []
        for structure in self.results:
            avg = structure.get_avg_alive_cnt()
            if avg != None:
                y.append(avg)
                x.append(structure.trap_cnt)
        return x,y
    
    def get_x_ticks(self):
        x_ticks = [self.results[i].trap_cnt for i in range(0, len(self.results), 5)]
        x_ticks_labels = [str(trap_cnt) if trap_cnt % 100 == 0 else "" for trap_cnt in x_ticks]
        
        return x_ticks, x_ticks_labels

def get_parametrs() -> list[Experiment]:
    with open('input.json', 'r') as f:
        data = json.load(f)
    result : list[Experiment] = []
    for exp in data:
        new_experiment = Experiment(exp["exp_id"])
        result.append(new_experiment)
    return result
        
def plot_alive_cnt(data : list[Experiment]):
    all_particles_cnt : int = 0
    steps : int = 0
    save_name = "../image_depends/imperial/Comparison_alive_"
    for exp_id in data:
        save_name += f"_{exp_id.experimet_id}"
        all_particles_cnt = max(all_particles_cnt, exp_id.all_particles_cnt)
        steps = max(steps, exp_id.steps)
    save_name += ".png"
    
    plt.figure(figsize=(12, 8))
    plt.title(f'Количество живых частиц из {all_particles_cnt}, в разных фракталах на {steps} шагов', fontsize=14)
    plt.grid(True, which="both", ls="--", linewidth=0.5)
    
    plt.xlabel('Количество ловушек', fontsize=12)
    x_ticks, x_ticks_labels = data[0].get_x_ticks()
    plt.xticks(x_ticks, x_ticks_labels, fontsize=10)
    plt.xlim(data[0].results[0].trap_cnt, data[0].results[-1].trap_cnt)
    
    plt.ylabel('Среднее количество живых частиц', fontsize=12)
    plt.ylim(0, all_particles_cnt)  # Устанавливаем пределы от 0 до общего количества частиц
    y_ticks = [i * 50 for i in np.arange(0, all_particles_cnt//50)]
    y_ticks_labels = [str(cnt_alive) for cnt_alive in y_ticks]
    plt.yticks(y_ticks, y_ticks_labels, fontsize=10)
    
    for exp in data:
        x, y = exp.get_avg_alive()
        plt.plot(x, y, ls ='-', label = exp.get_name_label())
    
    plt.tight_layout()
    plt.legend()
    plt.savefig(save_name, dpi=300)
    # plt.show()
    plt.close()
    
    
    
def plot_time_live(data : list[Experiment]):
    all_particles_cnt : int = 0
    steps : int = 0
    save_name = "../image_depends/imperial/Comparison_time_live_"
    for exp_id in data:
        save_name += f"_{exp_id.experimet_id}"
        all_particles_cnt = max(all_particles_cnt, exp_id.all_particles_cnt)
        steps = max(steps, exp_id.steps)
        
    save_name += ".png"
    
    plt.figure(figsize=(12, 8))
    plt.title(f'Количество живых частиц из {all_particles_cnt}, в разных фракталах на {steps} шагов', fontsize=14)
    plt.grid(True, which="both", ls="--", linewidth=0.5)
    
    plt.xlabel('Количество ловушек', fontsize=12)
    x_ticks, x_ticks_labels = data[0].get_x_ticks()
    plt.xticks(x_ticks, x_ticks_labels, fontsize=10)
    plt.xlim(data[0].results[0].trap_cnt, data[0].results[-1].trap_cnt)
    
    plt.ylabel('Среднее время жизни', fontsize=12)
    plt.ylim(0, all_particles_cnt)
    plt.ylim(0, steps)    
    y_ticks = [i * 5 * (10**5) for i in np.arange(0, steps//(5 * (10**5)) + 1)]
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
    
    for exp in data:
        x, y = exp.get_avg_time_live()
        plt.plot(x, y, ls ='-', label = exp.get_name_label())
    
    plt.tight_layout()
    plt.legend()
    plt.savefig(save_name, dpi=300)
    plt.close()
    


def main():
    data = get_parametrs()
    plot_alive_cnt(data)
    plot_time_live(data)
    
    

if __name__ == "__main__":
    main()