#ifndef MATRIX_H
#define MATRIX_H

#include "hardware/pio.h"

// Structure to control the color of LEDs (RGB values)
typedef struct {
    double red;
    double green;
    double blue;
} LedColor;

// Alias for the RGB color structure
typedef LedColor RGBColor;

// Matrix of LEDs (5x5 grid)
typedef LedColor LedMatrix[5][5];

// Animation frames structure
typedef struct {
    LedMatrix *frames[5]; // Array of frames
    int frame_count;      // Number of frames
} AnimationFrames;

// Function prototypes
uint32_t generate_binary_color(double red, double green, double blue);
uint configure_matrix(PIO pio);
void display_pattern(LedMatrix pattern, PIO pio, uint sm);
void run_animation(int animation_index, PIO pio, uint sm);
void configure_animations();
void display_number(uint number, PIO pio, uint sm);
void configure_numbers();
void clear_matrix(PIO pio, uint sm);
RGBColor get_color_from_rgb(int red, int green, int blue);

#endif // MATRIX_H