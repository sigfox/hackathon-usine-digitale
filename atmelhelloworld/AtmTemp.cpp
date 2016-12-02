/*****************************************************************************/
/*
  Arduino library for Atmel temperature module AT30TS75A.
  This library includes TWI(I2C) library because peculiar protocoll used.
  TWI library is Atmel developped.
*/
/*****************************************************************************/

/*
  Copyright (c) 2015 Daniele Denaro and
  - FuturaGroup srl (www.futurashop.it)
  - Open-Electronics (www.open-electronics.org)
  - Atmel Corporation.  
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#include "AtmTemp.h"

void ATMtemp::begin()
{
  pinMode(TEMPPWRON,OUTPUT);
  digitalWrite(TEMPPWRON,LOW);
  delay(300);
  twi_init();
}

float ATMtemp::readTemp()
{
  unsigned char ret=twi_read(TSENS_ADDR, twidata, 2);
  if (ret != TWI_RXDATA_NAK) {return -999;}
  word tw=word(twidata[0],twidata[1]);
  float temp=float(int(tw))/256;
  return temp;
}

void ATMtemp::end()
{
  digitalWrite(TEMPPWRON,HIGH);
}

/************************** TWI library *********************************/

/***********************************************************************************************************************
* Function Name: twi_init()
* Description : initialise TWI and check for temperature sensor
* Arguments : none
* Return Value : none
***********************************************************************************************************************/
void ATMtemp::twi_init(void)
{
	unsigned char twidata[2];

	// Two-Wire Serial Interface initialization
	// Communication Parameters: 100kBit/s
	TWBR = 72;		// 16MHz CPU clock, Prescaler = 1
	TWSR = 0x00;
	
	// test if sensor attached
	twi_read(TSENS_ADDR, twidata, 2);
}

/***********************************************************************************************************************
* Function Name: twi_send()
* Description : transmit data to twi slave
* Arguments : slave_addr: twi slave address
*             *data: data buffer
*             len: no. of data bytes to transfer
* Return Value : twi status code
***********************************************************************************************************************/
unsigned char ATMtemp::twi_send(unsigned char slave_addr, unsigned char *data, unsigned char len)
{
	unsigned long int timeout = TWI_TIMEOUTCNT;
	unsigned char status;
	unsigned char i;

    // Send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait to finish START
	while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
	status = TWSR;
	if((timeout > 0) && ((status & TWI_MASK) == TWI_START))
	{
		timeout = TWI_TIMEOUTCNT;
		// Send address to slave device with write access
		TWDR = slave_addr & TWI_WRITE;
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wait to finish address transmit
		while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
		status = TWSR;
		if((timeout > 0) && ((status & TWI_MASK) == TWI_TXADDR_ACK))
		{
			// Send data to slave device
			i = 0;
			do
			{
				timeout = TWI_TIMEOUTCNT;
				TWDR = *data++;
				TWCR = (1<<TWINT) | (1<<TWEN);
				// wait to finish data transmit
				while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
				status = TWSR;
			} while ((i++ < len) && (timeout > 0));
		}
	}
	if(timeout == 0) status = 0x98;		// Data transfer timed out


    // Send STOP condition
	timeout = TWI_TIMEOUTCNT;
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	// wait to finish STOP
	while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
	if(timeout == 0) status = 0x90;		// STOP could not be generated

	// Disable TWI	
	TWCR = 0;

	return status;
}

/***********************************************************************************************************************
* Function Name: twi_read()
* Description : read data from twi slave
* Arguments : slave_addr: twi slave address
*             *data: data buffer
*             len: no. of data bytes to transfer
* Return Value : twi status code
***********************************************************************************************************************/
unsigned char ATMtemp::twi_read(unsigned char slave_addr, unsigned char *data, unsigned char len)
{
	unsigned long int timeout = TWI_TIMEOUTCNT;
	unsigned char status;
	unsigned char i;

	// Send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait to finish START
	while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
	status = TWSR;
	if((timeout > 0) && ((status & TWI_MASK) == TWI_START))
	{
		timeout = TWI_TIMEOUTCNT;
		// Send address to slave device with read access
		TWDR = slave_addr | TWI_READ;
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wait to finish address transmit
		while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
		status = TWSR;
		if((timeout > 0) && ((status & TWI_MASK) == TWI_RXADDR_ACK))
		{
			// Read data from slave device with ACK (last byte with NAK)
			i = 0;
			do
			{
				timeout = TWI_TIMEOUTCNT;
				if(i < len-1) TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
				else		  TWCR = (1<<TWINT) | (1<<TWEN);
				// wait to finish data receive
				while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
				*data++ = TWDR;
				status = TWSR;
			} while ((i++ < len-1) && (timeout > 0));
		}
	}
	if(timeout == 0) status = 0x98;		// Data transfer timed out

	// Send STOP condition
	timeout = TWI_TIMEOUTCNT;
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	// wait to finish STOP
	while( !(TWCR & (1<<TWINT)) && timeout-- > 0);
	if(timeout == 0) status = 0x90;		// STOP could not be generated

	// Disable TWI	
	TWCR = 0;

	return status;
}
