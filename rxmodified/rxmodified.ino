/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
*/
const int sensitivity=3;
const unsigned long interval = 100; //ms
long intense1;
long intense2;
// When did we last send?
unsigned long last_sent;
 int ledPin=2;
  int  count1;
  int count2;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;
const uint16_t super_node = 5;
//Set threshold for SW420
int threshold=1000;

// Structure of our payload
struct payload_t
{
  long record;
  
  unsigned int id;
};
struct payload_rt
{
  long record1;
  long record2;
  
  unsigned int id;
};

void setup(void)
{
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  Serial.begin(57600);
 // Serial.println("RF24Network/Quaximity_protocol/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void transmitsuper(long intense1,long intense2)
{
  
 RF24NetworkHeader header1(super_node);
   // RF24NetworkHeader header(/*to node*/ other_node);
   
//  bool ok = network.write(header1,&payload,sizeof(payload));
    network.update();
    payload_rt payload1;
    payload1.id=4;
    payload1.record1=intense1;
    payload1.record2=intense2;
  //  Serial.print(payload1.record1);
   // Serial.print (payload1.record2);
    
    bool ok = network.write(header1,&payload1,sizeof(payload1));
    //  if (ok)
    //  Serial.println("ok.");
//      else
    //  Serial.print("failed");
}
void logger(unsigned int id,long intense)
{
  //Serial.println(id);
  if (id==1)
  {count1++;
  intense1=intense;}
  else if (id==2)
  {count2++;
   intense2=intense;
   }
 //transmitsuper(intense1,intense2);
  
//  Serial.println(count1);
//  Serial.println(count2);
  if (count1>sensitivity && count2>sensitivity)
  {//Serial.print("earthquake detected");
   count1=0;
   count2=0;
   digitalWrite(ledPin,HIGH);
   transmitsuper(intense1,intense2);
   delay(1000);
   digitalWrite(ledPin,LOW);
   
  }
  if (count1>9)
  {
   count1=0;
   
    
   }
   if (count2>9)
   {count2=0;}
  
  
  
}
void loop(void)
{
  // Pump the network regularly
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
 // Serial.print("Received packet: Intensity= ");
   Serial.print(payload.record);
    Serial.print(' ');
   Serial.print(payload.id);
   Serial.println();
    if(payload.record>threshold)
    {
    //  digitalWrite(ledPin,HIGH);
   //   Serial.println("High intensity recorded!!!!!!!!!");
      logger(payload.id,payload.record);//delay(1000);
    //  digitalWrite(ledPin,LOW);
    }
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
