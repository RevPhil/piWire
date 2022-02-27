/*
 * piWire.cxx
 */
#include "piWire.h"
#include <iostream>

#define I2C_ADDRESS 0x09

int main(int argc, char **argv)
{
	// start the I2C bus and initialise the Wire library
	if(Wire.begin() < 0) perror("begin");
	
	// send 32 Bytes to the Slave device
	Wire.beginTransmission(I2C_ADDRESS);
	for(int x = 0;x < 32;x++) Wire.write(x);
	if(Wire.endTransmission() < 0) perror("endTransmission");
	
	// send a string of characters to the Slave device
	char myString[] {"Hello World!"};
	if(Wire.writeTo(I2C_ADDRESS,&myString,sizeof(myString)) <0) perror("writeTo");
	
	return 0;
}

