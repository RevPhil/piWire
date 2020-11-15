# piWire
 A simple emulation of the Arduino Wire library for the Raspberry Pi

piWire is a simple header file which provides a basic emulation of the Arduino 'Wire' library for the Raspberry Pi.

The maximum Transmission length is 32 Bytes.

The default object is 'Wire'.

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

*Wire.begin();*

Opens the 'i2c-1' port for reading and writing.

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
