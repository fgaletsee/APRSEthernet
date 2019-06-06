//Author: Franz G. Aletsee
//Call: DL6FCD K1AFR
//Mail: franz.aletsee@dl6fcd.de
//Date: 08/2015

//Callsign
#define call "DL6FCD"
#define server "vancouver.aprs2.net"
#define serverport 14580
//Location
//Format must have same number of digits!
#define NS "4817.96N"
#define EW "01054.70E"



//Do not edit -Begin-
#define buflen 128
char txdata[buflen];
char *callid = "123456789";
//Do not edit -End-


#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xFA, 0x21, 0x06, 0x19, 0x88, 0x10 };
EthernetClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("REBOOT");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (1);
  }

}

void loop()
{

  if (client.connect(server, serverport)) {
    connect2Server(server, serverport, call, 5);
    Serial.println("TCP/IP connected");
    chearchar();
    add2buff("filter b/dl6fcd");
    Serial.println(txdata);
    client.println(txdata);
    Serial.println("Filter set!");
  }
  else {
    Serial.println("TCP/IP connection failed");
  }

  while (client.connected())
  {
    //do this every 10 minutes!
    sendPacket();
    uint16_t k = 0;
    for (k = 0; k < 1200; k++)
    {
      delay(500);
      readServer();
    }
  }
}



void sendPacket()
{
  sendItem("FranzT", "4817.30N", EW, 'K', '/', "Test1234");
  delay(1000);
  sendCall("DL6FCD-1", NS, EW, 'P', '/', "Test1234");
}


void readServer()
{
  Serial.println("ReadServer");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    if (c == '\n')
    {
      processPacket();
      chearchar();
    }
    else
    {
      char2buff(c);
    }

  }
}

void processPacket()
{
  char *p;
  p = strstr(txdata, ":DL6FCD   :F");
  if (p != NULL)
  {
    Serial.println("Event ausgeloesst");
    Serial.println(*(p + 12));
    if (*(p + 12) == '1')
    {
      digitalWrite(13, HIGH);
      Serial.println("LED an");
    }
    else
    {
      digitalWrite(13, LOW);
      Serial.println("LED aus");
    }
  }
}
