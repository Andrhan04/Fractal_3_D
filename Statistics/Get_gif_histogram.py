import json
import os
import imageio.v2 as imageio
from PIL import Image

exp_id : int

def get_parametrs():
    global exp_id
    with open(f'input.json', 'r') as f:
        data = json.load(f)
        exp_id = data['exp_id']


def get_all_files():
    path = f"../images/Experiment_{exp_id}/Hist"
    files = os.listdir(path)
    files = sorted(files, key=lambda x: int((x.split('.')[0]).split('_')[-1]))
    files = [f"{path}/{file}" for file in files]
    return files

def create_gif(files : list):
    images = []
    for file in files:
        img = Image.open(file)
        img = img.resize((1200, 400), Image.Resampling.LANCZOS)
        images.append(img)
    
    gif_path = f"../images/Experiment_{exp_id}/histogram.gif"
    imageio.mimsave(gif_path, images, duration=0.5, format='GIF')
    print(f"GIF анимация создана: {gif_path}")
    print(f"Количество кадров: {len(images)}")
    print(f"Длительность кадра: 0.5 сек")
    
    
if __name__ == '__main__':
    get_parametrs()
    files = get_all_files()
    create_gif(files)