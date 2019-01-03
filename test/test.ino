/****************************
* Din Vasilevsky			*
* din.vasilevsky@gmail.com	* 
* 03.09.2018				*
*****************************/


#define NMEA_MAX_LENGTH 82
#define NMEA_END_CHAR_1 '\n'
#define SPEED 4800
#define CONFIG SERIAL_8N1
#define TIME 500 //milliseconds 

// "$HCDM,060.0,M*24\r"
//String inputString = "$HCHDM,058.0,M*24";
//float heading = 0.0f;

void setup()
{
	Serial.begin(SPEED, CONFIG);
	while (!Serial) 
	{
		; // wait for serial port to connect. Needed for native USB
	}
}

uint8_t nmea_get_checksum(char sentence[])
{
	const char *n = sentence + 1;
	uint8_t chk = 0;

	/* While current char isn't '*' or sentence ending (newline) */
	while ('*' != *n && NMEA_END_CHAR_1 != *n && '\0' != *n) {
		chk ^= (uint8_t) *n;
		n++;
	}

	return chk;
}

unsigned long timing = 0;

void loop()
{		
	if ((millis() - timing) > TIME)
	{
		timing = millis();
		Serial.println("$HCHDM,099.9,M*24");
	}
}
