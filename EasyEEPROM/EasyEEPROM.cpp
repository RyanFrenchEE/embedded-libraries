#include "EasyEEPROM.h"

EasyEEPROM::EasyEEPROM() {
	this->regs_used = 0;
	this->num_data = 0;
	this->eeprom_len = EEPROM.length();
}

void EasyEEPROM::write(unsigned int address, unsigned int value) {
	EEPROM.write(address, value);
}

unsigned int EasyEEPROM::read(unsigned int address) {
	return EEPROM.read(address);
}

void EasyEEPROM::update(unsigned int address, unsigned int value) {
	EEPROM.update(address, value);
}

unsigned int EasyEEPROM::get_index(const char *data_name) {
	for (int i = 0; i < NUM_DATA_MAX; i++) {
		if (strcmp(dict[i], data_name) == 0){
			// We've found the data name, return its index
			return i;
		}
	}
	// If data_name isn't in the array, return 9999
	return 9999;
}

unsigned int EasyEEPROM::get_index(char data_name) {
	for (int i = 0; i < NUM_DATA_MAX; i++) {
		const char *c = &data_name;
		if (strcmp(dict[i], c) == 0){
			// We've found the data name, return its index
			return i;
		}
	}
	// If data_name isn't in the array, return 9999
	return 9999;
}
