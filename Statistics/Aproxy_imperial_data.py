import json
import os
import random
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# Глобальные параметры эксперимента
experimet_id : int = -1
all_particles_cnt : int = -1
steps : int = -1
degit_time_live : int = -1
configuration_field : int = -1

class Function:
    s : str
    __sample_function : list[str]
    a : float
    b : float
    c : float
    param_best : float
    path_to_save : str
    
    def __init__(self, function_str : str):
        self.__sample_function = [
            'a * x + b',
            'a * e^x + b',
            'a / x + b',
            'a / (x^2) + b',
            'a * log(x) + b',
            'a * (x^2) + b * (e^x)',
            'a*(x**2) + x*b + c',
            'a / (sqrt(x)) + b',
            'a / (x ^ b)']
        if function_str in self.__sample_function:
            self.s = function_str
        else:
            self.s = random.choice(self.__sample_function)
        print("-"*50 + "\nfunc: " + function_str)
        self.a = None
        self.b = None
        self.c = None
        self.param_best = None
        path_to_save = f'../image_depends/aproxy/experiment_{experimet_id}/'
    
    def get_LaTex(self) -> str:
        if (self.s == "a * x + b"):             return "$ax + b$"
        if (self.s == "a * e^x + b"):           return "$ae^x + b$"
        if (self.s == "a / x + b"):           return "$\\frac{a}{x} + b$"
        if (self.s == "a / (x^2) + b"):         return "$\\frac{a}{x^2} + b$"
        if (self.s == 'a * log(x) + b'):        return "$a \\times log(x) + b$"
        if (self.s == "a * (x^2) + b * (e^x)"): return "$ax^2 + be^x$"
        if (self.s == "a*(x**2) + x*b + c"):    return "$ax^2 + bx + c$"
        if (self.s == "a / (sqrt(x)) + b"):     return "$\\frac{a}{\\sqrt{x}} + b$"
        if (self.s == "a / (x ^ b)"):           return "$\\frac{a}{x^b}$"
    
    def set_params(self, x : list[float], y : list[float]):
        try:
            args, covar = curve_fit(self.count, x, y)
            self.a =  args[0]
            self.b =  args[1]
            if(self.s == "a*(x**2) + x*b + c"):
                self.c = args[2]
            self.param_best = self.__cnt_good__(x, y)
        except Exception as e:
            print(f"Can`t count params {e}")
            self.a = None
            self.b = None
            self.c = None
            self.param_best = None
    
    def count(self, x, a : float, b : float, c : float = 0):
        if (self.s == "a * x + b"): return (x*a + b)                                # 14.649% на времени жизни
        if (self.s == "a * e^x + b"): return (2.71**x)*a + b                        # не работает на времени жизни
        if (self.s == "'a / x + b'"): return a/x + b                                # 7.484% на времени жизни
        if (self.s == "a / (x^2) + b"): return a/(x**2) + b                         # не работает на времени жизни
        if (self.s == 'a * log(x) + b'): return (np.log(x)*a + b)                   # 11.134% на времени жизни
        if (self.s == "a * (x^2) + b * (e^x)"): return ((2.71**x)*b + a*(x**2))     # не работает на времени жизни
        if (self.s == "a*(x**2) + x*b + c"): return ((x)*b + a*(x**2) + c)          # 24.737% на времени жизни
        if (self.s == "a / (sqrt(x)) + b"): return (a/(x**(1/2)) + b)               # 9.327% на времени жизни
        if (self.s == "a / (x ^ b)"): return (a/(x**(b)))                           # не работает на времени жизни
    
    def __get_predictional__(self, x_data : list[float]) -> list[float]:
        yint=[]
        for x in x_data:
            if(self.c == None):
                yint.append(self.count(x, self.a, self.b))
            else:
                yint.append(self.count(x, self.a, self.b, self.c))
        return yint
      
    def __cnt_good__(self, x_data : list[float], y_data : list[float]) -> float:
        yint = self.__get_predictional__(x_data)
        so = 0
        for i in range(len(y_data)):
            if(y_data[i] != 0):
                so += abs(yint[i]-y_data[i])/y_data[i]
        so = so / (len(y_data)) * 100
        so = round(so,3)
        return so
    
    def __str__(self):
        if(self.a == None):
            return self.s
        else:
            if(self.c != None):
                return self.s + f" params: a = {self.a}, b = {self.b}, c = {self.c}, good = {self.param_best}"
            else:
                return self.s + f" params: a = {self.a}, b = {self.b}, good = {self.param_best}"
    
    def get_info(self) -> json:
        j : json
        function_id : int = self.__sample_function.index(self.s)
        if(self.a == None):
            j = {
                "Function_id" : function_id,
                "Function" : self.s, 
                "LaTex" : self.get_LaTex(),
                "args" : [],
                "Loss" : None,
                "path_to_paint" : self.path_to_save
            }
        elif (self.c == None):
            j = {
                "Function_id" : function_id,
                "Function" : self.s, 
                "LaTex" : self.get_LaTex(),
                "args" : [self.a, self.b],
                "Loss" : self.param_best,
                "path_to_paint" : self.path_to_save
            }
        else:
            j = {
                "Function_id" : function_id,
                "Function" : self.s, 
                "LaTex" : self.get_LaTex(),
                "args" : [self.a, self.b, self.c],
                "Loss" : self.param_best,
                "path_to_paint" : self.path_to_save
            }
        return j
    
    def paint_aproxy(self, path : str, data : list[Structure]):
        """
        path = f'../image_depends/aproxy/experiment_{experimet_id}/time_live'
        или
        path = f'../image_depends/aproxy/experiment_{experimet_id}/alive'
        """
        plt.figure(figsize=(12, 8))
        function_id : int = self.__sample_function.index(self.s)
        self.path_to_save = path + f"_{function_id}.png"
        
        x :list[float] = []
        y :list[float] = []
        if path.split('/')[-1] == "time_live":
            for structure in data:
                avg = structure.get_avg_time_live()
                if avg != None:
                    y.append(avg)
                    x.append(structure.trap_cnt)
        else:
            x = [structure.trap_cnt for structure in data]
            y = [structure.get_avg_alive_cnt() for structure in data]
            
        
        # Настройка отрисовки по оси x
        plt.xlabel('Количество ловушек', fontsize=12)
        plt.xlim(min(x), max(x))
        x_ticks = [data[i].trap_cnt for i in range(0, len(data), 5)]
        x_ticks_labels = [str(trap_cnt) if trap_cnt % 100 == 0 else "" for trap_cnt in x_ticks]
        plt.xticks(x_ticks, x_ticks_labels, fontsize=10)
        
        # Настройка отрисовки по оси у
        if path.split('/')[-1] == "time_live":
            plt.ylabel('Среднее время жизни', fontsize=12)
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
        else:
            plt.ylabel('Среднее количество живых частиц', fontsize=12)
            plt.ylim(0, all_particles_cnt)  # Устанавливаем пределы от 0 до общего количества частиц
            y_ticks = [i * 50 for i in np.arange(0, all_particles_cnt//50)]
            y_ticks_labels = [str(cnt_alive) for cnt_alive in y_ticks]
            plt.yticks(y_ticks, y_ticks_labels, fontsize=10)
        
        # Настройка отрисовки истиных занчений и функций
        plt.plot(x, y, ls = "--", color = 'b', label = "Истинные значения")
        
        
        label_func : str = self.get_LaTex()
        if(self.a == None):
            print(label_func)
            plt.plot([None], [None], color = 'r', label = label_func)
        else:
            y_init = self.__get_predictional__(x)
            plt.plot(x, y_init, color = 'r', label = label_func)
        
        # Настройка самого графика
        if path.split('/')[-1] == "time_live":
            plt.title(f'Аппроксимация времени жизни у частиц (с колличеством {all_particles_cnt}), \nв конфигузации {configuration_field}, эксперимент {experimet_id} на {steps} шагов\nФункция {self.s}, качество {self.param_best}', fontsize=14)
        else:
            plt.title(f'Аппроксимация количества живых частиц из {all_particles_cnt}, \nв конфигузации {configuration_field}, эксперимент {experimet_id} на {steps} шагов\nФункция {self.s}, качество {self.param_best}', fontsize=14) 
        plt.tight_layout()
        plt.grid(True, which="both", ls="--", linewidth=0.5)  # Добавляем сетку для лучшей читаемости
        plt.legend()
        plt.savefig(self.path_to_save)
        # plt.show()
        plt.close()

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
        avg_time_live = (self.get_avg_time_live())
        if avg_time_live != None:
            return f"trap_id: {self.trap_id:03} trap_cnt: {self.trap_cnt:04}, time_live({len(self.time_live):02}): {avg_time_live:8.3f}, alive_cnt: {self.get_avg_alive_cnt():.2f}"
        else:
            return f"trap_id: {self.trap_id:03} trap_cnt: {self.trap_cnt:04}, time_live({len(self.time_live):02}): {"None":11}, alive_cnt: {self.get_avg_alive_cnt():.2f}"
    
    def __gt__(self, other):
        return self.trap_cnt > other.trap_cnt



def get_data() -> list[Structure]:
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

def approximation_time_live(data : list[Structure]):
    x = []
    y = []
    for structure in data:
        avg = structure.get_avg_time_live()
        if avg != None:
            y.append(avg)
            x.append(structure.trap_cnt)
    all_functions = ['a * x + b','a * e^x + b','a / x + b','a / (x^2) + b','a * log(x) + b','a * (x^2) + b * (e^x)','a*(x**2) + x*b + c','a / (sqrt(x)) + b','a / (x ^ b)']
    all_json : list[json] = []
    for fun in all_functions:
        function : Function = Function(fun)
        function.set_params(x,y)
        function.paint_aproxy(path = f'../image_depends/aproxy/experiment_{experimet_id}/time_live', data = data)
        j : json = function.get_info()
        all_json.append(j)
    file_name = f"../Depends/exp_{experimet_id}_time_death.json"
    with open(file_name, 'w', encoding="utf-8") as file_save:
        json.dump(all_json, file_save, indent=4)
    
    
def approximation_alive_cnt(data : list[Structure]):
    global experimet_id
    x = []
    y = []
    for structure in data:
        avg = structure.get_avg_alive_cnt()
        if avg != None:
            y.append(avg)
            x.append(structure.trap_cnt)
    all_functions = ['a * x + b','a * e^x + b','a / x + b','a / (x^2) + b','a * log(x) + b','a * (x^2) + b * (e^x)','a*(x**2) + x*b + c','a / (sqrt(x)) + b','a / (x ^ b)']
    all_json : list[json] = []
    for fun in all_functions:
        function : Function = Function(fun)
        function.set_params(x,y)
        function.paint_aproxy(path = f'../image_depends/aproxy/experiment_{experimet_id}/alive', data = data)
        j : json = function.get_info()
        all_json.append(j)
    file_name = f"../Depends/exp_{experimet_id}_alive.json"
    with open(file_name, 'w', encoding="utf-8") as file_save:
        json.dump(all_json, file_save, indent=4)
        
        
        

def main():
    data = get_data()
    os.makedirs(f'../image_depends/aproxy/experiment_{experimet_id}', exist_ok=True)
    approximation_time_live(data)
    approximation_alive_cnt(data)
    
if __name__ == "__main__":
    main()