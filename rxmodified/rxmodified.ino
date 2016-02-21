
const int sensitivity=3;
const unsigned long interval = 100; //ms
long intense1;
long intense2;

unsigned long last_sent;
 int ledPin=2;
  int  count1;
  int count2;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(9,10);

RF24Network network(radio);


const uint16_t this_node = 0;


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
//Structure of transmission to super node
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
  network.begin(90,this_node);
}

void transmitsuper(long intense1,long intense2)
{
  
 RF24NetworkHeader header1(super_node);

    network.update();
    payload_rt payload1;
    payload1.id=4;
    payload1.record1=intense1;
    payload1.record2=intense2;

    
    bool ok = network.write(header1,&payload1,sizeof(payload1));

}
void logger(unsigned int id,long intense)
{

  if (id==1)
  {count1++;
  intense1=intense;}
  else if (id==2)
  {count2++;
   intense2=intense;
   }

  if (count1>sensitivity && count2>sensitivity)
  {
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
  
  network.update();

  while ( network.available() )
  {
 
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));

   Serial.print(payload.record);
    Serial.print(' ');
   Serial.print(payload.id);
   Serial.println();
    if(payload.record>threshold)
    {

      logger(payload.id,payload.record);
    }
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
