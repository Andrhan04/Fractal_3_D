# Fractal_3_D

Симуляция движения частиц в 3D-объёме, заданном последовательностью цилиндров (возможны наклонные сегменты). Проект включает:
- генерацию стартовых частиц в буферной зоне;
- моделирование движения частиц в рабочей зоне (фрактал);
- сохранение результатов и визуализацию.

## Структура каталогов
- Templates — базовые классы: Point, Plane, Figure (сечения x=const), Fractal (набор наклонных цилиндров), BuferZone, Particle, Generator, MyReader/MyWriter.
- Sowing — входные данные:
  - Fractal/Pole_{id}.txt — последовательность центров цилиндров (x y z на строку);
  - Bufer/Bufer_{id}.txt — два центра буферного цилиндра (по строке);
  - Config/Conf_{id}.json — связка Bufer/Fractal/Size/Point;
  - Particle/points_{id}.txt — стартовые позиции частиц.
- Start_particle — генерация стартовых частиц и конфигурации.
- Move — симуляция экспериментов (многократные запуски, большое число шагов).
- Draw_figure — визуализация буфера/фрактала и позиций частиц.
- Experiments, Results — выходные файлы экспериментов и метаданные.

## Требования
- Компилятор C++17 (MSVC/Visual Studio).
- Python 3.10+ с пакетами: numpy, matplotlib.

## Базовый сценарий
1. Подготовьте входные данные в Sowing (Bufer_{id}.txt, Pole_{id}.txt, Size).
2. Сгенерируйте частицы:
   - в Start_particle создайте файл config.txt со строкой: `BUFER_ID FRACTAL_ID N SIZE`;
   - соберите и запустите Start_particle — появятся Sowing/Particle/points_{id}.txt и Config/Conf_{id}.json.
3. Настройте вход Move:
   - используйте Move/input.json (структура как в Move/input.exmple.json) и задайте поля conf_id, exp_id, repeat, steps.
4. Соберите и запустите Move — будут созданы Experiments/exp_{id}.txt и Results/experiment_{exp_id}.json.
5. Для визуализации запустите Draw_figure/paint_fractal.py или Draw_figure/Draw_point_positions.py.

## Форматы
- Move/input.json — массив объектов: { conf_id, exp_id, repeat, steps }.
- Sowing/Config/Conf_{id}.json — { "Bufer": <id>, "Fractal": <id>, "Point": <id>, "Size": <int> }.

