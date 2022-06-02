#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>


#define FIREBASE_HOST "kasir-otomatis-default-rtdb.firebaseio.com"
#define WIFI_SSID "Server Project"
#define WIFI_PASSWORD "Master75wew"
String dataIn;
String dt[10];
int i, verifikasi;
boolean parsing = false;
int kirim_perintah = 5;

void setup() {

  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST);
  dataIn = "";
  pinMode(kirim_perintah, OUTPUT);


}


void loop() {

  if (Serial.available() > 0)
  {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '\n') {
      parsing = true;
    }
  }
  if (parsing)
  {
    parsingData();
    parsing = false;
    dataIn = "";
  }
  verifikasi = Firebase.getString("verifikasi").toInt();
  if (verifikasi == 1) {
    digitalWrite(kirim_perintah, HIGH);
  }
  if (verifikasi == 0) {
    digitalWrite(kirim_perintah, LOW);
    delay(5000);
    Firebase.setString("verifikasi", "1");
  }
}



void parsingData()
{
  int j = 0;
  //kirim data yang telah diterima sebelumnya

  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 1; i < dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
    }
  }
  //kirim data hasil parsing
  Firebase.setString("bayar", dt[0]);


}


