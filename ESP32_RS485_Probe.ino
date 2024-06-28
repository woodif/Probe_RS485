#include "Crc16.h"
Crc16 crc;
uint16_t Modbus_read_ID1byte(uint8_t id, uint8_t function_code, uint16_t r_register) {
  byte data[8];
  data[0] = id;
  data[1] = function_code;
  data[2] = highByte(r_register);
  data[3] = lowByte(r_register);
  data[4] = highByte(0);
  data[5] = lowByte(1);
  crc.clearCrc();
  unsigned short value = crc.Modbus(data, 0, 6);
  data[6] = lowByte(value);
  data[7] = highByte(value);
  Serial1.write(data, 8);
  byte buffer[7];
  while (!Serial1.available());
  int size = Serial1.readBytes(buffer, 7);
  uint16_t ccnn = (uint16_t)word(buffer[6], buffer[5]);
  crc.clearCrc();
  value = crc.Modbus(buffer, 0, 5);
  if (ccnn == value) {
    return int(word(buffer[3], buffer[4]));
  } else {
    return 0;
  }
}

uint16_t Humidity = 000;
uint16_t Temperature = 001;

void Modbus_confi_ID1byte(uint8_t id, uint8_t function_code, uint16_t r_register, uint16_t datas) {
  byte data[8];
  data[0] = id;
  data[1] = function_code;
  data[2] = highByte(r_register);
  data[3] = lowByte(r_register);
  data[4] = highByte(datas);
  data[5] = lowByte(datas);
  crc.clearCrc();
  unsigned short value = crc.Modbus(data, 0, 6);
  data[6] = lowByte(value);
  data[7] = highByte(value);
  Serial1.write(data, 8);
}
void setup() {
  Serial.begin(115200);
  Serial1.begin(4800, SERIAL_8N1, 17, 16);
  Serial1.setTimeout(100);

}

void loop() {

  uint16_t Temperature_ = Modbus_read_ID1byte(0x01, 0x03, Temperature);
  delay(100);
  uint16_t Humidity_ = Modbus_read_ID1byte(0x01, 0x03, Humidity);
  delay(100);

  Serial.print("Temperature: ");
  Serial.println(float(Temperature_) / 10);
  Serial.print("Humidity: ");
  Serial.println(float(Humidity_) / 10);

  Serial.println("##########################");
  //Temperature : 28.60
  //Humidity : 63.70
  //PM 2.5 : 180
  //PM 10 : 185
  //Wind Speed : 0
  //Wind Direction : 0
  //Atmospheric : 995
}
