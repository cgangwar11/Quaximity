/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

int ledPin=6;
int vibr_pin=5;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
//#include <String.h>


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node =2;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 100; //ms

// When did we last send?
unsigned long last_sent;

// How many have we sent already
unsigned long packets_sent;

// Structure of our payload
struct payload_t
{
  payload_t(long rec)
  {
    record=rec;
  }
 long record; 
 unsigned int id;
};

long SW420()
{
  long measurement=pulseIn(vibr_pin,HIGH,1000000);
  return measurement;
}


void setup(void)
{
  pinMode(ledPin,OUTPUT);
  pinMode(vibr_pin,INPUT);
  digitalWrite(ledPin,LOW);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(3,1);
  digitalWrite(4,0);
  
  Serial.begin(57600);
  Serial.println("RF24Network/Quaximity_protocol/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}
void transmit()
{
    // Pump the network regularly
  network.update();

  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    long rec=SW420();
    payload_t payload(rec);
    payload.id=2;
    RF24NetworkHeader header(/*to node*/ other_node);
    
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
      Serial.print(rec);
  /*  if(rec>1000)
     { //this_node++;
       digitalWrite(ledPin,HIGH);
    //   delay(1000);
       digitalWrite(ledPin,LOW);
     }*/
  }
}

void loop(void)
{ Serial.print(SW420());
  if (SW420()>1000)
  transmit();// Pump the network regularly
  
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
