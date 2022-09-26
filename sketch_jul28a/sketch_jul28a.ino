#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4
#define RST_PIN D2

#define AGENT_ID "526A"
#define SERVER_NOT_CONNECTED "3"
#define BUZZER_PIN D1

const char* ssid = "Mahathir";         // Enter the SSID of your WiFi Network. *******************
const char* password = "12345679";    // Enter the Password of your WiFi Network. ********************
char server[] = "192.168.43.161";    // write SMTP server address ******************
int port = 11500;
char data[100];

MFRC522 mfrc522(SS_PIN, RST_PIN);

WiFiClient Client;              //define wifi client as client

void setup() {  
    delay(1000);
    Serial.begin(9600); 
    
    WiFi.begin(ssid, password);     // connect to WIFI
  
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }
     SPI.begin();       // Init SPI bus
     mfrc522.PCD_Init(); // Init MFRC522
     Serial.begin(9600);
     Serial.flush();

     pinMode(BUZZER_PIN,OUTPUT);
     digitalWrite(BUZZER_PIN,LOW);
}

void loop() {
  int t,i;
  
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
        digitalWrite(BUZZER_PIN,HIGH);
        String RFID_TAG="";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
              RFID_TAG+=(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
              RFID_TAG+=mfrc522.uid.uidByte[i];
        }

        if(Client.connect(server, port) ==1){
              Client.println(AGENT_ID);
              Client.println(RFID_TAG);
              Client.flush();
              delay(20);

              digitalWrite(BUZZER_PIN,LOW);
              
              while(!Client.available());

              i=0;
              while(Client.available()){
                data[i]=Client.read();
                i++;
                //Serial.print((char)t);
              }
              data[i]='\0';
              Serial.println(data);
              Serial.flush();
              delay(50);

              while(!Serial.available());

              delay(50);

              i=0;
              while(Serial.available()){
                data[i]=Serial.read();
                i++;
                //Serial.print((char)t);
              }
              data[i]='\0';
              Client.println(data);
              Client.flush();
        }
        else{
          String nullData=SERVER_NOT_CONNECTED;
          nullData+=" null null 0";
          Serial.println(nullData); 
          Serial.flush();
          delay(50);
        }
        mfrc522.PICC_HaltA();
        delay(100);

        digitalWrite(BUZZER_PIN,LOW);
  }
}
