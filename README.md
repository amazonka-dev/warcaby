# Projekt C++: warcaby

Gra w warcaby napisana w języku C++. Aplikacja została skompilowana pod system operacyjny Linux.

## Instalacja narzędzi w systemie Linux Ubuntu

### Instalacja kompilatora GCC

Potrzebna minimalna wersja to gcc 11.4.

1. Zaktualizuj pakiet repozytoriów Ubuntu za pomocą następującej komendy:
```
sudo apt update
```

2. Zainstaluj pakiet build-essential:
```
sudo apt install build-essential
```

3. Sprawdź wersję GCC za pomocą następującej komendy:
```
gcc --version
```


### Instalacja zestawu narzędzi QT 5

Wersja Qt 5.15.3
```
sudo apt install qtcreator qtbase5-dev qt5-qmake cmake
```

Po zainstalowaniu narzędzi QT trzeba uruchomić QT Creator i wejść w ustawienia:
Narzędzia -> Opcje.

Wybierz zakładkę Kits (Zestawy narzędziowe), a następnie wybierz domyślny kit (zestaw).
Po przewinięciu formularza, który pojawi się poniżej wybranego zestawu, znajdź poniższe opcje i wypełnij je poprawnnymi danymi.

- Compiler/C++: ścieżka do kompilatora np. /usr/bin/g++
- QT version: wersja QT 5, należy ją wybrać z listy dostępnych.
- CMake Tool: ścieżka do programu cmake np. /usr/bin/gdb

## Budowanie projektu

Należy otworzyć plik projektu CMakeLists.txt za pomocą programu Qt Creator, a następnie wybrać opcję menu "Budowanie / Build All Projects".

Aby projekt uruchomć należy wybrać opcję menu "Budowanie / Uruchom".

Aby projekt uruchomć w trybie debugowania należy wybrać opcję menu "Debugowanie / Rozpocznij debugowanie".
