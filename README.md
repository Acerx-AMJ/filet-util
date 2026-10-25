# Filet
Filet is a file template utility tool that is capable of creating project and file templates as well as perform other common file/directory-modifying actions. It also contains localizations.

## Building
Filet only depends on C++17 and can be built using CMake:
```bash
cmake -B build
cmake --build build
```
The executable will be found in `build/filet`. The locales folder must be in the same directory in which the executable is ran. Or build using your favorite compiler:
```bash
g++ -std=c++17 -Iinclude src/*.cpp -o filet
```

## Usage
Run the following command to show all necessary info:
```bash
./filet -i
```

Here are examples of how to use Filet:
```bash
./filet -t repl          Create the REPL template.
./filet -f src/main.cpp  Create a new file.
./filet -d build lib     Create two folders.
./filet -c class         Create a class.cpp and class.hpp file pair.
```

## Credits
This project was made by Daniel Vishnevsky and is licensed under MIT License. It was made in August of 2025 and got published on October 2026 as an effort to make all of my projects open-source.
