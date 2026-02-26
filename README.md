List Serializer

Doubly linked list Serializer w/ random indices

Формат входного файла inlet.in
    
    Каждая строка описывает один узел:
    <data>;<rand_index>
    Где:
    - <data> — строка (могут быть пробелы, спецсимволы, кодировка UTF-8)
    - <rand_index> — индекс узла, на который указывает rand, либо -1, если rand == nullptr
    
    Пример входного файла (inlet.in):
      apple;2
      banana;-1
      carrot;1
      
    Этот список:
      - Узел 0: "apple" → rand на узел 2
      - Узел 1: "banana" → rand = nullptr
      - Узел 2: "carrot" → rand на узел 1

Выходной файл (outlet.out)
Бинарный файл, содержащий сериализованное представление двусвязного списка.

Binary Representation:

      [NodesCount] 4 bytes
      [
        [dataLen] 4 bytes
        [data]    0-1000 bytes
        [randIdx] 4 bytes
      ] NodesCount times

Ограничения

      - Максимальное число узлов: 10⁶
      - data может быть длиной до 1000 символов

Build and run:

      1) clone repo
      2) run ./buildAndRun.sh

