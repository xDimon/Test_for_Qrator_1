# Test_for_Qrator
Отчет по тестовому заданию #1 для соискателя позиции C/C++ Developer в компании Qrator

## Описание задачи
Реализовывать нужно без использования C++11

Дан граф из <= 50 нод, часть ребер ориентированные, часть нет.<br>
Требуется всем неориентированным ребрам задать ориентацию так, чтобы в итоговом графе не было циклов.

Граф задается матрицей смежности.

Формально: написать метод bool solve(const vector&lt;vector&lt;bool>> &in, vector&lt;vector&lt;bool>> &out):
- in - матрица смежности входного графа;
- out - матрица смежности получившегося графа без циклов.

Метод должен возвращать false, если задача не имеет решений, и true, если имеет.<br>
Метод должен работать &lt; 1c на современном процессоре, используя 1 поток.<br>
Можно считать, что входные данные корректны: in.size() > 0 && in.size() == in[0].size() && in[i].size() == in[j].size() && in[i][i] == false forall i,j &lt; in.size().

Пояснение по матрице:<br>
E[i][j] == true -  есть ориентированное ребро от i-ой вершины к j-ой.<br>
Если E[i][j] == true && E[j][i] == true, то есть оба ребра -> это неориентированное ребро.<br>
Отсутствие ребра - оба значения false.

## Окружение
Система: Ubuntu 15.10<br>
IDE: Eclipse CDT

## Сборка
Для сборки нужно вызвать make в папке build.

## Описание
Функция solve в первую очередь проверяет изначальную ацикличность графа (функция isAcyclic), так как при наличи цикла решения заведомо не может быть.<br>
Ацикличность проверяется путем поиска в глубину, следуя только ориентированым ребрам (функция depthFirstSearch). Если найден цикл, то граф неацикличный и дальнейший обход не делается.<br>
В случае, если граф изначально ацикличен, функция solve выбирает из графа все неориентированые ребра, при чем обход ведется по верхней треугольной матрице, а ребра для простоты (и для дедупликации) сохраняются в стек в виде пары нод с номерами "меньший->больший".<br>
Далее происходит рекурсивное ориентирование (функция orientEdges) сохраненных неориентированых ребер: извлекается обередное ребро из стека, сначала пробуем задать направление от меньшего к большему номеру ноды, затем наоборот.<br>
После каждой ориентации происходиит проверка наличие цикла от исходящей вершины только что ориентированного ребра. Если появился цикл, то вариант направления отвергаем и пробуем другой вариант.<br>
Если затем происходит рекурсивный вызов и ориентируется очередное ребро. Если на стеке нет ребер, это значит все ребра ориентированы, и в случае, если ацикличность сохраняется, это означает, что решение найдено.<br>
В иных случаях возвращаем ребру состояние неориентированности, возвращаем на стек и выходим (возвращаемся на предыдущий уровень рекурсии).<br>

