/****************************
* Din Vasilevsky			*
* din.vasilevsky@gmail.com	* 
* 07.09.2018				*
*****************************/
/*

*/
//Массив сегментов для 10 цифр
byte numbers[10] = { B11000000, B11111001, B10100100, B10110000, B10011001,
                     B10010010, B10000010, B11111000, B10000000, B10010000
                   };
//массив для линейки
unsigned int line[16] = {1, 3, 7, 15, 31, 63, 127, 255, 511, 1023,
                         2047, 4095, 8191, 16383, 32767, 65535
                        };

//Пин подключен к ST_CP входу 74HC595
const int latchPin = 5;
//Пин подключен к SH_CP входу 74HC595
const int clockPin = 8;
//Пин подключен к DS входу 74HC595
const int dataPin = 13;
//Пин подключен к светодиоду "минус"
const int Minus = 2;

int analogInPin = 1;     // номер порта к которому подключен потенциометр
unsigned int val = 0;    // переменная для хранения считываемого значения
int minVal = -20;          // мин.и макс. значения индикатора
int maxVal = 20;
  byte znak ;
  byte ledH ;
  byte ledL ;
  byte digitL ;
  byte digitH ;

#define SPEED 4800
#define CONFIG SERIAL_8N1

void setup() {
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(Minus, OUTPUT);
  send595 (0, 0, 0, 255, 255 );
  delay(1000);
  send595 (1, B11111111, B11111111, B10000000, B10000000 );
  delay(1000);
  send595 (0, 0, 0, 255, 255 );
  delay(1000);
  send595 (1, B11111111, B11111111, B10000000, B10000000 );
  delay(1000);
  send595 (0, 0, 0, 255, 255 );
  delay(1000);
  
  
  Serial.begin(SPEED, CONFIG);
	while (!Serial) 
	{
		; // wait for serial port to connect. Needed for native USB
	}
}

void loop() {

  val = analogRead(analogInPin);
  
  //int data = map ( val, 0, 1023, minVal, maxVal );
	
	double yy = (-0.0813*val+37.8881)*10;
	int y = (int)yy;
	int data = y;
  
  if ( data < 0 )  znak = 1;             // определяем знак для цифр
  else znak = 0;
  
  if(data > 99)data = 99;
  if(data < 0 )data = 0;
  data = abs ( data );  
  
  int interval = map ( data, 0, 99, 0, 15 );    //получили текущий интервал
  // разбиваем наши 16 бит на два байта
  // для записи в первый и второй регистр
  ledH = highByte(line[interval]);
  ledL = lowByte(line[interval]);
  digitL = data % 10; //остаток от деления на 10 - единицы
  digitH = data / 10; //целое кол-во десятков
  
   
  if ( digitH == 192 ) digitH = 255 ; //гасим незначащий ноль

	send595( znak, ledH, ledL, digitH, digitL);
	
	
	Serial.print(val);   Serial.print("   ");
	Serial.print(data);   Serial.print("   ");
	Serial.print(digitH); Serial.print(digitL);
	Serial.print("   ");  Serial.println(interval);
	
	
	
	// пауза перед следующей итерацией
  delay(500);
  
}

void send595( byte minus, byte b4, byte b3, byte b2, byte b1 ) {
    if ( minus == 0 )               // определяем знак для цифр
    {digitalWrite(Minus, HIGH);}
  else
    {digitalWrite(Minus, LOW);}
  // устанавливаем синхронизацию "защелки" на LOW
  digitalWrite(latchPin, LOW);
  // передаем 2 байта линейки
  shiftOut(dataPin, clockPin, MSBFIRST, b4);
  shiftOut(dataPin, clockPin, MSBFIRST, b3);
  //передаем 2 байта цифровой шкалы
  shiftOut(dataPin, clockPin, MSBFIRST, b2);
  shiftOut(dataPin, clockPin, MSBFIRST, b1);
  //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  digitalWrite(latchPin, HIGH);

}