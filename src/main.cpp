#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include <Time.h>
#include <TimeLib.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>



#ifndef STASSID
#define STASSID "Seezeit-MAC-Auth"
#define STAPSK  "seezeit-mac-auth"
#endif
const long utcOffsetInSeconds = 3600;
const char* ssid     = STASSID;
const char* password = STAPSK;
int currentDayOfMonth;
int currentHour;
Adafruit_NeoPixel chain(50, 13, NEO_RGB + NEO_KHZ800);
int bright = 10;

time_t utcCalc;
const char* host = "djxmmx.net";
const uint16_t port = 17;

unsigned long NTPstamp;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void dimmUp(int target){
	for (int i = chain.getBrightness(); i < target; i++){
		chain.setBrightness(i);
	}
	return;
}

void dimmDwn(int target){
	for (int i = chain.getBrightness(); i > target; i--){
			chain.setBrightness(i);
		}
		return;
}

void setup() {
	Serial.begin(9600);
	chain.begin();
	chain.setBrightness(10);
	int i = 0;
	while (i < 50){
		chain.setPixelColor(i, 0, 0, 0);
		i++;
	}
	chain.show();

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	}
void loop() {
	timeClient.update();
	NTPstamp = timeClient.getEpochTime();
	utcCalc = NTPstamp;
	Serial.println(minute(utcCalc));
	delay(2000);
	currentDayOfMonth = day(utcCalc);
	currentHour = hour(utcCalc);


	//Block for month handling
	chain.clear();
	switch (currentDayOfMonth){
	case 31:
		chain.setPixelColor(30, 255, 0, 0);
		/* no break */

	case 30:
		chain.setPixelColor(29, 100, 0, 255);
		/* no break */

	case 29:
		chain.setPixelColor(28, 0, 200, 0);
		/* no break */

	case 28:
		chain.setPixelColor(27, 255, 0, 25);
		/* no break */

	case 27:
		chain.setPixelColor(26, 0, 152, 200);
		/* no break */

	case 26:
	   	chain.setPixelColor(25, 255, 150, 0);
	   		/* no break */

	case 25:
		chain.setPixelColor(24, 125, 0, 125);
		/* no break */

	case 24:
    	chain.setPixelColor(23, 255, 0, 0);
    	/* no break */

	case 23:
    	chain.setPixelColor(22, 255, 125, 0);
    	/* no break */

	case 22:
    	chain.setPixelColor(21, 0, 255, 0);
    	/* no break */

	case 21:
    	chain.setPixelColor(20, 0, 0, 255);
    	/* no break */

	case 20:
    	chain.setPixelColor(19, 255, 0, 200);
    	/* no break */

	case 19:
    	chain.setPixelColor(18, 255, 0, 0);
    	/* no break */

	case 18:
    	chain.setPixelColor(17, 255, 100, 0);
    	/* no break */

	case 17:
    	chain.setPixelColor(16, 0, 255, 0);
    	/* no break */

	case 16:
    	chain.setPixelColor(15, 0, 0, 255);
    	/* no break */

	case 15:
    	chain.setPixelColor(14, 255, 0, 0);
    	/* no break */

	case 14:
    	chain.setPixelColor(13, 255, 0, 100);
    	/* no break */

	case 13:
    	chain.setPixelColor(12, 255, 100, 0);
    	/* no break */

	case 12:
    	chain.setPixelColor(11, 0, 255, 0);
    	/* no break */

	case 11:
    	chain.setPixelColor(10, 0, 0, 255);
    	/* no break */

	case 10:
    	chain.setPixelColor(9, 255, 0, 200);
    	/* no break */

	case 9:
    	chain.setPixelColor(8, 255, 0, 0);
    	/* no break */

	case 8:
    	chain.setPixelColor(7, 255, 100, 0);
    	/* no break */

	case 7:
    	chain.setPixelColor(6, 20, 0, 255);
    	/* no break */

	case 6:
    	chain.setPixelColor(5, 0, 255, 0);
    	/* no break */

	case 5:
    	chain.setPixelColor(4, 255, 0, 255);
    	/* no break */

	case 4:
    	chain.setPixelColor(3, 100, 20, 0);
    	/* no break */

	case 3:
    	chain.setPixelColor(2, 0, 0, 255);
    	/* no break */

	case 2:
    	chain.setPixelColor(1, 0, 255, 0);
    	/* no break */

	case 1:
		chain.setPixelColor(0, 255, 0, 0);
    	chain.show();
	}



	//hour handle block
	if(currentHour > 12){//12h system
		currentHour = currentHour - 12;
	}

	for (int i = 0; i <= currentHour; ++i) {
		chain.setPixelColor(49-i, 0, 255, 255);
	}
	for (int i = currentHour+1; i < 37; --i) {
		chain.setPixelColor(i, 0, 0, 0);
	}
	chain.show();

	//dimming block
	if((hour(utcCalc) >= 22 || hour(utcCalc) <= 8)){
		if(bright > 10){
			dimmDwn(5);
			bright = 5;
			chain.show();
		}
	}
	else if(bright <= 5){
		dimmUp(225);
		bright = 225;
		chain.show();
	}
}
