#include "GPIO.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>

GPIO::GPIO( int port, dir_t dir, double debTime ):
	ioport(port), direction(dir), debouncingTime(debTime)
{
	closePort();
	openPort();
	setDirection(direction);
	value = MARK;
	previousValue = value;
	debouncingTimeStarted = false;
}

GPIO::~GPIO()
{
	if( direction==OUT )
		setValue( SPACE );
	closePort();
}

void GPIO::openPort() const
{
	std::fstream fs;
	fs.open ("/sys/class/gpio/export", std::fstream::out | std::fstream::app);
	fs << ioport;
	fs.close();
}

void GPIO::closePort() const
{
	std::fstream fs;
	fs.open ("/sys/class/gpio/unexport", std::fstream::out | std::fstream::app);
	fs << ioport;
	fs.close();
}

void GPIO::setDirection( dir_t d )
{
	direction = d;
	std::string sFile = "/sys/class/gpio/gpio" + toString(ioport) + "/direction";
	const char * c = sFile.c_str();
	std::fstream fs;
	fs.open (c, std::fstream::out | std::fstream::app);
	if( d == IN )
		fs << "in";
	else
		fs << "out";
	fs.close();
}

void GPIO::setValue( int i )
{
	if( direction == OUT )
	{
		if( i==1 )
			setValue( MARK );
		else
			setValue( SPACE );
	}
}

void GPIO::setValue( bit_t val )
{
	value = val;
	std::string sFile = "/sys/class/gpio/gpio" + toString(ioport) + "/value";
	const char * c = sFile.c_str();
	std::fstream fs;
	fs.open (c, std::fstream::out | std::fstream::app);
	if( val == MARK )
		fs << "1";
	else
		fs << "0";
	fs.close();
}

bit_t GPIO::getValue()
{
	if( direction == IN)
	{
		std::string sFile = "/sys/class/gpio/gpio" + toString(ioport) + "/value";
		const char * c = sFile.c_str();
		std::fstream fs;
		fs.open (c, std::fstream::in | std::fstream::app);
		int val;
		fs >> val;
		fs.close();
		if( val == 1 )
			value = MARK;
		else
			value = SPACE;
	}
	return value;
}

bit_t GPIO::getDebouncedValue()
{
	bit_t actualValue = getValue();
	if( actualValue != previousValue )
	{
		if( !debouncingTimeStarted )
		{
			start = std::chrono::system_clock::now();
			debouncingTimeStarted = true;
		}
		else
		{
			end = std::chrono::system_clock::now();
		}
		elapsed_time = end-start;
		if( elapsed_time.count() >= debouncingTime )
		{
			previousValue = actualValue;
		}
	}
	else
	{
		debouncingTimeStarted = false;
	}
	return previousValue;
}

void GPIO::setDebouncingTime(double debTime)
{
	if( debTime < 0.0 )
		debouncingTime = 0.0;
	else
		debouncingTime = debTime;
}

std::string GPIO::toString(const int &i)
{
	std::ostringstream stream;
	stream << i;
	return stream.str();
}

/*
int GPIO::fromString(const std::string &s)
{
	std::istringstream stream (s);
	int t;
	stream >> t;
	return t;
}
*/
