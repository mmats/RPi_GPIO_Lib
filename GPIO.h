/*
 * GPIO.h
 *
 *  Created on: 28.08.2013
 *      Author: mats
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <iosfwd>

enum bit{
	SPACE = 0,
	MARK  = 1
};

enum dir{
	IN,
	OUT
};

class GPIO
{
public:
	GPIO( int port, enum dir=OUT );
	~GPIO();

	void setDirection( dir d );
	/**
	 * sets the direction (input or output
	 *
	 * Linux command is:
	 * echo "out" > /sys/class/gpio/gpio4/direction
	 */

	void setValue( bit val );
	void setValue( int i );
	/**
	 * sets a GPIOx to 1 or 0
	 *
	 * Linux command is:
	 * echo "1" > /sys/class/gpio/gpio4/value
	 */

	bit getValue();
	/**
	 * gets the GPIOx value
	 *
	 * Linux command is:
	 * cat /sys/class/gpio/gpio4/value
	 */

private:
	void openPort() const;
	/**
	 * opens GPIOx
	 *
	 * Linux command is:
	 * echo "4" > /sys/class/gpio/export
	 */

	void closePort() const;
	/**
	 * closes GPIOx
	 *
	 * Linux command is:
	 * echo "4" > /sys/class/gpio/unexport
	 */

	std::string toString(const int &i);
	/**
	 * convertes an int to a string
	 */

//	int fromString(const std::string &s);
	/**
	 * convertes a string to an int
	 */

	int ioport;
	dir direction;
	bit value;
};

#endif /* GPIO_H_ */
