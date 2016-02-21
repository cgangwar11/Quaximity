
 int ledPin=4;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(9,10);


RF24Network network(radio);


const uint16_t this_node = 5;

const uint16_t other_node = 4;

int threshold=100;

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
  network.begin( 90, this_node);
}

void loop()
{

  network.update();

  while (network.available())
  {
    Serial.println("Available he bhaaaai");
   
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
  
      delay(1000);
      digitalWrite(ledPin,LOW);
    }
  }
}


