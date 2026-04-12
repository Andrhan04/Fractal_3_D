import json
import numpy as np
from scipy.optimize import curve_fit

func = "a/(values_x**(b))"
def function(values_x, a, b):
    #return (2.71**values_x)*a + b                  # не работает на времени жизни
    #return a/values_x + b                          # 7.484% на времени жизни
    #return a/(values_x**2) + b                      # не работает на времени жизни
    #return (np.log(values_x)*a + b)                # 11.134% на времени жизни
    #return ((2.71**values_x)*b + a*(values_x**2))  # не работает на времени жизни
    #return ((values_x)*b + a*(values_x**2))        # 24.737% на времени жизни
    #return ((values_x)*a + b)                      # 14.649% на времени жизни
    #return (a/(values_x**(1/2)) + b)               # 9.327% на времени жизни
    return (a/(values_x**(b)))                     # не работает на времени жизни

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
                structure.add_time_live(exp['Trap_id'])
                structure.add_alive_cnt(exp['Alive'])
                was_added = True
                break
        if not was_added:
            trap_cnt = exp['Traps_count']
            time_live = exp['time_live']
            alive_cnt = exp['Alive']
            structure = Structure(trap_cnt, exp['Trap_id'])
            structure.add_time_live(time_live)
            structure.add_alive_cnt(alive_cnt)
            result.append(structure)

    return result

def cnt_good(x_data : list[float], y_data : list[float], a : float, b : float, func) -> float:
    yint=[]
    for x in x_data:
        yint.append(func(x, a, b))
    so = 0
    for i in range(len(y_data)):
        if(y_data[i] !=0):
            so += abs(yint[i]-y_data[i])/y_data[i]
    so = so / (len(y_data)) * 100
    so = round(so,3)
    return so

def approximation_time_live(data : list[Structure]):
    x = [structure.trap_cnt for structure in data]
    y = [structure.get_avg_time_live() for structure in data]
    args, covar = curve_fit(function, x, y)
    a =  args[0]
    b =  args[1]
    print(f"Параметры аппроксимации: a = {a}, b = {b}")
    print(f"Погрешность аппроксимации: {cnt_good(x, y, a, b, function)}%")
    
def approximation_alive_cnt(data : list[Structure]):
    x = [structure.trap_cnt for structure in data]
    y = [structure.get_avg_alive_cnt() for structure in data]
    
    try:
        args, covar = curve_fit(function, x, y)
        a =  args[0]
        b =  args[1]
        print(f"Параметры аппроксимации: a = {a}, b = {b}")
        print(f"Погрешность аппроксимации: {cnt_good(x, y, a, b, function)}%")
        j : json = {
            "Function" : func, 
            "args" : [a,b], # [a,b]
            "Loss" : cnt_good(x, y, a, b, function) # "-"
        }
    except Exception as e:
        print(e)
        j : json = {
            "Function" : func, 
            "args" : [],
            "Loss" : None
        }
    file_name = f"../Depends/exp_{experimet_id}_alive.json"
    try:
        with open(file_name, 'r', encoding="utf-8") as file_save:
            all_data : list[json] = json.load(file_save)
            all_data.append(j)
        with open(file_name, 'w', encoding="utf-8") as file_save:
            json.dump(all_data, file_save, indent=4)
    except Exception as e:
        print(e)
        with open(file_name, 'w', encoding="utf-8") as file_save:
            json.dump([j], file_save)
        
        
        

def main():
    data = get_data()
    for exp in data:
        print(exp)
    
    #approximation_time_live(data)
    approximation_alive_cnt(data)
    
if __name__ == "__main__":
    main()