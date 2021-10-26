# ants
Ants simulation in C++ with SFML

## Requirements
In order to compile you need CMake and SFML 2.5 or greater

```bash
# Mac
brew install cmake sfml

# Ubuntu
sudo apt install cmake libsfml-dev
```

## Build and Run

### 1.1 Compile with CMake
```bash
# Change working directory to this repository 
cd ants
# Create a directory to store build files
mkdir build
cd build
# Configure
cmake -DCMAKE_BUILD_TYPE=Release ../
# Build
cmake --build . -j4 --target Ants
```

### 1.2 Compile without CMake (GCC only)

```bash
mkdir build
cd build
g++ ../main.cpp ../src/*.cpp -I../include -lsfml-graphics -lsfml-window -lsfml-system -std=c++17 -o Ants
```

### 2. Run

```bash
./Ants
```