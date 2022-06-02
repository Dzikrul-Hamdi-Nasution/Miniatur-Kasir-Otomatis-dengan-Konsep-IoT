#include <LiquidCrystal_I2C.h> // Library modul I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Servo.h>
Servo portal;
#include<SoftwareSerial.h>
SoftwareSerial scaner_qr(3, 2);
String kode;
int baca_nodemcu;

int conveyor = 5, obstacle = 6, verifikasi = A0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  scaner_qr.begin(9600);
  lcd.backlight();
  lcd.init();
  portal.attach(7);
  lcd.setCursor(5, 0);
  lcd.print("KASIR");
  lcd.setCursor(3, 1);
  lcd.print("OTOMATIS");
  pinMode(conveyor, OUTPUT);
  pinMode(obstacle, INPUT);

  analogWrite(conveyor, 0);
  portal.write(140);
  //Serial.println("Memulai Scanner");
  delay(2000);
  lcd.clear();

}

int  jumlah, total_item, kunci;

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  lcd.print("Harga : ");
  lcd.setCursor(7, 0);
  lcd.print(jumlah);

  lcd.setCursor(0, 1);
  lcd.print("Total : ");
  lcd.setCursor(7, 1);
  lcd.print(total_item);
  cek_benda();

  baca_nodemcu = analogRead(verifikasi);

  if (baca_nodemcu > 600) {
    portal.write(70);
  }
  if (baca_nodemcu < 500) {
    lcd.clear();
    portal.write(140);
    total_item = 0;
    jumlah = 0;
  }



}


void cek_benda() {
  if (digitalRead(obstacle) == HIGH) {
    analogWrite(conveyor, 58);
    kunci = 0;
  }
  else {
    analogWrite(conveyor, 0);
    if (scaner_qr.available() > 0) {
      analogWrite(conveyor, 120);
      kode = scaner_qr.readString();
      //Serial.println(kode);
      
      total_item = total_item + 1;
      delay(1000);

      if (kunci == 0) {
        jumlah = jumlah + kode.toInt();
        //Serial.println(jumlah);
        kirim();
        kode = "";
      }
      kunci = 1;
    }
  }

}


void kirim() {
  Serial.print("*");
  Serial.print(jumlah);
  Serial.print(",");
  Serial.println("#");
}
