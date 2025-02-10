#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "inc/ssd1306.h"
#include "matrix.h"
#include "hardware/i2c.h"
#include "inc/font.h"
#include "string.h"

// Definitions for the integrated OLED display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADDRESS 0x3C

// Peripheral pin definitions
const uint8_t LED_R = 13, LED_B = 12, LED_G = 11;
const uint8_t BUTTON_A = 5, BUTTON_B = 6, BUTTON_JOY = 22;

// Global variables for matrix drawing
PIO pio;
uint sm;

// Global variable to track time and debounce interrupts
static volatile uint32_t previous_time = 0;

// LED control variables
static volatile bool led_g_on = false, led_b_on = false;

// Initialize the display structure
ssd1306_t ssd;

// Function prototypes
void initialize_leds();
void initialize_buttons();
void set_rgb(char color, bool state);
void update_matrix(char key);
void update_display(char msg);
static void gpio_irq_handler(uint gpio, uint32_t events);
void initialize_oled_display();

int main() {
    stdio_init_all();

    // Initialize PIO for LED matrix
    pio = pio0;
    sm = configure_matrix(pio);
    configure_numbers();

    // Variable for serial input
    char msg = '\0';
    bool color = true;

    // Configure GPIO for LEDs and buttons
    initialize_leds();
    initialize_buttons();
    initialize_oled_display();

    // Set up interrupt triggers for buttons
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_JOY, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        // Update the LED matrix
        update_display(msg);

        // Check if USB connection is active
        if (stdio_usb_connected()) {
            // Read character from the queue
            if (scanf("%c", &msg) == 1) {
                update_matrix(msg); // Update the LED matrix with the new character
                printf("Character received: %c\n", msg);
                update_display(msg); // Immediately update the OLED display
            }
        }

        sleep_ms(500);
    }
}

/*
|   Function: initialize_leds
|   Configures the RGB LED pins as outputs
*/
void initialize_leds() {
    // Red LED
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    // Green LED
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Blue LED
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
}

/*
|   Function: initialize_buttons
|   Configures buttons A, B, and joystick as inputs with pull-up resistors
*/
void initialize_buttons() {
    // Button A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Button B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Joystick button
    gpio_init(BUTTON_JOY);
    gpio_set_dir(BUTTON_JOY, GPIO_IN);
    gpio_pull_up(BUTTON_JOY);
}

/*
|   Function: initialize_oled_display
|   Configures and initializes the SSD1306 OLED display for use
|   Communication is done via I2C
*/
void initialize_oled_display() {
    // Initialize I2C at 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line

    // Initialize and configure the display
    ssd1306_initialize(&ssd, DISPLAY_WIDTH, DISPLAY_HEIGHT, false, ADDRESS, I2C_PORT);
    ssd1306_configure(&ssd);
    ssd1306_send_data_to_display(&ssd);

    // Clear the display
    ssd1306_clear_display(&ssd, false);
    ssd1306_send_data_to_display(&ssd);
}

/*
|   Function: set_rgb
|   Turns on or off the RGB LED based on the color and state parameters
|   char color: 'R' = red, 'G' = green, 'B' = blue, 'W' = white, '-' = off
*/
void set_rgb(char color, bool state) {
    switch (color) {
        case 'R':
            gpio_put(LED_R, state);
            break;
        case 'G':
            gpio_put(LED_G, state);
            break;
        case 'B':
            gpio_put(LED_B, state);
            break;
        case 'W':
            gpio_put(LED_R, state);
            gpio_put(LED_G, state);
            gpio_put(LED_B, state);
            break;
        case '-':
            gpio_put(LED_R, 0);
            gpio_put(LED_G, 0);
            gpio_put(LED_B, 0);
            break;
        default:
            printf("Invalid character!!\nPlease provide R, G, B, W, or -\n");
            break;
    }
}

/*
|   Function: gpio_irq_handler
|   Callback function for GPIO interrupt handling
|   Implements software debouncing and controls the state of the green and blue LEDs
|   Also activates the bootsel mode
*/
static void gpio_irq_handler(uint gpio, uint32_t events) {
    // Get the current execution time
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Debounce: Ensure the button is pressed for at least 200ms
    if (current_time - previous_time > 200000) {
        previous_time = current_time;

        // Handle the interrupt
        if (gpio == BUTTON_A) {
            led_g_on = !led_g_on;
            set_rgb('G', led_g_on);
            printf("Green LED is now: %d\n", led_g_on);
        } else if (gpio == BUTTON_B) {
            led_b_on = !led_b_on;
            set_rgb('B', led_b_on);
            printf("Blue LED is now: %d\n", led_b_on);
        } else if (gpio == BUTTON_JOY) {
            reset_usb_boot(0, 0);
        }

        update_display('\0');
    }
}

/*
|   Function: update_matrix
|   Receives a character as a parameter and converts it to an equivalent integer.
|   If it's a number between 0-9, the number is displayed on the matrix; otherwise, the matrix is cleared.
*/
void update_matrix(char key) {
    // Check if the character is a number
    if (key >= '0' && key <= '9') {
        uint value = key - 48; // Convert ASCII to integer
        display_number(value, pio, sm); // Display the number on the matrix
    } else {
        clear_matrix(pio, sm); // Clear the matrix
    }
}

/*
|   Function: update_display
|   Updates the data displayed on the OLED display
*/
void update_display(char msg) {
    printf("Updating display\n");

    // Clear the display
    bool color = true;
    char str1[20] = "Blue LED", str2[20] = "Green LED";

    // Check the state of the blue LED
    if (led_b_on) {
        strcat(str1, " On\0");
    } else {
        strcat(str1, " Off\0");
    }

    // Check the state of the green LED
    if (led_g_on) {
        strcat(str2, " On\0");
    } else {
        strcat(str2, " Off\0");
    }

    ssd1306_clear_display(&ssd, !color); // Clear the display
    ssd1306_draw_rectangle(&ssd, 3, 3, 122, 58, color, !color); // Draw a rectangle around the edges

    // Display the state of the blue LED
    ssd1306_draw_string(&ssd, str1, 10, 8);

    // Display the state of the green LED
    ssd1306_draw_string(&ssd, str2, 10, 16);

    // Display the last character received
    ssd1306_draw_string(&ssd, "Last character:", 10, 24);
    if (msg != '\0') {
        ssd1306_draw_character(&ssd, msg, 60, 40);
    }

    ssd1306_send_data_to_display(&ssd); // Update the display
}