

void initializeLEDstrips()
{

	shieldLEDstrip.begin();
	shipLEDstrip.begin();

	shieldLEDstrip.setBrightness(shieldMaxBright);
	shieldLEDstrip.fill(shieldLEDstrip.Color(shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue), 0, shieldLEDnum);
	shieldLEDstrip.show();
	delay(1000);

	shieldLEDstrip.fill(shieldLEDstrip.Color(0, 0, 0, 0), 0, shieldLEDnum);
	shieldLEDstrip.show();

	shipLEDstrip.setBrightness(shipMaxBright);
	shipLEDstrip.fill(shipLEDstrip.Color(shipRedValue, shipGreenValue, shipBlueValue, shipWhiteValue), 0, shipLEDnum);
	shipLEDstrip.show();
	delay(1000);

	shipLEDstrip.fill(shipLEDstrip.Color(0, 0, 0, 0), 0, shipLEDnum);
	shipLEDstrip.show();
}