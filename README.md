# «Живой океан» (OceanSimulation)

## Фичи
* **ООП**: иерархия `Entity → Algae / HerbivoreFish / PredatorFish`.
* **Шаблоны**: мини‑класс‑синглтон `Logger<T>` + обёртка над `std::mt19937`.
* **RAII + smart‑ptr**: вся динамика хранится в `std::unique_ptr`.
* **Наследование / virtual**: поведение сущностей переопределяется.
* **Исключения**: защитные проверки выхода за границы.
* **Rule of Five** + **Pimpl**: класс `Ocean` демонстрирует обе идиомы.
* **CMake**: сборка одной командой.
* **Bonus**: визуализация на **SFML** с раскраской объектов.

## Сборка
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./ocean   # или ocean.exe под Windows
```