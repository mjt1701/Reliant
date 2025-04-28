int LED1 = 12;
int LED2 = 13;
int button = 6;

boolean LED1State = false;
boolean LED2State = false;

long buttonTimer = 0;
long longPressTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(button, INPUT);

}

void loop() {

	if (digitalRead(button) == HIGH) {      //Button pressed

		if (buttonActive == false) {    //false means the program is detecting the button being pressed for the first time

			buttonActive = true;
			buttonTimer = millis();

		}

		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {    //test how long we have held the button

			longPressActive = true;
			LED1State = !LED1State;
			digitalWrite(LED1, LED1State);

		}

	} else {                             //Button not pressed

		if (buttonActive == true) {          //  first time thru after button released

			if (longPressActive == true) {     //  long press completed

        // for reliant: start over 
				longPressActive = false;

			} else {                          //  then it was a short click
        
        // for reliant: continue as normal
				LED2State = !LED2State;
				digitalWrite(LED2, LED2State);

			}

			buttonActive = false;

		}

	}

}