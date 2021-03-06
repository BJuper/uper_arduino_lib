 /**
 * \著作权 
 * @名称：  uper_DHT11.cpp
 * @作者：  uper
 * @版本：  v171213
 * @URL: 	http://www.uper.cc
 * @维护：  uper
 * @时间：  2017/12/13
 *
 * \说明
 * DTH11 温湿度传感器驱动函数 数字口模拟口都可以
 *
 * \公有方法列表
 * 
 * 		1.int DHT11::read()
 * 		2.int DHT11::getHumidity(bool immediately)
 * 		3.int DHT11::getTemperature(bool immediately)
 *
 * \修订历史
 * `<Author>`      `<Time>`        `<Version>`        `<Descr>`
 *  
 * \示例
 *  
 * 		1.uper_dht11.ino
 */
#include "uper_DHT11.h"

UPER_DHT11::UPER_DHT11(int pin)
{
	_pin = pin;
}

 /*Return values:
 *DHTLIB_OK
 *DHTLIB_ERROR_CHECKSUM
 *DHTLIB_ERROR_TIMEOUT
 */
int UPER_DHT11::read()
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delay(18);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(40);
	pinMode(_pin, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(_pin) == LOW)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	loopCnt = 10000;
	while(digitalRead(_pin) == HIGH)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(_pin) == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(_pin) == HIGH)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
        // as bits[1] and bits[3] are allways zero they are omitted in formulas.
	_humidity    = bits[0]; 
	_temperature = bits[2]; 

	uint8_t sum = bits[0] + bits[2];  

	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;
}

int UPER_DHT11::getHumidity(bool immediately)
{
	immediately && read();
	return _humidity;
}

int UPER_DHT11::getTemperature(bool immediately)
{
	immediately && read();
	return _temperature;
}
//
// END OF FILE
//