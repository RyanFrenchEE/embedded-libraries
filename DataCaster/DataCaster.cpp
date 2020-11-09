#include "DataCaster.h"

DataCaster::DataCaster() {
	for (uint8_t i = 0; i < 4; i++) {
		// init all values to 0
		current_bytes[i] = 0;
		byte_val[i] = 0;
	}
	long_val = 0;
	float_val = 0.0;
}

/*
 * Generic method to update all values
 */
void DataCaster::update() {
	update_arr();
	update_long();
	update_float();
}

// Method to update all values if input is character list
template <>
void DataCaster::value<uint8_t*> (uint8_t *input_val) {
	for (uint8_t i = 0; i < 4; i++) {
		this->current_bytes[i] = input_val[i];
	}
	update();
}

void DataCaster::update_arr() {
	for (uint8_t i = 0; i < 4; i++) {
		// update each byte in the array
		this->byte_val[i] = current_bytes[i];
	}
}

void DataCaster::update_long() {
	// reinterpret the 4-byte value into a long
	long *long_ptr = reinterpret_cast<long*>(current_bytes);
	this->long_val = *long_ptr;
}

void DataCaster::update_float() {
	// reinterpret the 4-byte value into a float
	float *float_ptr = reinterpret_cast<float*>(current_bytes);
	this->float_val = *float_ptr;
}
