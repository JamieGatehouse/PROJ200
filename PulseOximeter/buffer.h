#ifndef BUFFER_H_
#define BUFFER_H_

#include <stm32f4xx.h>

#define BUFFER_SIZE 15000

// Circular buffer structure
typedef struct {
    uint8_t data[BUFFER_SIZE]; // Buffer to hold data
    volatile uint16_t write_index;  // Write index for buffer
    volatile uint16_t read_index;   // Read index for buffer
    volatile uint16_t count;        // Number of elements in the buffer
} CircularBuffer;

// Function prototypes
void init_buffer(CircularBuffer* cb);  // Initialize the circular buffer
void write_to_buffer(CircularBuffer* cb, uint8_t data);  // Write data to buffer
uint8_t read_from_buffer(CircularBuffer* cb);  // Read data from buffer

#endif /* BUFFER_H_ */
