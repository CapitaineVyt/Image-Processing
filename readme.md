# Image Processing in C (BMP Manipulation)

A comprehensive C programming project focused on manipulating and processing **8-bit (Grayscale)** and **24-bit (RGB)** BMP images. This library implements low-level file handling, dynamic memory management, and mathematical image processing algorithms from scratch.

## 📋 Project Overview
[cite_start]This project was developed for the **Algorithm and Data Structures 1** module at **Efrei Paris**. [cite_start]The goal is to build a robust image processing engine using only the C standard library[cite: 11].

## ✨ Key Features

### 1. Grayscale Image Processing (8-bit)
* [cite_start]**File Handling**: Implementation of `bmp8_loadImage` and `bmp8_saveImage` to handle 8-bit depth images with mandatory color tables[cite: 160, 180, 200, 218].
* [cite_start]**Basic Transforms**: Inversion (negative), brightness adjustment, and image binarization (thresholding)[cite: 249, 256, 264].
* [cite_start]**Convolution Engine**: A 2D convolution system using odd-sized kernels to apply filters such as Box Blur, Gaussian Blur, Outline, Emboss, and Sharpen[cite: 276, 288, 302].

### 2. Color Image Management (24-bit)
* [cite_start]**Direct RGB Access**: Handling images with 3 color channels (Red, Green, Blue) without a color table[cite: 320, 324].
* [cite_start]**Advanced Architecture**: Implementation of structured headers (`t_bmp_header`, `t_bmp_info`) and pixel matrices for high-fidelity manipulation[cite: 343, 360].
* [cite_start]**Average Grayscale**: Conversion of color images to grayscale by calculating the mean of the three channels[cite: 528].

### 3. Histogram Equalization
* [cite_start]**Grayscale Equalization**: Improving global contrast using Cumulative Distribution Function (CDF) normalization[cite: 583, 590, 638].
* [cite_start]**YUV Color Space**: Enhancing color image contrast by converting RGB to YUV, equalizing the luminance (Y) component, and converting back to preserve original hues[cite: 695, 698, 711].

## 🛠️ Data Structures
* [cite_start]**`t_bmp8`**: Dedicated to 8-bit images, containing the 54-byte header and 1024-byte color table[cite: 168, 171].
* **`t_bmp24`**: Dedicated to 24-bit images, utilizing a 2D matrix of `t_pixel` structures[cite: 351, 358].
* [cite_start]**`t_pixel`**: A triplet of `uint8_t` for Red, Green, and Blue channels[cite: 383, 388].
