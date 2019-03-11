#include "Point.h"



Point::Point()
{
}


Point::~Point()
{
}

// latitude
double Point::getLatitude(void)
{
	return latitude;
}

void Point::setLatitude(double latitude)
{
	this->latitude = latitude;
}


// longitude
double Point::getLongitude(void)
{
	return longitude;
}

void Point::setLongitude(double longitude)
{
	this->longitude = longitude;
}

// Ele
double Point::getEle(void)
{
	return ele;
}

void Point::setEle(double ele)
{
	this->ele = ele;
}