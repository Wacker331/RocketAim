# RocketAim
Test case for kotlin-novator

## Сложное решение ("Difficult" branch): //Получилось :)
Тест: расставить цели квадратом со стороной 10, радиус = 8 -> выстрел в середину квадрата

Есть проблемы с округлением, поэтому квадрат со сторонами 11 и радиусом поражения 8 не вычисляет правильно

Можно начать использовать числа с плавающей точкой, но при большом количестве целей вычислений будет очень много, а вещественные числа могут серьезно замедлить программу
