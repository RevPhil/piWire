#ifndef TWOWIRE_H
#define TWOWIRE_H

/*
  Wire.h for Raspberry Pi
  
  Copyright (c) 2019 Reverend Phil Morris.  All rights reserved.
  
  A very simple emulation of the Arduino Wire library for Raspberry Pi use

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  
*/

#include <iostream>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <cerrno>

#define I2C_BUFFER_SIZE 32

class TwoWire {
public:
	int begin(void);
	void beginTransmission(int targetI2c);
	void write(uint8_t data);
	int writeTo(int targetI2c,void *buf,int numBytes);
	int endTransmission(void);
	int requestFrom(int targetI2c, int numBytes);
	int read(void);
	int readFrom(int targetI2c,void *buf, int numBytes);
	
	uint8_t buffer[(I2C_BUFFER_SIZE + 1)];
private:
	int fileHandle = 0;
	bool transmitting = false;
	uint8_t bufferPointer = 0;
	int currentI2c = -1;
};

int TwoWirebegin(void) {
	//----- OPEN THE I2C-1 BUS -----//
	// is the I2C buss already open? if so, just return the file handle
	if(fileHandle > 0) return fileHandle;
	fileHandle = open("/dev/i2c-1", O_RDWR);
	return fileHandle;	// fileHandle will be > 0 if successful
}

// SEND //
// bufferPointer is the number of Bytes in the buffer buffer + 1
// prepare to send data to Slave
void TwoWire::beginTransmission(int targetI2c) {
	bufferPointer = 0;
	transmitting = true;
	currentI2c = targetI2c;
}

// write a single Byte to the Slave (buffer)
void TwoWire::write(uint8_t data) {
	if(bufferPointer < I2C_BUFFER_SIZE && transmitting) buffer[bufferPointer++] = data;
}

//?????????
// write an array or variable to the Slave (buffer)
int TwoWire::writeTo(int targetI2c,void *buf,int len) {
	if(len > I2C_BUFFER_SIZE ) len = I2C_BUFFER_SIZE;
	uint8_t* txBytes = reinterpret_cast<uint8_t*>(buf);
	while(len--) write(*txBytes++);
	int result;
	if ((result = ioctl(fileHandle, I2C_SLAVE, targetI2c)) < 0) return result;	
	result = ::write(fileHandle, &buf, len);
	return result;
}

// send the buffered data to the Slave and end transmission
int TwoWire::endTransmission(void) {
	if(bufferPointer >= I2C_BUFFER_SIZE) bufferPointer = (I2C_BUFFER_SIZE);
	if(!transmitting) return -1;
	transmitting = false;
	int result;
	if ((result = ioctl(fileHandle, I2C_SLAVE, currentI2c)) < 0) return result;	
	result = ::write(fileHandle, &buffer, bufferPointer);
	currentI2c = 0;
	bufferPointer = 0;
	return result;
}


// RECEIVE //
// fetch the given number of Bytes from the Slave into the buffer
int TwoWire::requestFrom(int targetI2c, int numBytes) {
	bufferPointer = 0;
	int result;
	if(numBytes > I2C_BUFFER_SIZE) numBytes = I2C_BUFFER_SIZE;
	if ((result = ioctl(fileHandle, I2C_SLAVE, targetI2c)) < 0) return result;
	return ::read(fileHandle, &buffer, numBytes);
}

// read a single Byte from the buffer
int TwoWire::read(void) {
	if(bufferPointer >= I2C_BUFFER_SIZE) return -1;
	return buffer[bufferPointer++];
}

// read an array or variable from the buffer
int TwoWire::readFrom(int targetI2c,void *buf, int numBytes) {
	if(numBytes > I2C_BUFFER_SIZE) return numBytes = I2C_BUFFER_SIZE;
	int result;
	if((result = requestFrom(targetI2c,numBytes)) < 0) return result;
	uint8_t* rxBytes = reinterpret_cast<uint8_t*>(buf);
	while(numBytes--) *rxBytes++ = read();
	return numBytes;
}

extern TwoWire Wire;	// default object

#endif
