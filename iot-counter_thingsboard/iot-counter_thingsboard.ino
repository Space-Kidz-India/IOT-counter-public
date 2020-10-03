#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "NETWORK_SSID";
const char* password = "NETWORK_PASSWORD";


#define TOKEN "xxxxxxxxxxxxxx" 
char ThingsboardHost[] = "demo.thingsboard.io";


WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;
String payload;

String data = "";
char temp = ' ';
int current_val=0;
int prev_inx = 0;
int parsed_data[10];
String parsed_data_string[10];

void setup() {
Serial.begin(9600);
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("connected to");
  Serial.println(ssid);
  client.setServer( ThingsboardHost, 1883 );
}

void loop() {

if ( !client.connected() ) 
{
    reconnect();
}


  
  //While start
  while (Serial.available() > 0 ) {
    data = Serial.readString();
    Serial.println(data);
    //parser
    for(int i=0; i<data.length(); i++){
      temp = data[i];
      if(temp == ','){
        parsed_data[current_val]=data.substring(prev_inx, i).toInt();
        parsed_data_string[current_val]=data.substring(prev_inx, i);
        current_val++;
        prev_inx = i+1;
      }else if(i==(data.length()-1)){
        parsed_data[current_val]=data.substring(prev_inx, i+1).toInt();
        parsed_data_string[current_val]=data.substring(prev_inx, i+1);
        current_val++;
      }    
    }
    prev_inx = 0;
    current_val=0;
    //end parser
    //data array = parsed_data[]
    for(int i=0; i<10; i++){
    Serial.print("Channel ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(parsed_data[i]);
    }
     payload = "{";
  for(int i=0; i<10; i++){
    
    payload += "\"channel";
    payload += String(i);
      payload += "\":";
    payload += parsed_data[i];
    if(i<9){
    payload += ",";
    }else{
      payload += "}";
    }
     
  }
  char attributes[1000];
  payload.toCharArray( attributes, 1000 ); 
  //<--------------------CHANGE THIS---------------------> 
  client.publish( "v1/devices/me/telemetry",attributes);
  //<---------------------------------------------------->
  Serial.println( attributes );
  }
  //while end
}



void reconnect() {
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( client.connect("Esp8266", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 500 );
    }
  }
}
