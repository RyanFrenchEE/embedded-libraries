/*
 * A wrapper class for the Arduino's EEPROM library.
 *
 * All setting is done with calls to EEPROM's update() method, which extends
 * the lifetime of the EEPROM by eliminating unncessary writes.
 *
 * Created by Ryan French
 * github.com/ryanfrenchphysics
 */

#ifndef _EASYEEPROM_H
#define _EASYEEPROM_H

#include <Arduino.h>
#include <EEPROM.h>

const int NUM_DATA_MAX = 50;

struct EEPROMInfo {
	// Struct in which the relevant information is stored:
	// The data's length and its location in EEPROM
	unsigned int data_len;
	unsigned int reg;
};

class EasyEEPROM {
public:
	EasyEEPROM();

	template<typename T>
	void add_data(const char *name, T data);

	template<typename T>
	void add_data(char name, T data);

	// Save a given data (variable/array) by name
	template<typename T>
	void save(const char *name, T data);

	template<typename T>
	void save(char name, T data);

	// Read a data's value by name
	template<typename T>
	void load(const char *name, T& data);

	template<typename T>
	void load(char name, T& data);

	// Basic EEPROM methods, if needed:
	void write(unsigned int address, unsigned int value);
	unsigned int read(unsigned int address);
	void update(unsigned int address, unsigned int value);
	template<typename T>
	void put(unsigned int address, T data);
	template<typename T>
	void get(unsigned int address, T data);

private:

	unsigned int eeprom_len;
	unsigned int regs_used;
	unsigned int num_data;

	// Create arrays for EEPROMInfo and data arrays
	EEPROMInfo info[NUM_DATA_MAX];
	const char* dict[NUM_DATA_MAX];
	unsigned int get_index(const char *data_name);
	unsigned int get_index(char data_name);
};


template<typename T>
void EasyEEPROM::add_data(const char *name, T data) {
	unsigned int size = sizeof(data);
	if (regs_used + size < eeprom_len) {
		info[num_data].data_len = size;
		info[num_data].reg = regs_used;
		dict[num_data] = name;
		num_data++;
		regs_used += size;
	}
}

template<typename T>
void EasyEEPROM::add_data(char name, T data) {
	unsigned int size = sizeof(data);
	if (regs_used) + size < eeprom_len) {
		info[num_data].data_len = size;
		info[num_data].reg = regs_used;
		dict[num_data] = name;
		num_data++;
		regs_used += size;
	}
}

template<typename T>
void EasyEEPROM::save(const char *name, T data) {
	unsigned int index = get_index(name);
	if (index != 9999) {
		unsigned int reg = info[index].reg;
		EEPROM.put(reg, data);
	}
}

template<typename T>
void EasyEEPROM::save(char name, T data) {
	unsigned int index = get_index(name);
	if (index != 9999) {
		unsigned int reg = info[index].reg;
		EEPROM.put(reg, data);
	}
}

template<typename T>
void EasyEEPROM::load(const char *name, T& data) {
	unsigned int index = get_index(name);
	if (index != 9999) {
		unsigned int reg = info[index].reg;
		EEPROM.get(reg, data);
	}
}

template<typename T>
void EasyEEPROM::load(char name, T& data) {
	unsigned int index = get_index(name);
	if (index != 9999) {
		unsigned int reg = info[index].reg;
		EEPROM.get(reg, data);
	}
}

template<typename T>
void EasyEEPROM::put(unsigned int address, T data) {
	EEPROM.put(address, data);
}

template<typename T>
void EasyEEPROM::get(unsigned int address, T data) {
	EEPROM.get(address, data);
}

#endif // _EASYEEPROM_H
