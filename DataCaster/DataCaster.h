/*
 * Library to cast 4-byte values using safe pointers. Useful
 * for data sent serially or by I2C.
 * 
 * Created by Ryan French
 * github.com/ryanfrenchphysics
 */


#ifndef DATA_CASTER_H_
#define DATA_CASTER_H_

#include <Arduino.h>
#include <stdint.h>

class DataCaster {
public:
	DataCaster();

	template <typename T>
	void value(T inputValue);

	uint8_t byte_val[4];
	long long_val;
	float float_val;

private:
	uint8_t current_bytes[4];
	void update();
	void update_arr();
	void update_long();
	void update_float();
};

// Method to update all values in DataCaster
template <typename T>
void DataCaster::value(T input_val) {
	T *input_ptr = &input_val;
	uint8_t *casted_ptr = reinterpret_cast<uint8_t*>(input_ptr);

	for (uint8_t i = 0; i < 4; i++) {
		this->current_bytes[i] = casted_ptr[i];
	}
	update();
}

#endif // DATA_CASTER_H
