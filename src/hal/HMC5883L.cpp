#include "./apps/tools/compass.h"
#include <math.h>
#include <HMC5883LCompass.h>
#include <gfx_util.h>
#include <osw_app.h>
#include <osw_hal.h>
#include <osw_pins.h>
#include <EEPROM.h>

HMC5883L compass; 												/*** Store our compass as a variable		***/
int error = 0;
MagnetometerScaled axis;

void OswHal::setupHMC5883L(void)
{
    compass = HMC5883L();

	/*** Set scale of the compass ***/
	Serial.println("Setting scale to +/- 1.3 Ga");

  	error = compass.SetScale(1.3);
	if(error != 0) {											/*** If there is an error, print it out 	***/ 
    	Serial.println(compass.GetErrorText(error));
		_hasHMC5883L = false;
	}

	error = compass.SetMeasurementMode(Measurement_Continuous); /*** Set the measurement mode to Continuous ***/ 
	if(error != 0) {											/*** If there is an error, print it out 	***/ 
    	Serial.println(compass.GetErrorText(error));
		_hasHMC5883L = false;
	}

	_hasHMC5883L = true;
}

int OswHal::hmc5883_GetAzimuth(void)
{
	
	float heading = atan2(axis.YAxis, axis.XAxis);

	// Correct for when signs are reversed.
  	if(heading < 0)
    	heading += 2*PI;
    
	// Check for wrap due to addition of declination.
	if(heading > 2*PI)
		heading -= 2*PI;

	// Convert radians to degrees for readability.
	float headingDegrees = heading * 180/M_PI; 	

	return (int) headingDegrees;
}

int OswHal::hmc5883_GetX(void)
{
    return axis.XAxis;
}

int OswHal::hmc5883_GetY(void)
{
    return axis.YAxis;
}

int OswHal::hmc5883_GetZ(void)
{
    return axis.ZAxis;
}

void OswHal::hmc5883_Read(void)
{
    axis = compass.ReadScaledAxis();								/*** Retrived the scaled values from the compass ***/
}

void OswHal::hmc5883_setCalibration(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
{
    //TODO
}

bool OswHal::hasHMC5883L(void) { return _hasHMC5883L; }