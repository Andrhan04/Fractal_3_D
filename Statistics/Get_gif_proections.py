import json
import os
import glob
from PIL import Image

exp_id : int # id эксперимента, для которого строится гистограмма

def get_parametrs():
    global exp_id
    with open(f'input.json', 'r') as f:
        data = json.load(f)
        exp_id = data['exp_id']
        
def create_gif_from_projections():
    """
    Создает GIF анимацию из всех файлов проекций
    """
    get_parametrs()
    
    # Путь к папке с проекциями
    path_to_files = f"../images/Experiment_{exp_id}/Projections"
    
    # Проверяем существование папки
    if not os.path.exists(path_to_files):
        print(f"Папка с проекциями не найдена: {path_to_files}")
        return
    
    # Получаем все PNG файлы проекций и сортируем их по номеру шага
    image_files = glob.glob(os.path.join(path_to_files, "step_*.png"))
    
    # Сортируем файлы по номеру шага
    def get_step_number(filename):
        # Извлекаем число из имени файла step_XXXXXXX.png
        base_name = os.path.basename(filename)
        step_str = base_name.replace("step_", "").replace(".png", "")
        try:
            return int(step_str)
        except ValueError:
            return 0
    
    image_files.sort(key=get_step_number)
    
    if not image_files:
        print("Не найдено файлов проекций для создания GIF")
        return
    
    print(f"Найдено {len(image_files)} файлов проекций")
    
    # Создаем список для хображений
    images = []
    
    # Загружаем все изображения
    for image_file in image_files:
        try:
            img = Image.open(image_file)
            images.append(img)
            print(f"Загружено: {os.path.basename(image_file)}")
        except Exception as e:
            print(f"Ошибка загрузки {image_file}: {e}")
    
    if not images:
        print("Не удалось загрузить ни одного изображения")
        return
    
    # Создаем GIF
    gif_path = f"../images/Experiment_{exp_id}/projections_animation.gif"
    
    # Создаем папку если её нет
    os.makedirs(os.path.dirname(gif_path), exist_ok=True)
    
    # Сохраняем как GIF с оптимизацией
    images[0].save(
        gif_path,
        format='GIF',
        append_images=images[1:],
        save_all=True,
        duration=200,  # 200ms между кадрами
        loop=0,       # бесконечный цикл
        optimize=True
    )
    
    print(f"GIF анимация создана: {gif_path}")
    print(f"Количество кадров: {len(images)}")
    print(f"Размер GIF: {os.path.getsize(gif_path) / 1024 / 1024:.2f} MB")

if __name__ == "__main__":
    create_gif_from_projections()