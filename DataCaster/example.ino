#include <DataCaster.h>
#include <Wire.h>

// DataCaster object
DataCaster rec_data;

// I2C address of device that's sending the data
byte I2C_ADDR = 0x12;

void setup()
{
    // Init serial and I2C
    Serial.begin(9600);
    Wire.begin(I2C_ADDR);
    Wire.onReceive(receive_event);

}

void loop()
{
    // Do nothing. Wait for incoming I2C data
}

void receive_event(int byte_count)
{
    byte receive_buffer[4];
    while (Wire.available()) {
        for (int i = 0; i < 4; i++) {
            // Populate the buffer
            receive_buffer[i] = Wire.read();
        }
    }

    // Send the data to process_data, which will print the values
    process_data(receive_buffer);
    delay(1);
}


void process_data(byte data[])
{
    byte val[4];
    val[0] = data[0];
    val[1] = data[1];
    val[2] = data[2];
    val[3] = data[3];

    // Update DataCaster value
    rec_data.value(val);

    // Print the values serially
    Serial.print("Received data (long): ");
    Serial.println(rec_data.long_val);
    Serial.print("Received data (float): ");
    Serial.println(rec_data.float_val);
}