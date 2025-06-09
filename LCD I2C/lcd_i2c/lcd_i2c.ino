#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//instalujemy bibliotekę
//LiquidCrystal_I2C (autor: Frank de Brabander)

// Adres I2C (najczęściej 0x27 lub 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  lcd.init();
  lcd.backlight(); // Włącz podświetlenie
  lcd.setCursor(0, 0);
  lcd.print("Witaj Arduino!");
  lcd.setCursor(0, 1);
  lcd.print("LCD 2x16 I2C :)");
}


//jesli nie znamy adresu to skanujemy
/*void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Skanowanie I2C...");
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Znaleziono urządzenie na adresie: 0x");
      Serial.println(i, HEX);
    }
  }
}*/

void loop() {
  // Nic nie robimy w pętli
}
