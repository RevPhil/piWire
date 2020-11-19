#ifndef PIWIRE_H
#define PIWIRE_H

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
#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <cerrno>

#define I2C_BUFFER_SIZE 64

class piWire {
public:
	int begin(void);
	void beginTransmission(int targetI2c);
	void write(uint8_t data);
	int endTransmission(void);
	int requestFrom(int targetI2c, int numBytes);
	int read(void);
	
	uint8_t buffer[(I2C_BUFFER_SIZE + 1)];
private:
	int file_i2c;
	bool transmitting = false;
	uint8_t bufferPointer = 0;
	int currentI2c = -1;
};

int piWire::begin(void) {
	//----- OPEN THE I2C-1 BUS -----
	char *filename = (char*)"/dev/i2c-1";
	file_i2c = open(filename, O_RDWR);
	return file_i2c;
}

// SEND //
// bufferPointer is the number of Bytes in the buffer buffer + 1
// prepare to send data to Slave
void piWire::beginTransmission(int targetI2c) {
	bufferPointer = 0;
	transmitting = true;
	currentI2c = targetI2c;
}

// write a single Byte to the Slave (buffer)
void piWire::write(uint8_t data) {
	if(bufferPointer < I2C_BUFFER_SIZE && transmitting) buffer[bufferPointer++] = data;
}


// send the buffered data to the Slave and end transmission
int piWire::endTransmission(void) {
	if(bufferPointer >= I2C_BUFFER_SIZE) bufferPointer = (I2C_BUFFER_SIZE);
	if(!transmitting) return -1;
	transmitting = false;
	int result;
	if ((result = ioctl(file_i2c, I2C_SLAVE, currentI2c)) < 0) return result;	
	result = ::write(file_i2c, &buffer, bufferPointer);
	bufferPointer = 0;
	return result;
}


// RECEIVE //
// fetch the given number of Bytes from the Slave into the buffer
int piWire::requestFrom(int targetI2c, int numBytes) {
	bufferPointer = 0;
	int result;
	if(numBytes > I2C_BUFFER_SIZE) numBytes = I2C_BUFFER_SIZE;
	if ((result = ioctl(file_i2c, I2C_SLAVE, targetI2c)) < 0) return result;
	return ::read(file_i2c, buffer, numBytes);
}

// read a single Byte from the buffer
int piWire::read(void) {
	if(bufferPointer >= I2C_BUFFER_SIZE) return -1;
	return buffer[bufferPointer++];
}


piWire Wire;	// default object

#endif
