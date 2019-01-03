/****************************
* Din Vasilevsky			*
* din.vasilevsky@gmail.com	* 
* 17.08.2018				*
*****************************/


#define PIN_LO_L 8
#define PIN_LO_R 7
#define PIN_HI_L 9
#define PIN_HI_R 10

#define PIN_L_TO_R 12
#define PIN_R_TO_L 11

#define DELAY_MS 2000

void setup()
{
	pinMode(PIN_LO_L, INPUT_PULLUP);
	pinMode(PIN_LO_R, INPUT_PULLUP);
	pinMode(PIN_HI_L, INPUT_PULLUP);
	pinMode(PIN_HI_R, INPUT_PULLUP);
	
	pinMode(PIN_L_TO_R, OUTPUT);
	pinMode(PIN_R_TO_L, OUTPUT); 
	
	digitalWrite(PIN_L_TO_R, 0);
	digitalWrite(PIN_R_TO_L, 0);
}

unsigned long time_l = 0;
unsigned long time_r = 0;
unsigned int  old_l_to_r = 0;
unsigned int  old_r_to_l = 0;

void out(int l_to_r, int r_to_l)
{	
	if(l_to_r != old_l_to_r)
		time_l = millis();
	
	if( (millis() - time_l) >= DELAY_MS )
		digitalWrite(PIN_L_TO_R, l_to_r);
	
	
	if(r_to_l != old_r_to_l)
		time_r = millis();
	
	if( (millis() - time_r) >= DELAY_MS )
		digitalWrite(PIN_R_TO_L, r_to_l);
	
	old_l_to_r = l_to_r;
	old_r_to_l = r_to_l;
}

int read_input(void)
{
	int temp = 0;
	temp = digitalRead(PIN_HI_R)
		| digitalRead(PIN_HI_L) << 1
		| digitalRead(PIN_LO_R) << 2
		| digitalRead(PIN_LO_L) << 3;
	return temp;
}

void loop()
{
	int input = read_input();

	switch (input)
	{			  // out(PIN_L_TO_R, PIN_R_TO_L)
		case 0b1111: out(1, 1); break; //  0
		case 0b1110: out(0, 1); break; //  1
		case 0b1101: out(0, 1); break; //  2
		case 0b1100: out(0, 0); break; //  3 !!!
		case 0b1011: out(1, 0); break; //  4
		case 0b1010: out(0, 0); break; //  5 !!!
		case 0b1001: out(1, 0); break; //  6
		case 0b1000: out(0, 0); break; //  7 !!!
		case 0b0111: out(0, 1); break; //  8
		case 0b0110: out(0, 1); break; //  9
		case 0b0101: out(0, 0); break; // 10 !!!
		case 0b0100: out(0, 0); break; // 11 !!!
		case 0b0011: out(0, 0); break; // 12
		case 0b0010: out(0, 0); break; // 13 !!!
		case 0b0001: out(1, 0); break; // 14 !!!
		case 0b0000: out(1, 0); break; // 15 !!!
		default: break;
	}	
	
	delay(10);
}
