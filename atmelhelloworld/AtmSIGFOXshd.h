/*****************************************************************************/
/*
  Arduino library for Atmel SIGFOX shield ATAB0101A.
  This library concerns the Atmel shield that includes some hardware:
  - a SIGFOX module on SPI bus
  - a temperature sensor on I2C bus
  - a push-button
  - a couple of leds (but just one (red) user managed) (green led for temp. on)
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



#ifndef AtmSIGFOXshd_h
#define AtmSIGFOXshd_h

#include <Arduino.h>

#include "AtmSIGFOX.h"
#include "AtmTemp.h"

#define LED1 5               // Arduino pin for user led (red)) 
#define SHIELDBT 6           // Arduino pin for push-button
#define VPWRANA 0            // Arduino analogical pin wired with power supply V

SIGFOX   SIG;                // load class SIGFOX (Atm SIGFOX module library)
ATMtemp  TEMP;               // load class ATMtemp (temperature module library)



class SIGFOXshield
{
  public:
  
  /*** Generic shield functions ***/
  
  /*
  * Initialize shield library.
  * If shield doesn't replay, function returns false
  * else true
  */
  boolean begin();
  /*
  * Get button status
  */              
  boolean getButton();
  /*
  * Read voltage value
  */
  int getVpwr();
  /*
  * Switch off user led
  */
  void LedON();
  /*
  * Switch on user led
  */
  void LedOFF();
  /*
  * Blink user led for time milliseconds
  */
  void LedBlink(int time);
  
  /*** Convenience functions for temperature module ***/
  /*** (call ATMtemp library functions)***/

  /*
  * Read temperature (Celsius degrees)
  */
  float readTemp(){return TEMP.readTemp();}
  /*
  * Switch off temperature module
  */
  void TempOff(){TEMP.end();}
  /* 
  * Switch on temperature module
  */  
  void TempOn(){TEMP.begin();}


  /*** Convenience functions for SIGFOX module ***/
  /*** (call AtmSIGFOX library functions) ***/
  
  /*
  * Send null terminated string message to SIGFOX network
  */  
  int sendMessage(char mess[]){return SIG.sendMessage(mess);}
  /*
  * Send bytes array buffer to SIGFOX network
  */  
  int sendMessage(unsigned char mess[],int len){return SIG.sendMessage(mess,len);}

  /*
  * Switch off SIGFOX module
  */
  void SIGFOXoff(){SIG.end();}
  /*
  * Switch on SIGFOX module
  */  
  void SIGFOXon(){SIG.begin();}

  /*
  * Read status. I.E. fill status variables (ssm,atm,sig)
  * You can read these variable using getStatusCode 
  * or you can get corresponding string with getStatusAtm/Sig functions
  */
  void readStatus(){SIG.getStatus();}
  /*
  * Return status code.
  * Type: 0 -> ssm status ; 1 -> atm status ; 2 -> sigfox status    
  */  
  int getStatusCode(byte type){return SIG.getStatusCode(type);}
  /*
  * Return atm status message 
  */  
  char* getStatusAtm(){return SIG.getStatusAtm();}
  /*
  * Return SIGFOX status message
  */  
  String getStatusSig(){return SIG.getStatusSig();}
  /*
  * Return ATM version (major ver,minor ver)(two bytes)
  */  
  unsigned char* getAtmVersion(){return SIG.getAtmVersion();}
  /*
  * Return SIGFOX version (major ver, minor ver) (two bytes)
  */  
  unsigned char* getSigVersion(){return SIG.getSigVersion();}
  /*
  * Return ID (4 bytes)
  */  
  unsigned char* getID(){return SIG.getID();}
  /*
  * Return PAC (16 bytes)
  */  
  unsigned char* getPAC(){return SIG.getPAC();}
  
};

  boolean SIGFOXshield::begin()
  {
    SIG.begin();                    // initialize SIGFOX module
    TEMP.begin();                   // initialize Temperature module
    pinMode(LED1,OUTPUT);           // initialize Led
    pinMode(SHIELDBT,INPUT_PULLUP); // initialize push-button
    unsigned char *buff=SIG.getSigVersion();
    if ((buff[0]|buff[1])==0) return false;
    return true;
  }

  boolean SIGFOXshield::getButton()
  {
    if (digitalRead(SHIELDBT)==LOW) return true;
    else return false;
  }
  
  int SIGFOXshield::getVpwr()
  {
    long v=analogRead(VPWRANA);
    v=(v*1778)/100;
    return v;
  }
  
  void SIGFOXshield::LedON(){analogWrite(LED1,5);}
  void SIGFOXshield::LedOFF() {digitalWrite(LED1,0);}
  void SIGFOXshield::LedBlink(int time)
  {
    int it=time/100;int i; 
    for (i=0;i<it;i++) 
     {analogWrite(LED1,10);delay(100);analogWrite(LED1,0);delay(100);} 
  }
  

#endif

