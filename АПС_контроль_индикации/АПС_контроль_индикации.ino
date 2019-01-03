#include <LiquidCrystal_PCF8574.h>


#include <Wire.h>     // подключаем библиотеку I2C

byte aR = 0;
byte bR = 0;
byte aL = 0;
byte bL = 0;
byte b;
// Определим адреса регистров прав/лев борта
#define R_IN 0x20
#define R_OUT 0x21
#define L_IN 0x22
#define L_OUT 0x23

unsigned int pattern[17] = {
  0b0000000000000000,
  0b0000000000000001,
  0b0000000000000010,
  0b0000000000000100,
  0b0000000000001000,
  0b0000000000010000,
  0b0000000000100000,
  0b0000000001000000,
  0b0000000010000000,
  0b0000000100000000,
  0b0000001000000000,
  0b0000010000000000,
  0b0000100000000000,
  0b0001000000000000,
  0b0010000000000000,
  0b0100000000000000,
  0b1000000000000000
};
LiquidCrystal_PCF8574 lcd(0x27); // присваиваем имя lcd для дисплея 16х2
void setup()
{
//    Serial.begin(9600);
//    Serial.println("Let's go.....");

    Wire.begin(); // wake up I2C bus
// Установим принудительно режимы входов для регистров
// на ввод
    Wire.beginTransmission(R_IN);
    Wire.write(0x00); // IODIRA register
    Wire.write(0xFF); // set all of port A to input
    Wire.endTransmission();
    Wire.beginTransmission(R_IN);
    Wire.write(0x01); // IODIRB register
    Wire.write(0xFF); // set all of port B to input
    Wire.endTransmission();
//
    Wire.beginTransmission(L_IN);
    Wire.write(0x00); // IODIRA register
    Wire.write(0xFF); // set all of port A to input
    Wire.endTransmission();
    Wire.beginTransmission(L_IN);
    Wire.write(0x01); // IODIRB register
    Wire.write(0xFF); // set all of port B to input
    Wire.endTransmission();
//
// на вывод
    Wire.beginTransmission(R_OUT);
    Wire.write(0x00); // IODIRA register
    Wire.write(0x00); // set all of port A to outputs
    Wire.endTransmission();

    Wire.beginTransmission(R_OUT);
    Wire.write(0x01); // IODIRB register
    Wire.write(0x00); // set all of port B to outputs
    Wire.endTransmission();

    Wire.beginTransmission(L_OUT);
    Wire.write(0x00); // IODIRA register
    Wire.write(0x00); // set all of port A to outputs
    Wire.endTransmission();

    Wire.beginTransmission(L_OUT);
    Wire.write(0x01); // IODIRB register
    Wire.write(0x00); // set all of port B to outputs
    Wire.endTransmission();
//
  lcd.begin(16,2); // инициализация LCD дисплея
  lcd.clear();
  //lcd.blink();
  lcd.setBacklight(255); // включение подсветки дисплея
   
  lcd.setCursor(0,0); // ставим курсор на 1 символ первой строки
  lcd.print("Hi, StartNik !"); // печатаем сообщение на первой строке
  
  lcd.setCursor(0,1);  // ставим курсор на 1 символ второй строки
  lcd.print("How are your ?"); // печатаем сообщение на второй строке
   
}
void loop()
{
// чтение банк A регистра правыый борт
Wire.beginTransmission(R_IN); // подключение к чипу
Wire.write(0x12);             // выбираем для работы банк A
Wire.endTransmission();       // отключаемся.
Wire.requestFrom(R_IN, 1);    // запрашиваем один байт
aR=Wire.read();                // чтение состояния портов банка А 
// чтение банк B регистра
Wire.beginTransmission(R_IN); // подключение к чипу
Wire.write(0x13);             // выбираем для работы банк B
Wire.endTransmission();       // отключаемся.
Wire.requestFrom(R_IN, 1);    // запрашиваем один байт
bR=Wire.read();                // чтение состояния портов банка B

//Выыод на индикацию
      Wire.beginTransmission(R_OUT);
      Wire.write(0x12);      // address bank A
      Wire.write(aR);
      Wire.endTransmission();
      Wire.beginTransmission(R_OUT);
      Wire.write(0x13);      // address bank B
      Wire.write(bR);
      Wire.endTransmission();     
      delay(100);
//
// чтение банк A регистра левый борт
Wire.beginTransmission(L_IN); // подключение к чипу
Wire.write(0x12);             // выбираем для работы банк A
Wire.endTransmission();       // отключаемся.
Wire.requestFrom(L_IN, 1);    // запрашиваем один байт
aL=Wire.read();                // чтение состояния портов банка А 
// чтение банк B регистра
Wire.beginTransmission(L_IN); // подключение к чипу
Wire.write(0x13);             // выбираем для работы банк B
Wire.endTransmission();       // отключаемся.
Wire.requestFrom(L_IN, 1);    // запрашиваем один байт
bL=Wire.read();                // чтение состояния портов банка B

//Выыод на индикацию
      Wire.beginTransmission(L_OUT);
      Wire.write(0x12);      // address bank A
      Wire.write(aL);
      Wire.endTransmission();
      Wire.beginTransmission(L_OUT);
      Wire.write(0x13);      // address bank B
      Wire.write(bL);
      Wire.endTransmission();     
      delay(300);
      

/*
  for (int i = 0; i < 17; i++)
  {
      b = highByte(pattern[i])^0b11111111;
      Wire.beginTransmission(0x21);
      Wire.write(0x13);      // address bank B
      Wire.write((byte)b);
      Wire.endTransmission();
      
      b = lowByte(pattern[i])^0b11111111;
      Wire.beginTransmission(0x21);
      Wire.write(0x12);      // address bank A
      Wire.write((byte)b);
      Wire.endTransmission();
      
      delay(300);
  }
*/  
}

