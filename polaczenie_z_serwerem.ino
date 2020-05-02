#include <WiFi.h>
//#include <WebServer.h>
#include "DHT.h"
#include <HTTPClient.h>

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/*Podaj SSID & haslo*/
const char* ssid = "KIAE";  // Enter SSID here
const char* password = "K3N4U5F42X";  //Enter Password here

//linki interakcji z serwerem
const char* serverUstawienia = "https://esp32-terrarium-control.now.sh/getConfig";
const char* serverNameHumi = "http://192.168.4.1/humidity";
const char* serverNamePres = "http://192.168.4.1/pressure";

char* odpowiedz;

// DHT Sensor
uint8_t DHTPin = 4; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;

unsigned long poprzedniCzas = 0;
const long oczekiwanie = 5000; //5000ms
 
void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  dht.begin();              

  Serial.println("Trwa laczenie z ");
  Serial.println(ssid);

  //inicjalizuj polaczenie
  WiFi.begin(ssid, password);

  //wypisuj kropki podczas laczenia
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("o Boi jestem polaczony!");
  Serial.print("a moje IP: ");  Serial.println(WiFi.localIP());

}
void loop() {
  
unsigned long aktualnyCzas = millis();

if(aktualnyCzas - poprzedniCzas >= oczekiwanie) {
//  Sprawdz czy sie polaczyles z WiFi
  if(WiFi.status()==WL_CONNECTED){
//    do dopisania zbierania zmiennych ustawien
//    odpowiedz = httpGETDATA(serverUstawienia);
     String odp = httpGETDATA(serverUstawienia);
     odp = odp.substring(3,odp.length());
     char dupajasiu[odp.length()];
     odp.toCharArray(dupajasiu,odp.length());
     char* Tem = strtok(dupajasiu," :\{\"temp,");
//    char *Tem = strtok(odpowiedz," temp,.");
    while (Tem != NULL){
    Serial.println(Tem);
    Tem = strtok(NULL," :\{\"temp,wilg");
    }
    Serial.println(odpowiedz);


  }
}
delay(1000);
}

String httpGETDATA(const char* linkSerwera) {
  HTTPClient http;

//  Vader the time has come
  http.begin(linkSerwera);

// Do it,... do it now
  int httpOdpowiedz = http.GET();

  String ladunek = "--";

  if (httpOdpowiedz > 0)  {
    Serial.print("Sir! Serwer mowi: ");
//    Serial.print(httpOdpowiedz);
    ladunek=http.getString();
  }
  else {
    Serial.print("Sir! Jest cicho, zbyt cicho: ");
    Serial.print(httpOdpowiedz);
  }
// Vader release him at once
  http.end();

  return ladunek;
}
