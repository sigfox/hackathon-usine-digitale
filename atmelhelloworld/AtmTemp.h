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


#ifndef AT30TS75A_h
#define AT30TS75A_h

#include <Arduino.h>

#define TEMPPWRON 3            //Arduino pin for power supply enable(active low) 

/* TWI library defines */
#define TSENS_ADDR     0x90     //I2C module address 
#define TWI_TIMEOUTCNT	20000L	//CPU cycles
#define TWI_MASK	0xFC
#define TWI_READ	0x01
#define TWI_WRITE	0xFE
#define TWI_START       0x08
#define TWI_REPSTART    0x10
#define TWI_TXADDR_ACK  0x18
#define TWI_TXADDR_NAK  0x20
#define TWI_TXDATA_ACK  0x28
#define TWI_TXDATA_NAK  0x30
#define TWI_ARB_LOST    0x38
#define TWI_RXADDR_ACK  0x40
#define TWI_RXADDR_NAK  0x48
#define TWI_RXDATA_ACK  0x50
#define TWI_RXDATA_NAK  0x58
#define TWI_DONE	0xF8


class ATMtemp
{
  public:
  
  /*
  * Initialize temperature module
  **/
  void begin();
  
  /* 
  * Read temperature  (Celsius degrees)
  **/  
  float readTemp();
  
  /*
  * Disable temperature module
  **/  
  void end();
  
  
  private:
  
  void twi_init(void);
  unsigned char twi_send(unsigned char slave_addr, unsigned char *data, unsigned char len);
  unsigned char twi_read(unsigned char slave_addr, unsigned char *data, unsigned char len);
  unsigned char twidata[2];
 
};


#endif

