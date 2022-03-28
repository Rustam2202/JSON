# JSON
Библиотека для поддержки формата JSON. Обрабатывает входной поток istream в подходящий тип и формирует данные в формате JSON в выходной поток ostream.
Класс json::Node хранит объект в переменной std::variant
Формируемые типы:
- nullptr_t;
- bool;
- int;
- double;
- std::string;
- std::vector;
- std::map.

Функционал:
- конструкторы создания объекта соответвующего типа;
- проверка объекта на соответсвие типа;
- получение значения;
- сравнение объектов одного типа.
