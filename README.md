# piWire
 A simple emulation of the Arduino Wire library for the Raspberry Pi

piWire is a simple header file which provides a basic emulation of the Arduino 'Wire' library for the Raspberry Pi.

###The I2C interface MUST be enabled on the Raspberry Pi

The maximum Transmission length is 64 Bytes, only 32 Bytes can be handled by Arduino 'Wire' Slave devices.

The default object is 'Wire'.

The following 'Master' commands are supported:

	int begin(void);
	void beginTransmission(int targetI2c);
	void write(uint8_t data);
	int endTransmission(void);
	int requestFrom(int targetI2c, int numBytes);
	int read(void);
	
**int begin(void)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Opens the 'i2c-1' port for reading and writing.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*Wire.begin();*

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*returns* the filehandle if successful or an error number if unsuccessful.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'perror()' can be used to determine the error.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*if(Wire.begin() < 0) perror("begin");*

**void beginTransmission(int targetI2c)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Prepares the I2C buffer for transmission to the 'target' I2C address.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*Wire.beginTransmission(0x55);*

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*returns* nothing.

**void write(uint8_t data)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Writes single unsigned Bytes sequentially to the TX buffer (maximum 64 Bytes).

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*uint8_t data = 0xAA;*_

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*Wire.write(data);*

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*returns* nothing.

**int endTransmission(void)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Closes the TX buffer and sends the buffer contents to the I2C address specified in 'beginTransmission()'.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*Wire.endTransmission();*

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*returns* true if successful or an error number if unsuccessful.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'perror()' can be used to determine the error.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*if(Wire.endTransmission() < 0) perror("endTransmission");*

**int requestFrom(int targetI2c, int numBytes)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Requests the number of Bytes specified from the I2C address (maximum 64 Bytes).

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*Wire.requestFrom(0x55,4);*

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*returns* the number of Bytes received if successful or an error number if unsuccessful.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'perror()' can be used to determine the error.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*if(Wire.requestFrom(0x55,4) < 0) perror("requestfrom");*

**int read(void)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Reads single sequential Bytes from the RX buffer.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*uint8_t myByte = Wire.read();*

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*returns* the Byte read or '-1' if the buffer end has been reached.


**example**

\#include "piWire.h"

\#define TARGET_I2C 0x55

int main() {

if(Wire.begin() < 0) {  
perror("begin");  
return 1;  
}  

uint8_t _Byte1 = 1;
uint8_t _Byte2 = 2;
uint8_t _Byte3 = 3;

// write 3 Bytes to the target I2C address  
if(Wire.beginTransmission(TARGET_I2C) < 0) perror("beginTransmission");  
Wire.write(_Byte1);  
Wire.write(_Byte2);  
Wire.write(_Byte3);  
if(Wire.endTransmission() < 0) perror("endTransmission");  

// read 3 Bytes from the target I2C address  
int result = 0;  
if(result = Wire.requestFrom(TARGET_I2C,3) <0) perror("requestFrom");  
if(result == 3) {  
\_Byte1 = Wire.read();  
\_Byte2 = Wire.read();  
\_Byte3 = Wire.read();  
printf("Byte1 = %u\tByte2 = %u\tByte3 = %u\r\n",\_Byte1,\_Byte2,\_Byte3);  
}  

return 0;  
}
