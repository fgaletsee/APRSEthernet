//Author: Franz G. Aletsee
//Call: DL6FCD K1AFR
//Mail: franz.aletsee@dl6fcd.de
//Date: 08/2015

//Callsign ID password and ARPS-IS server
#define call "DL6FCD"
#define id 7

#define pass 17068
#define server "vancouver.aprs2.net"
#define serverport 14580
//Location
//Format must have same number of digits!
#define NS "4821.20N"
#define EW "01054.00E"
//#define altitude 450 //meter
#define comment "wRSW"

//Check consitency of defines
#if (id > 9)
#define idleng 2
#else
#define idleng 1
#endif


#define buflen 128
char txdata[buflen];

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);
#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xFA, 0x21, 0x06, 0x19, 0x88, 0x10 };
EthernetClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("REBOOT");
  dht.begin();
  Serial.println("DHT11 init");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (1);
  }
}

void loop()
{

  if (client.connect(server, serverport)) {
    Serial.println("connected");

    //Credentials
    chearchar();
    add2char("user ");
    add2char(call);
    add2char("-");
    adduint162buff(id, idleng);
    add2char(" pass ");
    adduint162buff(pass, 5);
    add2char(" vers ArWX V1.0 filter b/dlxxxx-1");
    client.println(txdata);
    Serial.println(txdata);
    delay(1000);
    readServer();
    chearchar();
    //add2char("DL6FCD-7>APRS,TCPIP*: Test Test Test by DL6FCD !");
    //lient.println(txdata);
    //Serial.println(txdata);
    //delay(1000);
    //readServer();

  }
  else {

    Serial.println("connection failed");
  }

  //connected!
  while (client.connected())
  {

    sendPacket();
    uint8_t k=0;
for (k=0;k<50;k++)
{
  delay(800); 
  readServer();
}
}

  

  //Wenn Verbindung abbricht
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

}

void sendPacket()
{
 //Debuging
  uint8_t tmp=100;
  uint16_t pair=10240;
  float f = dht.readTemperature(true); // Read Temp in Fahreneheit
  float h = dht.readHumidity();  //Humidity in %
  chearchar();
  add2char(call);
  add2char("-");
  adduint162buff(id, idleng);
  add2char(">APRS,TCPIP*:!");
  add2char(NS);
  add2char("/");
  add2char(EW);
  add2char("_.../...g...t");

  adduint162buff(f, 3);
  add2char("b");
  adduint162buff(pair, 5);

  add2char("h");
  adduint162buff(h, 2);
  //add2char(comment);
  Serial.println(txdata);
  client.println(txdata);
}


void readServer()
{
  Serial.println("ReadServer");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  }

