/*
 * CHANGE ADDRESS!
 * Change the device address, network (session) key, and app (session) key to the values
 * that are registered via the TTN dashboard.
 * The appropriate line is "myLora.initABP(XXX);" or "myLora.initOTAA(XXX);"
*/
#include <rn2xx3.h>


const char *appEui = "0000000000000000";
const char *appKey = "279691821C3B2B85BE307B21E081DB2D";



rn2xx3 myLora(Serial2);
double last_send_time = 0;
double interval = 60000; // one minute
//double interval = 60000 * 5; // 5 minutes
//double interval = 60000 * 10;// 10 minutes


// the setup routine runs once when you press reset:
void setup()
{
  //output LED pin
  pinMode(3, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(57600); //serial port to computer
  Serial2.begin(57600); //serial port to radio
  Serial.println("Startup");

  initialize_radio();

  //transmit a startup message
  //myLora.tx("TTN Mapper on TTN Enschede node");

  led_off();
  delay(2000);
}

void initialize_radio()
{
  //reset rn2483
  //was pin 18
  //while(!Serial){}
  
  Serial.println("resetting lora");
  pinMode(PA10, OUTPUT);
  digitalWrite(PA10, LOW);
  delay(1000);
  digitalWrite(PA10, HIGH);
  Serial.println("done resetting lora");
  // ingestelde appKey en joinEUI
  Serial.print("appKey: ");
  Serial.println(appKey);
  Serial.print("joinEUI: ");
  Serial.println(appEui);
  delay(100); //wait for the RN2xx3's startup message
  Serial2.flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  myLora.autobaud();

  //check communication with radio
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;


  /*
   * ABP: initABP(String addr, String AppSKey, String NwkSKey);
   * Paste the example code from the TTN console here:
   */
  //const char *devAddr = "260B6795";
  //const char *nwkSKey = "DA2BEB58A9FA3DABAB56BE98FA090921";
  //const char *appSKey = "039AC75D6FDCC9F96FAC7F01D210BFB5";

  //join_result = myLora.initABP(devAddr, appSKey, nwkSKey);

  /*
   * OTAA: initOTAA(String AppEUI, String AppKey);
   * If you are using OTAA, paste the example code from the TTN console here:
   */
   //const char *appEui = "0000000000000000";
   //const char *appKey = "5E7773DF01C66243843429D3B38C5FCB";


  join_result = myLora.initOTAA(appEui, appKey);

  // Loopt vast bij OTAA, uncomment voor ABP
  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(30000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");
  
}

// the loop routine runs over and over again forever:
void loop(){

  if (millis() - last_send_time >= interval)
  {
    last_send_time = millis();
    led_on();
    Serial.println("TXing");
    double start = millis();
    //data = "ABCD_ABCD_ABCD_ABCD_ABCD_ABCD_ABCD_ABCD"
    byte data[5] = {0x0, 0x17,  0x4, 0x4C, 0x50};

    myLora.txBytes(data, 5); 
    double transmission = millis() - start;
    Serial.println(transmission);

    led_off();
    myLora.sleep(interval - 6000);
    myLora.autobaud();
  } 

}

void led_on()
{
  digitalWrite(3, 1);
}

void led_off()
{
  digitalWrite(3, 0);
}
