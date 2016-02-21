

int ledPin=6;
int vibr_pin=5;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,10);
RF24Network network(radio);

const uint16_t this_node =2;

const uint16_t other_node = 0;

const unsigned long interval = 100; //ms
unsigned long last_sent;
unsigned long packets_sent;
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
  network.begin(90, this_node);
}
void transmit()
{
  network.update();

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

  }
}

void loop(void)
{ Serial.print(SW420());
  if (SW420()>1000)
  transmit();
  
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
