# GrayscaleCpp

A simple project showcasing various implementations of converting bitmap images (`.bmp`) to grayscale in C++ without using the OpenCV library. Each version demonstrates different optimization techniques (e.g., multithreading, mutex usage, vectorization). The goal is to experiment with performance improvements while maintaining a straightforward, pure C++ approach.

---

## Version 1 Notes

> **Why BMP and not JPEG?**  
> JPEG format involves complex compression and decompression methods (e.g., Huffman coding, DCT), requiring advanced image processing expertise if done from scratch without libraries like OpenCV. Therefore, this project uses the simpler **BMP (BitMap)** format, developed by Microsoft, which fits the needs of this pure C++ approach.

1. **BMP File Header**  
   - The first 54 bytes contain metadata such as image size, format, etc.
   - These bytes are stored in an `unsigned char baslik[]` array in the program.
   - The image dimensions in the project are fixed at **1088×1088** to simplify the logic.

2. **Storing Color and Grayscale Values**  
   - Each pixel's **red (r)**, **green (g)**, **blue (b)**, and **gray** values are stored as `unsigned char`, which can represent values from 0 to 255 (typical for 8-bit color channels).
   - Since we want integer arithmetic for pixel processing, we often use type casting if a `float` is involved in calculations.

3. **Grayscale Conversion**  
   - Nested `for` loops traverse rows (height) and columns (width) of the image.
   - **Weighted Grayscale Formula**:
     \[
     \text{gray} = 0.299 \cdot r + 0.587 \cdot g + 0.114 \cdot b
     \]
     - Human vision is more sensitive to green, hence the 0.587 factor.
     - Red is weighted at 0.299.
     - Blue is weighted at 0.114 (the least sensitive channel).

4. **Focus of Version 1**  
   - Only processes a **single** image.
   - Uses `float` for clarity and simplicity, despite some performance trade-offs.
   - Demonstrates the **basic approach** to convert an image to grayscale without further optimizations.

---

## Compilation Commands

Below are some examples of how to compile various versions of the program. Adjust flags as needed.

```bash
g++ -o grayscale-v7 grayscale-conversion-v7.cpp -std=c++11 -pthread -pg -O3
g++ -o grayscale-v6 grayscale-conversion-v6.cpp -std=c++11 -pthread -O3
g++ -o grayscale-v5 grayscale-conversion-v5.cpp -std=c++11 -pthread -O3
g++ -o grayscale-v4 grayscale-conversion-v4.cpp
g++ -o grayscale-v3 grayscale-conversion-v3.cpp
g++ -o grayscale-v2 grayscale-conversion-v2.cpp
g++ -o grayscale-v1 grayscale-conversion-v1.cpp
```

### Preparing Images for Processing

```bash
time taskset -c 0-11 ./kopyapasta  # Run in the "images" directory to manage input files
sudo mkdir -p /mnt/ramdisk
sudo mount -t tmpfs -o size=11G tmpfs /mnt/ramdisk
sudo cp -r ~/Desktop/GrayscaleCpp/images /mnt/ramdisk/
sudo chown -R $(whoami):$(whoami) /mnt/ramdisk/images
```

The above commands create a temporary filesystem in RAM to reduce I/O overhead for faster processing.

### Running the Program with 12 Threads

```bash
time taskset -c 0-11 ./grayscale-v7
```

This pins the process to CPU cores 0–11, ensuring parallel performance on a system with at least 12 cores.

### Cleaning Up (Removing Images from RAM)

```bash
sudo rm -r /mnt/ramdisk/images
sudo umount -l /mnt/ramdisk
sudo rm -r /mnt/ramdisk
```

## Screenshots and Version Highlights

### Version 4

<img src="https://github.com/can61cebi/GrayscaleCpp/blob/main/ciktilar/versiyon-4-thread.png" width="500">

Introduced multithreading for parallel processing.

### Version 5

<img src="https://github.com/can61cebi/GrayscaleCpp/blob/main/ciktilar/versiyon-5-mutex.png" width="500">

Added mutex to handle thread-safe shared data operations.

### Version 6

<img src="https://github.com/can61cebi/GrayscaleCpp/blob/main/ciktilar/versiyon-6-vectorization.png" width="500">

Vectorization applied for optimized pixel manipulation.

### Version 7

<img src="https://github.com/can61cebi/GrayscaleCpp/blob/main/ciktilar/versiyon-7-O2.png" width="500">

Spawning parent-child threads. Shown here with the -O2 optimization flag.

<img src="https://github.com/can61cebi/GrayscaleCpp/blob/main/ciktilar/versiyon-7-O3.png" width="500">

Same parent-child approach but compiled with -O3.
```
