#include "buffer.h"

// Initialize the circular buffer
void init_buffer(CircularBuffer* cb)
{
    cb->write_index = 0;
    cb->read_index = 0;
    cb->count = 0;
}

// Write data to the circular buffer
void write_to_buffer(CircularBuffer* cb, uint8_t data)
{
    if (cb->count < BUFFER_SIZE) {
        cb->data[cb->write_index] = data;
        cb->write_index = (cb->write_index + 1) % BUFFER_SIZE;
        cb->count++;
    }
}

// Read data from the circular buffer
uint8_t read_from_buffer(CircularBuffer* cb)
{
    uint8_t data = 0;
    if (cb->count > 0) {
        data = cb->data[cb->read_index];
        cb->read_index = (cb->read_index + 1) % BUFFER_SIZE;
        cb->count--;
    }
    return data;
}
