# GLabs - Лабы по OpenGL

## Сборка

### Необходимые программы и компоненты
- CMake версии >=3.19
- Visual Studio 2022 или MSYS2
- Поддержка драйверами OpenGL версии >=4.5 или `EXT_direct_state_access`

### Создание билда
1. __Склонируйте репозиторий:__

Выполните команды в консоли:

`git clone https://github.com/leoovs/glabs glabs`

`cd glabs`

Выберите нужную ветку, например:
`git checkout dev`

2. __Сгенерируйте конфигурацию CMake:__

Если у вас Visual Studio 2022: `cmake -B build -G "Visual Studio 17 2022"`

Если у вас MSYS2: `cmake -B build -G "MinGW Makefiles"`

3. __Соберите CMake-проект:__

Выполните команду в консоли: `cmake --build build --parallel`

