1. [Структура проекта](#projstruct)
2. [О задаче](#aboutproblem)
3. [Алгоритмы параллельного решения](#parallelsolution)
4. [Benchmark](#benchmark)
5. [Выводы](#result)

## <a id="projstruct"></a>Структура проекта

* `include/` - папка с заголовочными файлами
* `src/` - папка с исходными кодами
* `src/main.cpp` - точка входа в программу
* `CMakeLists.txt` - файл для cmake
* `hr-go.go` - исходный код аналогичной программы на Golang (подробнее в разделе Benchmark)

С++17 стандарт только из-за использования std::filesystem::file_syze. Можно изменить способ измерения размера файла на различные костыли и код станет в соответствии с C++11.

Build проекта:
```
cmake .
make
```
Build Golang-решения:
```
go build hr-go.go
```

## <a id="aboutproblem"></a>О задаче

- В качестве хэш-функции я использовал CRC32. Таким образом, выбором некриптостойкой хеш-функции, предполагается вычисление контрольной суммы данных, полученных из доверенного источника.
- Каждое хэш-значение я записывал в конец выходного файла в формате 4-байтного числа, таким образом тратя 10 байт (*можно было бы всерьёз сэкономить, если вместо ASCII-значений цифр числа записывать прямо его 4 байтовое значение*). Т.е. вместо `CRC32("FOO") = "0450111403"` записывать 4 байта `0x1AD427AB`
- Хэш-функцию я скопипастил дабы соответствовать пункту о запрете сторонних библиотек. Иначе пришлось бы тянуть с собой Boost, OpenSSL или ещё что-то (*что было бы сделать более правильно, поскольку это криптография*)
- Более подходящим инструментом для решения этой задачи было бы использовать Golang.

## <a id="parallelsolution"></a>Алгоритмы параллельного решения

### Моё решение 

Создаём N потоков, каждый из которых выполняет следующий цикл:
- Получить уникальное смещение;
- Прочитать из входного файла блок данных по смещению;
- Посчитать хэш-значение от блока данных;
- Записать в выходной файл по смещению.

![Моя версия](/images/multithread-1.png "Визуализация")

### Более правильный и более сложный для реализации

- Один поток читает из входного файла и помещает блоки в буфер;
- Потоки из Thread Pool: берут из буфера блок, считают хэш-значение, помещают в другой буфер;
- Ещё один поток считывает данные из буфера с хэшами и записывает в выходной файл.

![Более архитектурно верное](/images/multithread-2.png?raw=true "Визуализация")

## <a id="benchmark"></a>Benchmark

Для того, чтобы увидеть повышение эффективности в многопроцессорной системе я сравнивал производительность следующих версий программы:

- `./hr` - моё предыдущее однопоточное решение
- `./hrmt` - нынешнее многопоточное решение
- `./hr-go` - многопоточное решение на Golang.

Все тесты производительности делались при помощи [Hyperfine](https://github.com/sharkdp/hyperfine). Все нижеописанные результаты получены при помощи 30 запусков. Вот такой командой:

```
hyperfine -r 30 --export-markdown md.md './hr input output_1' './hrmt input output_2' './hr-go input output_3'
```

*(после этого я сравнивал `md5sum output_*` полученных файлов, всё сходилось)*

1. 
* `входной файл` - файл размером 1 Гбайт (*GiB, 2^30 байт*)
* `размер блока` - 1 Мбайт (*MiB, 2^20 байт*)

| Command | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| `./hr input output_1` | 3.896 ± 0.009 | 3.883 | 3.927 | 5.46 ± 0.10 |
| `./hrmt input output_2` | 1.941 ± 0.012 | 1.930 | 1.990 | 2.72 ± 0.05 |
| `./hr-go input output_3` | 0.714 ± 0.013 | 0.688 | 0.738 | 1.00 |

2.
* `входной файл` - файл размером 5 Гбайт
* `размер блока` - 1 Мбайт

| Command | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| `./hr input output_1` | 19.469 ± 0.045 | 19.394 | 19.521 | 5.51 ± 0.05 |
| `./hrmt input output_2` | 9.696 ± 0.033 | 9.646 | 9.772 | 2.74 ± 0.02 |
| `./hr-go input output_3` | 3.536 ± 0.029 | 3.483 | 3.591 | 1.00 |


3. 
* `входной файл` - файл размером 1 Гбайт
* `размер блока` - 100 Мбайт

| Command | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| `./hr input output_1 104857600` | 4.489 ± 0.318 | 4.416 | 6.174 | 8.31 ± 3.04 |
| `./hrmt input output_2 104857600` | 2.178 ± 0.010 | 2.169 | 2.209 | 4.03 ± 1.45 |
| `./hr-go input output_3 104857600` | 0.540 ± 0.194 | 0.477 | 1.313 | 1.00 |

## <a id="result"></a>Выводы


По результатам тестов, в сравнении с предыдущим однопоточным решением, многопоточное запускается в ~2 раза быстрее, что объясняется тем, что запуски производились на двухядерном процессоре. На машине с большим количеством вычислительной мощи, можно ожидать кратного количеству ядер процессора эффекта увеличения скорости программы *(до того как всё упрётся в I/O bounds)*.

Golang показал себя куда лучше,  что показывает корректность выбора его в качестве инструмента для конкретно этой задачи. 
