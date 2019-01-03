/****************************
* Din Vasilevsky			*
* din.vasilevsky@gmail.com	* 
* 30.08.2018				*
*****************************/


#define NMEA_MAX_LENGTH 35
#define NMEA_END_CHAR_1 '\n'
#define SPEED 4800
//#define CONFIG SERIAL_8N1
#define TIME 200 //milliseconds 

// "$HCDM,060.0,M*24\r"
String inputString = "$HCHDM,000.0,M*24"; 
//String outputString = "";
bool stringComplete = false;
float heading = 0.0f;

void setup()
{
	Serial.begin(SPEED);
	while (!Serial) 
	{
		; // wait for serial port to connect. Needed for native USB
	}
	
	// reserve N bytes for the inputString:
	//inputString.reserve(80);
}


/* 	   1   2   3 4   5 6
	   |   |   | |   | |
$--HDG,x.x,x.x,a,x.x,a*hh
1) Magnetic Sensor heading in degrees
2) Magnetic Deviation, degrees
3) Magnetic Deviation direction, E = Easterly, W = Westerly
4) Magnetic Variation degrees
5) Magnetic Variation directio
6) Checksum
*/

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
char c_outputSentence[NMEA_MAX_LENGTH];

void loop()
{
	// print the string when a newline arrives:
	if (stringComplete)
	{
		if(inputString.substring(0,7) == "$HCHDM,")
		{
			strcpy(c_outputSentence, "\0");
			
			char c_inputSentence[NMEA_MAX_LENGTH];		
			uint8_t checkSum = 1;			
			
			inputString.toCharArray(c_inputSentence, inputString.length());
			
			// "$HCHDM,058.0,M*24\r\n"
			char c_heading[5];
			for(int i = 0; i < 5; i ++)
				c_heading[i] = c_inputSentence[i + 7];			 
			
			heading = atof(c_heading);			
			
			strcat(c_outputSentence, "$HCHDG,");			
			strcat(c_outputSentence, c_heading);
			strcat(c_outputSentence, ",,W,,W*");
			
			char c_checkSum[2];
			checkSum = nmea_get_checksum(c_outputSentence);			
			String s_checkSum = String(checkSum, HEX);
			s_checkSum.toUpperCase();
			s_checkSum.toCharArray(c_checkSum, s_checkSum.length()+1);
			
			strcat(c_outputSentence, c_checkSum); 
			strcat(c_outputSentence, "\r\n");			
			
			strcpy(c_inputSentence, "\0");	

			inputString = "";			
	
		}
		 
		stringComplete = false;
	}	
		
	if ((millis() - timing) > TIME)
	{
		timing = millis();
		Serial.print(c_outputSentence);
		
		//Serial.print("$GPHDT,123.45,T*32\r\n");
		//Serial.print("$HCHDG,10.10,0.0,W,0.0,W*42\r\n");
		//Serial.print("$GPHDG,20.20,0.0,W,0.0,W*5E\r\n");		
		//Serial.print("$GPHDG,30.30,,W,,W*5E\r\n");		
		//Serial.print("$HCHDG,40.00,,W,,W*42\r\n");		
		//Serial.print("$GPHDT,50.50,M*2C\r\n");		
		//Serial.print("$GPHDG,60.60.20,,W,,W*72\r\n");
		//Serial.print("$GPHDG,70.70.20,,,,*72\r\n");		
	}
}


void serialEvent() 
{
	while (Serial.available() > 0) 
	{
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n') 
		{
			stringComplete = true;
		}
	}
}
