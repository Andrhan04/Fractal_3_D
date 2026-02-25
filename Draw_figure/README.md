## Draw_figure

Скрипты визуализации буфера, фрактала и позиций частиц.

### Зависимости
- Python 3.10+
- `pip install numpy matplotlib pillow`

### Скрипты
- `paint_fractal.py` — визуализация буфера и фрактала с заданным радиусом.
- `Draw_point_positions.py` — визуализация результатов эксперимента вместе с геометрией и стартовыми точками.

### Настройки
- `draw_point.json`:
  - для `Draw_point_positions.py` содержит:
    - `exp_id` — id результата в каталоге `Results/experiment_{exp_id}.json`;
    - `try_id` — индекс запуска внутри файла результатов.

### Запуск
1. Убедитесь, что каталоги `Sowing` и `Results/Experiments` заполнены результатами симуляций.
2. Запустите скрипт:
   
   `python paint_fractal.py`
   
   или
   
   `python Draw_point_positions.py`

### Что делает программа
- Читает геометрию:
  - `Sowing/Bufer/Bufer_{id}.txt` — один наклонный цилиндр;
  - `Sowing/Fractal/Pole_{id}.txt` — последовательность центров наклонных цилиндров.
- Рисует цилиндрические сегменты (основания перпендикулярны оси OX).
- Для `Draw_point_positions.py` дополнительно читает точки попытки из `Experiments/exp_{id}.txt` и подсказки из `Results/experiment_{exp_id}.json`.

