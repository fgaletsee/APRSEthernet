//Author: Franz G. Aletsee
//Call: DL6FCD K1AFR
//Mail: franz.aletsee@dl6fcd.de
//Date: 08/2015

//Callsign ID password and ARPS-IS server
#define call "DF0HSA"
#define id 7

#define pass 21677
#define server "vancouver.aprs2.net"
#define serverport 14580
//Location
//Format must have same number of digits!
#define NS "4821.29N"
#define EW "01054.29E"
//#define altitude 450 //meter
#define comment " Temp in shack"

//Check consitency of defines
#if (id > 9)
#define idleng 2
#else
#define idleng 1
#endif


#define buflen 128
char txdata[buflen];

//float f = 0;
//float h = 0;


#include <SFE_BMP180.h>
#include <Wire.h>

// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;
  double T,P;
 #define ALTITUDE 500.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters 
  
#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xFA, 0x21, 0x06, 0x19, 0x88, 0x10 };
EthernetClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("REBOOT");
if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (1);
  }
  doMeas();
}

void loop()
{

  if (client.connect(server, serverport)) {
    Serial.println("connected");

    //Credentials
    chearchar();
    add2char("user ");
    add2char(call);
    //add2char("-");
    //adduint162buff(id, idleng);
    add2char(" pass ");
    adduint162buff(pass, 5);
    add2char(" vers ArduPRS V1.0 filter b/dlxxxx-1");
    client.println(txdata);
    Serial.println(txdata);
    delay(1000);
    readServer();
  chearchar();
  add2char(call);
  add2char(">APRS,TCPIP*:>430.300MHz FM www.hs-augsburg.de/df0hsa");
  Serial.println(txdata);
  client.println(txdata);
  
  }
  else {

    Serial.println("connection failed");
  }

  //connected!
  while (client.connected())
  {

    sendPacket();
    uint16_t k=0;
 
for (k=0;k<1200;k++)
{
  delay(500); 
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
 
doMeas();
T=(9.0/5.0)*T+32.0; // in Farhenheit
P=P*10.0;

  chearchar();
  add2char(call);
  //add2char("-");
  //adduint162buff(id, idleng);
  add2char(">APRS,TCPIP*:!");
  add2char(NS);
  add2char("/");
  add2char(EW);
  add2char("_.../...g...t");

  adduint162buff(T, 3);
  add2char("b");
  adduint162buff(P, 5);

  add2char("h..");

  add2char(comment);
  Serial.println(txdata);
  client.println(txdata);
    delay(500); 
  readServer();
  
  chearchar();
  add2char(call);
  add2char(">APRS,TCPIP*:!");
  add2char(NS);
  add2char("/");
  add2char(EW);
  add2char("rClubstation der Hochschule Augsburg");
  Serial.println(txdata);
  client.println(txdata);
  
  chearchar();
  add2char(call);
  add2char(">APRS,TCPIP*:>430.300MHz FM www.hs-augsburg.de/df0hsa");
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

void doMeas()

{ 
  char status;
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");

      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
           P = pressure.sealevel(P,ALTITUDE); 
            Serial.print("sea pressure: ");
          Serial.print(P,2);


         
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  }
