# Códigos referentes ao sistema de medição de corrente elétrica (Project SaIoT)
## Autor: Iran Macedo Bezerra Neto (IranNeto) 

### libraries
```
#include <Time.h>
#include <TimeLib.h>
#include <Ticker.h>
```
Are required to the interruption and to work with time formats

See oficial documentation [here] (https://github.com/esp8266/Arduino).

All the libraries' functions are in the libraries/library_you_are_searching/library.properties.
See allow field.

### How the code works ?

```
void setup()
```
It will set the interruption and do the webSocket connection.

```
void loop()
```
	It works as a sampling routine
	* Take the sampling time
	* Get the sensor's value, square it and accumule
	* Count the number of samples
	* Calcule the load power and send to post it.


```
	sensorValueI = map(sensorValueI, 1, 722, 1, 512);
```
	The current sensor will output a range voltage from 0 to 5 V but the ESP's analogic pin only takes until 3.3 V so it's necessary to map those ranges

	(when there's no current in the sensor it will output 2.5 V and the ADC will show
		3.3V ----- 1023
		2.5V ------- X
		X = 775
		But it should be confirmated by pratical experiments.

		775 is the real zero signal's center where it will oscilate. Map this value to a virtual zero simulating an 5V ADC (to 2.5 V = 512).

		Then, to do the rms calculations it's necessary to center the signal in a zero x-axis correcting the offset.  

	)

  	Calculate the rms value in the sampling time and post the loadPower.

  	:sunglasses: