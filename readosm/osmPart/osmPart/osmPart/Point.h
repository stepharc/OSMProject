#pragma once
class Point
{
public:
	Point();
	~Point();

	double getLatitude(void);
	void setLatitude(double);

	double getLongitude(void);
	void setLongitude(double);

	double getEle(void);
	void setEle(double);

private:
	double latitude;
	double longitude;
	double ele;
};

