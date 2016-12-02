/*****************************************************************************/
/*
  Arduino library for Atmel SIGFOX module ATA8520.
  This library uses Arduino SPI library.
  (Included for convenience into the same directory)
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

#ifndef SIGFOX_h
#define SIGFOX_h

#include <Arduino.h>

#define PWRON  8            // Arduino pin for module enable
#define NRES   9            // Arduino pin for module reset
#define NEVENT 2            // Arduino pin for module event interrupt

#define SHIELDBT 6
#define LED1 5    

#define BLEN  64            // Communication buffer length

/* NSS (SS inverse)(pin 10), MOSI(pin 11), MISO(pin 12), SCK(pin 13) */
/* are dfined in SPI library */


class SIGFOX
{
  public:
  
  /*
  * Initialize module (ready to transmit)
  */  
  void begin();
  /*
  * Send null terminated string as message to SIGFOX network
  * Max 12 characters long  
  * Return SIGFOX status code
  */  
  int sendMessage(char mess[]);
  /*
  * Send an array of bytes (max 12 bytes long) as message to SIGFOX network
  * Return SIGFOX status code
  */  
  int sendMessage(unsigned char mess[],int len);
  
  /*
  * Read status (fill ssm,atm,sig status variables)
  */
  void getStatus();  
  /*
  * Return status code.
  * Type: 0 -> ssm status ; 1 -> atm status ; 2 -> sigfox status    
  */  
  int getStatusCode(byte type);
  /*
  * Return atm status message 
  */  
  char* getStatusAtm();
  /*
  * Return SIGFOX status message
  */  
  String getStatusSig();
  /*
  * Return ATM version (major ver,minor ver)(two bytes)
  */  
  unsigned char* getAtmVersion();
  /*
  * Return SIGFOX version (major ver, minor ver) (two bytes)
  */  
  unsigned char* getSigVersion();
  /*
  * Return ID (4 bytes)
  */  
  unsigned char* getID();
  /*
  * Return PAC (16 bytes)
  */  
  unsigned char* getPAC();
  /*
  * Reset module
  */  
  void reset();
  /*
  * Test mode
  */  
  void testMode(byte frameL,byte frameH, byte chanL,byte chanH);
  /*
  *  Disable module
  */  
  void end();
  
  private:
  
 
  byte ssm;
  byte atm;
  byte sig;
  unsigned char buffer[BLEN];  // module communication buffer

};


#endif

