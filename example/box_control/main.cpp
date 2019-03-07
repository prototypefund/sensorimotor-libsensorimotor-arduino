#include <LiquidCrystal_I2C.h>
#include <sensorimotor.hpp>

// potentiometer values
volatile int values[4] = {0, 0, 0, 0};
// LCD display
LiquidCrystal_I2C lcd(0x27, 20, 4);

Motorcord motors;

// position vars
float m0pos = 0.0;
float m1pos = 0.0;
float m2pos = 0.0;
float m3pos = 0.0;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();

  pinMode(A3, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

float to_percent(int raw) {
  return (raw / 1024.0) * 100.0;
}

void read_potis() {
  values[0] = analogRead(A3);
  values[1] = analogRead(A0);
  values[2] = analogRead(A1);
  values[3] = analogRead(A2);
}

void draw_lcd() {
  lcd.setCursor(0, 0);
  lcd.print("  M1  M2  M3  M4    ");
  lcd.print("%  0   0   0   0    ");
  lcd.print("P 0.0 0.0 0.0 0.0   ");
  lcd.print("                    ");

  lcd.setCursor(2, 3);
  lcd.print(to_percent(values[0]), 0);
  lcd.setCursor(6, 3);
  lcd.print(to_percent(values[1]), 0);
  lcd.setCursor(10, 3);
  lcd.print(to_percent(values[2]), 0);
  lcd.setCursor(14, 3);
  lcd.print(to_percent(values[3]), 0);
}

void loop() {
  read_potis();
  motors.apply(); // perform communication and control
  draw_lcd();
}
