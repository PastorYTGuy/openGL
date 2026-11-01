Projekt renderowania 3D za pomocą OpenGL. Stworzony z pomocą Qt oraz CMake.

Celem projektu jest ewolucja do pełnego edytora graficznego wraz z dodawaniem coraz to nowszych mechanizmów.

Wymagania Wstępne:
* **Qt Creator** (zalecane) lub inne środowisko wspierające CMake.
* Kompilator C++ wspierający standard **C++17** (np. MinGW / MSVC).
* **Qt 6** (z modułami OpenGL / Widgets).

Projekt tworzony oryginalnie w Qt Creatorze.
Jak uruchomić:
- uruchom Qt Creator
- File/Open file or project
- wybierz plik **CMakeLists.txt**
- skonfiguruj building kit wybierając desktop Qt (np. **Qt 6.10.2 MinGW**)
- wciśnij skonfiguruj projekt
- Build
- Run
- Enjoy!

lub wersja terminalowa:
mkdir build
cd build
cmake ..
cmake --build .
