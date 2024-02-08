# CSB framework

## Installation
in source directory:
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --install .
```
You can specify CMAKE_INSTALL_PREFIX to change the installation location

In the installation directory there is config.sh file, which should bee sourced to add CMake package location to indexed directories list. Consider adding it to .profile.

## Общее описание

Работа фреймворка хорошо показана на данной схеме.

Сначала вызывается функция simulate класса Dynamic, которая последовательно считывает, моделирует и выводит, используя наследников классов VRead, VUpdate и VWrite соответственно. 
  
Фреймворк состоит из 3 классов-фабрик: VRead, VUpdate, VWrite, 4 класса контейнера: Dynamic,  SystemState, Particle, Vector3, а так же 2 заголовочных файлов с описание функции и определениями нужных констант.
## Интерфейсные классы

Всего в фреймворке описано 3 интерфейсных класса.
#### VRead - модуль для чтения

В классе VRead определены 3 функции: 
+ ввод вектора - без параметров, возвращает Vector3
+ ввод частицы - без параметров, возвращает Particle
+ ввод состояния системы - принимает кол-во ввозимых частиц, возвращает SystemState
#### VUpdate - модуль для обновления

В классе VUpdate определена 1 функция для обновления частиц - updateSystemState, принимает на вход состояние системы, внешнюю силу, силу взаимодействия 2 частиц и промежуток времени. Очень важно, чтобы промежуток времени был достаточно мал. Функция возвращает новое состояние системы.
#### VWrite - модуль для вывода

В классе VWrite определены 3 функции: 
+ вывод вектора - принимает Vector3, ничего не возвращает
+ вывод частицы - принимает Particle, ничего не возвращает
+ вывод состояния системы - принимает SystemState, ничего не возвращает
### 1. Стандартные реализации

Все реализации лежат в директории implem
В директории они лежат в 3 папках

#### Read

Для Vread есть 3 стандартные реализации
+ StdCinRead - стандартный поток ввода stdin. Нет конструктора
+ StreamRead - поток ввода std::istream. Конструктор принимает константную ссылку на поток std::istream
+ FileRead -поток ввода std::ifstream. Два конструктора: от пути до файла и от потока std::ifstream

#### Update

Для класса VUpdate есть одна реализация - UpdateNoCollisions. Предоставляет стандартное обновление частиц, без учета их столкновений.

#### Write

Для VWrite есть 3 стандартные реализации
+ StdCoutWrite - стандартный поток вывода stdout. Нет конструктора
+ StreamWrite - поток вывода std::ostream. Конструктор принимает константную ссылку на поток std::ostream
+ FileWrite - поток вывода std::ofstream. Два конструктора: от пути до файла и от потока std::ofstream

### 2. Свои реализации

Для любого из интересных классов есть возможность написать свои реализации. Для этого достаточно унаследовать от них и переопределить все функции.

## Классы контейнеры

### Vector3 - трехмерный вектор

Vector3 представляет собой трехмерный вектор.

В классе описаны 3 поля:
+ Параметр 1 (p1_). Тип: double
+ Параметр 2 (p2_). Тип: double
+ Параметр 3 (p3_). Тип: double

В классе определены следующие конструкторы:
+ Конструктор по умолчанию: все будет заполнено нулями
+ Конструктор от 3 переменных типа double: копирование этих переменных в параметры вектора
+ Копирующий конструктор: поэлементное копирование параметров
### Particle - частица

Класс Particle описывает состояние одной частицы

В классе описаны 4 поля:
+ Скорость частицы (velocity_). Тип Vector3
+ Координаты частицы (coordinates_). Тип Vector3
+ Масса частицы (mass_). Тип double
+ Спин частицы (I_). Тип float

Определены следующие конструкторы:
+ Конструктор по умолчанию: mass_ и I_ нули, velocity_ и coordinates_ - конструкторы по умолчанию
+ Конструктор от 4 переменных: Vector3 velocity, Vector3 coordinates, double mass, float I. Копирование velocity -> velocity_, coordinates -> coordinates_, mass -> mass_, I -> I_
+ Конструктор копирования: поэлементное копирование

Так же в классе Particle описана булевая функция isNormalVelocity(), она не принимает параметров и возвращает true, если все измерения скорости строго меньше скорости света.
### SystemState - состояние системы

Класс SystemState описывает текущее состояние системы

Класс хранит 2 параметра:
+ Текущее время (time_). Тип double
+ Вектор частиц (particles_). Тип std::vector\<Particle\>

Определены следующие конструкторы:
+ Конструктор по умолчанию: time_ равно нулю, particles_ размера 0;
+ Конструктор от 2 переменных: double time,  std::vector\<Particle\> particles. Копирование time -> time_, particles -> particles_
+ Конструктор копирования: поэлементное копирование

Для класса перегружены оператор квадратные скобки для индексирования по вектору particles_. При выходе за границы массива программа упадет с сообщением "Array of particles in SystemState.h out of range"

Для класса определена функция size(), которая возвращает размер particles_
### Dynamic - класс менеджер

Dynamic - это класс менеджер, контролирующий роботу всего фреймворка.
Он хранит в себе:
+ Состояние системы (state_). Тип SystemState
+ Внешнюю силу, действующую на частицы (external_f_). Тип std::function<Vector3(Particle)>
+ Силу, действующую между двумя частицами (f_btw_two_par_). Тип std::function<Vector3(Particle, Particle)>
+ Указатель на модуль чтения (reader_). Тип std::unique_ptr\<VRead\>
+ Указатель на модуль обновления (updater_). Тип std::unique_ptr\<VUpdate\>
+ Указатель на модуль записи (writer_). Тип std::unique_ptr\<VWrite\>

У него есть только один конструктор: на вход подаются:
+ std::unique_ptr\<VRead\> reader
+ std::unique_ptr\<VUpdate> updater
+ std::unique_ptr\<VWrite> writer
+ std::function\<Vector3(Particle)> external_f
+ std::function\<Vector3(Particle, Particle)> f_btw_two_par
  В конструкторе проходит копирование сил и передача указателей

#### simulate - метод симуляции

В Dynamic определен метод simulate, предназначенный для симуляции движения частиц. Метод получает время симуляции, разбивает его на заданное количество промежутков (промежутки по времени должны быть маленькими) и проводит обновление частиц за все эти промежутки.

На вход метод принимает 4 параметра:
+ num_of_iterations - количество итераций обновления. Тип int
+ time - общее время движения частиц. Тип double
+ num_of_particles - количество частиц. Тип size_t
+ save_states - флаг, нужно ли сохранять промежуточные значения системы. Тип bool

В начале считывается состояние системы

Дальше идут num_of_iterations итераций обновления частиц. На каждой итерации проверяется корректность частиц. Если save_states равен true, идет сохранение состояния.

В конце, когда все итерации пройдены, в зависимости от переменной save_states выводятся либо все состояния системы, либо только конечное.
## Интерфейс для представления сил

Для представления сил было решено использовать std::function, возвращающий Vector3 - силу. Есть два вида сил: взаимодействие двух частиц и постоянная сила. Первая описывается с помощью std::function<Vector3(Particle, Particle)> (на вход идут 2 частицы, на выходе Vector3). Вторая описывается с помощь std::function<Vector3(Particle)> (на вход идет одна частица, на выходе Vector3).

### 1. Стандартные реализации для сил


### 2. Добавление собственной силы

