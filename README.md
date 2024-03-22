# 3DStylus: 3D Pointer and Simple Stroke Recognition (ongoing)

## Overview

C++ application for 3D Pointer and Simple Stroke Recognition using depth cameras.

## Features

- **Hand Isolation:** Efficiently isolates hands in image sequences.
- **Pointer Location:** Computes pointer location in the depth-image space.
- **Stroke Classification:** Classifies strokes based on pointer positions.

## Implementation

The application uses the [$1 Unistroke Recognizer](https://depts.washington.edu/acelab/proj/dollar/index.html).

## Prerequisites

- OpenCV (>= 3.0)

## Usage

### 1. Clone the repository
```bash
git clone https://github.com/SkxPhan/3DStylus.git
```
### 2. Compile the code
```bash
cd 3DStylus
mkdir build && cd build
cmake ..
make
```
### 3. Run the executable
```
./main
```

## TODO

- Improve hand segmentation,
- Handle 2 hands at the same time.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

