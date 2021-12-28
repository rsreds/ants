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

## Build

### Compile with CMake
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

### Compile without CMake (GCC only)

```bash
mkdir build
cd build
g++ ../main.cpp ../src/*.cpp -I../include -lsfml-graphics -lsfml-window -lsfml-system -std=c++17 -o Ants
```

## Run

```bash
./Ants
```

## Keys
| Control                   | Action                                 |
|---------------------------|----------------------------------------|
| **LMB** (LeftMouseButton) | Add food source at mouse location      |
| **H**                     | Show/Hide Heatmap                      |
| **X**                     | Reset simulation                       |
| **M + Arrow Up/Down**     | Increase/decrease markers lifetime     |
| **T + Arrow Up/Down**     | Increase/decrease ants hunting timeout |
| **R + Arrow Up/Down**     | Increase/decrease ants randomness      |

