# computer_club_manager
Сборка и запуск программы без cmake:  
cd <project>  
g++ main.cpp logic.cpp -o manager.exe   
./manager.exe test_file.txt  


Тестирование программы (использован фреймворк gtest в сочетании со инструментарием cmake для сборки проекта):  
cd <project>  
mkdir build  (создаём рабочую директорию, чтобы в ней cmake  плодил все дополнительные файлы)  
cd build     (переходим в рабочую директорию)  
cmake .. -G "MinGW Makefiles"    (настраиваем cmake под mingw)  
cmake --build .                  (собираем всё, что можно разом, т.е. все цели cmake - ниже представлен консольный вывод команды)  

Можно отдельно собирать бинарь программы (cmake --build . --target manager) или тесты (cmake --build . --target unit_tests)  

После сборки все тесты (gtest) можно запустить так (из той же build директории):  
unit_tests.exe  

Посмотреть список тестов, при необходимости (стандартная gtest опция):  
unit_tests.exe --gtest_list_tests  

Запутить конкретный тест при необходимости (стандартная gtest опция):  
unit_tests.exe --gtest_filter=<имя_теста>  


