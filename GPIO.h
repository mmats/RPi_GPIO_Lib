#ifndef GPIO_H_
#define GPIO_H_

#include <iosfwd>
#include <ctime>
#include <chrono>

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
	GPIO( int port, enum dir=OUT, double debTime=0.0 );
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

	void setDebouncingTime( double debTime );
	/**
	 * sets the time the output has to be
	 * stable to cause a change
	 */

	bit getDebouncedValue();
	/**
	 * gets the GPIOx value,
	 * but only if it was stable for
	 * a certain amount of time
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
	bit previousValue;
	double debouncingTime;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_time;
	bool debouncingTimeStarted;
};

#endif /* GPIO_H_ */
