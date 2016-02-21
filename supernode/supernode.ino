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
 
 int ledPin=4;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 5;

// Address of the other node
const uint16_t other_node = 4;

//Set threshold for SW420
int threshold=100;

// Structure of our payload
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
  Serial.println("RF24Network/Quaximity_protocol/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop()
{
  // Pump the network regularly
  network.update();
  //Serial.print("Hello");

  // Is there anything ready for us?
  while (network.available())
  {
    Serial.println("Available he bhaaaai");
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_rt payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Received packet: Intensity= ");
    Serial.print(payload.record1);
    Serial.print(' ');
    Serial.print(payload.record2);
    Serial.print(' ');
    Serial.print(payload.id);
    Serial.println();
    if(payload.record1>threshold )
    {
      digitalWrite(ledPin,HIGH);
   //   Serial.println("High intensity recorded!!!!!!!!!");
      delay(1000);
      digitalWrite(ledPin,LOW);
    }
  }
}
/* vim:ai:cin:sts=2 sw=2 ft=cpp*/

