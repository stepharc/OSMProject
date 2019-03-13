#pragma once

/*!
 * \file Point.h
 * \brief Class representing a point
 * \author Ludovic
 */

 /*! \class Point
 * \brief class representing a point, with latitude, longitude, and elevation
 */
class Point
{
public:
	/*!
	*  \brief Constructor
	*
	*  Constructor of the class Point.
	*/
	Point();
	
	/*!
	*  \brief Destructor
	*
	*  Destructor of the class Point.
	*/
	~Point();

	/*!
	*  \brief Get the latitude of the Point
	*
	*  \return The latitude of the Point
	*/
	double getLatitude(void);

	/*!
	*  \brief Set the latitude of the Point
	*
	*  \param The latitude of the Point
	*/
	void setLatitude(double);

	/*!
	*  \brief Get the longitude of the Point
	*
	*  \return The longitude of the Point
	*/
	double getLongitude(void);

	/*!
	*  \brief Set the longitude of the Point
	*
	*  \param  The longitude of the Point
	*/
	void setLongitude(double);

	/*!
	*  \brief Get the "ele" of the Point (the elevation in meters)
	*
	*  \return The "ele" of the Point (the elevation in meters)
	*/
	double getEle(void);

	/*!
	*  \brief Set the "ele" of the Point (the elevation in meters)
	*
	*  \return The "ele" of the Point (the elevation in meters)
	*/
	void setEle(double);

private:
	double latitude; /*!< geographic latitude */
	double longitude; /*!< geographic longitude */
	double ele; /*!< ele of the point, if ele equals 0, then ele is not defined in the osm file */
};

