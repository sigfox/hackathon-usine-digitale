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


#include "AtmSIGFOX.h"
#include "SPI.h"

const String statusCodes[] = {
    "OK", 
    "Manufacturer error",
    "ID or key error", 
    "State machine error",
    "Frame size error",
    "Manufacturer send error",
    "Get voltage/temperature error",
    "Close issues encountered",
    "API error indication",
    "Error getting PN9",
    "Error getting frequency",
    "Error building frame",
    "Error in delay routine",
    "Timeout interrupt no transmission"
};

void SIGFOX::begin()
{
  buffer[0]=0;
  pinMode(NEVENT,INPUT_PULLUP);
  digitalWrite(PWRON,LOW);
  pinMode(PWRON,OUTPUT);
  digitalWrite(NRES,HIGH);
  pinMode(NRES,OUTPUT);
  digitalWrite(NRES,LOW);
  digitalWrite(PWRON,HIGH);
  delayMicroseconds(20);
  digitalWrite(NRES,HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST); 
//  SPI.setBitOrder(LSBFIRST); 
  pinMode(SHIELDBT,INPUT_PULLUP);
  delay(20);
}

int SIGFOX::sendMessage(char mess[])
{
  int len=strlen(mess);
  return sendMessage((unsigned char*)mess,len);
}

int SIGFOX::sendMessage(unsigned char mess[],int len)
{
  if (len==0) return -1;
  getStatus();             //reset NNEVENT
  digitalWrite(SS,LOW); delayMicroseconds(50);
  if (len>12) len=12;
  SPI.transfer(0x07); delayMicroseconds(50);


  SPI.transfer(len); delayMicroseconds(50);
  int i;
  for(i=0;i<len;i++) {SPI.transfer(mess[i]); delayMicroseconds(50);}
  digitalWrite(SS,HIGH); delayMicroseconds(50);
  
  digitalWrite(SS,LOW); delayMicroseconds(50);
//  SPI.transfer(0x0D); delayMicroseconds(50);
  SPI.transfer(0x0E);delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50);
  int ret=99;
  for(i=0;i<300;i++) 
  {
    if (digitalRead(NEVENT)==0) {getStatus();ret=getStatusCode(2);break; }
    else {analogWrite(LED1,10);delay(50);analogWrite(LED1,0);delay(50);} 
  } 
  if (ret==99) sig=13;
  return sig; 
}


int SIGFOX::getStatusCode(byte type)
{
  switch (type)
  {
    case 0 : return ssm;break;
    case 1 : return atm;break;
    case 2 : return sig;break;
  }
}

char* SIGFOX::getStatusAtm()
{
  buffer[0]='\0';
  byte err=(atm&B0011110)>>1;
  char pa[10];pa[0]='\0';
  if (bitRead(atm,0)==1) strcpy(pa,"PA ON");else strcpy(pa,"PA OFF");
  if (err>0) 
     {snprintf((char*)buffer,BLEN,"Error code: %i",err);return (char*)buffer;}
  if (bitRead(atm,5)==1) 
     {snprintf((char*)buffer,BLEN,"Frame sent");return (char*)buffer;} 
  if (bitRead(atm,6)==1)
     {snprintf((char*)buffer,BLEN,"%s . System ready",pa);return (char*)buffer;} 
  snprintf((char*)buffer,BLEN,"%s",pa);
  return (char*)buffer;  
}

String SIGFOX::getStatusSig()
{
  if (sig>sizeof(statusCodes)){
   return "Controller comm. error, invalid status code";
  }
  return statusCodes[sig];
}

void SIGFOX::getStatus()
{
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x0A); delayMicroseconds(50);
  SPI.transfer(0); delayMicroseconds(50);
  ssm=SPI.transfer(0); delayMicroseconds(50);
  atm=SPI.transfer(0); delayMicroseconds(50);
  sig=SPI.transfer(0); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50);
}

unsigned char* SIGFOX::getAtmVersion()
{
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x06); delayMicroseconds(50);
  SPI.transfer(0); delayMicroseconds(50);
  byte mv=SPI.transfer(0); delayMicroseconds(50);
  byte lv=SPI.transfer(0); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50); 
  buffer[0]=mv;
  buffer[1]=lv;
  buffer[2]='\0';
  return buffer;
}

unsigned char* SIGFOX::getSigVersion()
{
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x06); delayMicroseconds(50);
  SPI.transfer(0); delayMicroseconds(50);
  byte mv=SPI.transfer(0); delayMicroseconds(50);
  byte lv=SPI.transfer(0); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50); 
  buffer[0]=mv;
  buffer[1]=lv;
  buffer[2]='\0'; 
  return buffer;
}

unsigned char* SIGFOX::getID()
{
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x12); delayMicroseconds(50);
  SPI.transfer(0); delayMicroseconds(50);
  byte ID3=SPI.transfer(0); delayMicroseconds(50);
  byte ID2=SPI.transfer(0); delayMicroseconds(50);
  byte ID1=SPI.transfer(0); delayMicroseconds(50);
  byte ID0=SPI.transfer(0); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50); 
  buffer[0]=ID3;buffer[1]=ID2;buffer[2]=ID1;buffer[3]=ID0;
  buffer[4]='\0';
  return buffer;  
}

unsigned char* SIGFOX::getPAC()
{
  int i;
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x0F); delayMicroseconds(50);
  SPI.transfer(0); delayMicroseconds(50);
  for(i=0;i<16;i++) {buffer[i]=SPI.transfer(0); delayMicroseconds(50);}
  digitalWrite(SS,HIGH); delayMicroseconds(50); 
  buffer[16]='\0';
  return buffer;  
}

void SIGFOX::reset()
{
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x01); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50); 
}

void SIGFOX::testMode(byte frameL,byte frameH, byte chanL,byte chanH)
{
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x08); delayMicroseconds(50);
  SPI.transfer(frameL); delayMicroseconds(50);
  SPI.transfer(frameH); delayMicroseconds(50);
  SPI.transfer(chanL); delayMicroseconds(50);
  SPI.transfer(chanH); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50);  
}

void SIGFOX::end()
{
  digitalWrite(PWRON,LOW);  
  digitalWrite(SS,LOW); delayMicroseconds(50);
  SPI.transfer(0x05); delayMicroseconds(50);
  digitalWrite(SS,HIGH); delayMicroseconds(50);
}

