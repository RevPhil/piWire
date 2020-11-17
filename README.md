# piWire
 A simple emulation of the Arduino Wire library for the Raspberry Pi

piWire is a simple header file which provides a basic emulation of the Arduino 'Wire' library for the Raspberry Pi.

The maximum Transmission length is 32 Bytes. The default object is 'Wire'.

**Please Note: There is a problem with certain 'Slave' devices which causes bit 7 of the first Byte requested from the 'Slave' to be lost or zeroed. This is obvious when the 'Slave' device is an Arduino board using the Arduino 'Wire' library. Test are ongoing to see if this caveat is present on other I2C devices such as the DS323x RTC devices etc. I believe the problem is the result of a I2C protocol mismatch.**

The following 'Master' commands are supported:

	int begin(void);
	void beginTransmission(int targetI2c);
	void write(uint8_t data);
	void write(void *buf,int numBytes);
	int endTransmission(void);
	int requestFrom(int targetI2c, int numBytes);
	int read(void);
	int read(void *buf, int numBytes);
	
**int begin(void)**

Opens the 'i2c-1' port for reading and writing.

*Wire.begin();*

*returns* the filehandle if successful or an error number if unsuccessful.

'perror()' can be used to determine the error.

*if(Wire.begin() < 0) perror("begin");*

**void beginTransmission(int targetI2c)**

Prepares the I2C buffer for transmission to the 'target' I2C address.

*Wire.beginTransmission(0x55);*

*returns* nothing.

**void write(uint8_t data)**

Writes single unsigned Bytes sequentially to the TX buffer (maximum 32 Bytes).

*uint8_t data = 0xAA;*_

*Wire.write(data);*

*returns* nothing.

**void write(void *buf,int numBytes)**

Writes multiple Bytes from a variable or array etc. to the buffer (maximum 32 Bytes).

*float source = 12345.12345;*

*Wire.write(&source,sizeof(source));*

*returns* nothing.

**int endTransmission(void)**

Closes the TX buffer and sends the buffer contents to the I2C address specified in 'beginTransmission()'.

*Wire.endTransmission();*

*returns* true if successful or an error number if unsuccessful.

'perror()' can be used to determine the error.

*if(Wire.endTransmission() < 0) perror("endTransmission");*

**int requestFrom(int targetI2c, int numBytes)**

Requests the number of Bytes specified from the I2C address (maximum 32 Bytes).

*Wire.requestFrom(0x55,4);*

*returns* the number of Bytes received if successful or an error number if unsuccessful.

'perror()' can be used to determine the error.

*if(Wire.requestFrom(0x55,4) < 0) perror("requestfrom");*

**int read(void)**

Reads single sequential Bytes from the RX buffer.

*uint8_t myByte = Wire.read();*

*returns* the Byte read or '-1' if the buffer end has been reached.

*if(Wire.read() < 0) printf("read failed!");*

**int read(void *buf, int numBytes)**

Reads multiple Bytes from the RX buffer into a variable or array etc.

*float data = 0.0;*

*Wire.read(&data,sizeof(data));*

*returns* the number of Bytes read or '-1' if the buffer end has been reached.

*if(Wire.read(&data,sizeof(data)) < 0) printf("read failed!");*

**example**

\#include "piWire.h"

\#define TARGET_I2C 0x55

int main() {

if(Wire.begin() < 0) perror("begin");

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
