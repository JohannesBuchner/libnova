/* This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

Copyright (C) 2000 Liam Girdwood <liam@nova-ioe.org>
*/

/*! \mainpage libnova Astronomical Calculation Library
* 
* \section intro Introduction
* libnova is a general purpose, double precision, astronomical calculation library.
*
* The intended audience of libnova is C / C++ programmers, astronomers and anyone else interested in calculating positions of astronomical objects.
* libnova is the calculation engine used by the <A href="http://nova.sf.net">Nova</A> project and most importantly, is free software.
*
* \section features Features
* The current version of libnova can calculate:
*
* - Aberration
* - Nutation
* - Apparent Position
* - Dynamical Time
* - Julian Day
* - Precession
* - Proper Motion
* - Sidereal Time
* - Solar Coordinates
* - Coordinate Transformations
* - Planetary Positions (Mercury - Neptune using VSOP87)
* - Planetary Magnitude, illuminated disk and phase angle.
* - Lunar Position, phase.
*
* \section docs Documentation
* API documentation for libnova is included in the source. It can also be found in this website and an offline tarball is available <A href="http://libnova.sf.net/libnovadocs.tar.gz">here</A>.
*
* \section download Download
* The latest version of libnova is available for download <A href="http://sf.net/project/showfiles.php?group_id=57697">here.</A>
*
* \section cvs CVS
* The latest CVS version of libnova is available via CVS <A href="http://sf.net/cvs/?group_id=57697">here.</A>
*
* \section licence Licence
* libnova is released under the <A href="http://www.gnu.org">GNU</A> LGPL.
*
* \section contact Contact
* If you would like further information, then please contact me <A href="mailto:liam@nova-ioe.org">here</A>
*
*	Liam Girdwood
*
* \section thanks Thanks
* Thanks to Jean Meeus for most of the algorithms used in this library.
* From his book "Astronomical Algorithms".
* 
* Thanks to Michelle Chapront-Touze and Jean Chapront for publishing
* the Lunar Solution ELP 2000-82B.
*
* Thanks to Messrs. Bretagnon and Francou for publishing planetary 
* solution VSOP87.
*
* Also thanks to Sourceforge for hosting this project. <A href="http://sourceforge.net"> <IMG src="http://sourceforge.net/sflogo.php?group_id=57697&amp;type=5" width="210" height="62" border="0" alt="SourceForge Logo"></A> 
*/

#ifndef LN_LIBNOVA_H
#define LN_LIBNOVA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>	
	
/*!
** Date
* \struct ln_date 
* \brief Human readable Date and time used by libnova
*/

struct ln_date
{
    int years; 		/*!< Years. All values are valid */
    int months;		/*!< Months. Valid values : 1 (January) - 12 (December) */
    int days; 		/*!< Days. Valid values 1 - 28,29,30,31 Depends on month.*/
    int hours; 		/*!< Hours. Valid values 0 - 23. */
    int minutes; 	/*!< Minutes. Valid values 0 - 59. */
    double seconds;	/*!< Seconds. Valid values 0 - 59.99999.... */
};

/*! \struct ln_dms
** \brief Human readable Angle in degrees, minutes and seconds
*/

struct ln_dms
{
    int degrees;	/*!< Degrees. All values are valid */
    int minutes;	/*!< Minutes. Valid 0 - 59 */
    double seconds;	/*!< Seconds. Valid 0 - 59.9999... */
};

/*! \struct ln_hms
** \brief Human readable Angle in hours, minutes and seconds
*/

struct ln_hms
{
    int hours;		/*!< Hours. Valid 0 - 23 */
    int minutes;	/*!< Minutes. Valid 0 - 59 */
    double seconds;	/*!< Seconds. Valid 0 - 59.9999... */
};

/*! \struct lnh_equ_posn
** \brief Human readable Equatorial Coordinates.
*/

struct lnh_equ_posn
{
    struct ln_hms ra;	/*!< RA. Object right ascension.*/
    struct ln_dms dec;	/*!< DEC. Object declination */
};

/*! \struct lnh_hrz_posn
** \brief Human readable Horizontal Coordinates.
*/

struct lnh_hrz_posn
{
    struct ln_dms az;	/*!< AZ. Object azimuth. */
    struct ln_dms alt;	/*!< ALT. Object altitude. */
};


/*! \struct lnh_lnlat_posn
** \brief Human readable Longitude and Latitude.
*/

struct lnh_lnlat_posn
{
    struct ln_dms lng; /*!< longitude. Object longitude */
    struct ln_dms lat; /*!< latitude. Object latitude */
};


/*! \struct ln_equ_posn
** \brief Equatorial Coordinates.
*/

struct ln_equ_posn
{
    double ra;	/*!< RA. Object right ascension.*/
    double dec;	/*!< DEC. Object declination */
};

/*! \struct ln_hrz_posn
** \brief Horizontal Coordinates.
*/

struct ln_hrz_posn
{
    double az;	/*!< AZ. Object azimuth. */
    double alt;	/*!< ALT. Object altitude. */
};


/*! \struct ln_lnlat_posn
** \brief Longitude and Latitude.
*/

struct ln_lnlat_posn
{
    double lng; /*!< longitude. Object longitude */
    double lat; /*!< latitude. Object latitude */
};


/*! \struct ln_helio_posn
* \brief heliocentric position 
*/
struct ln_helio_posn
{
	double L;	/*!< Heliocentric longitude */
	double B;	/*!< Heliocentric latitude */
	double R;	/*!< Heliocentric radius vector */
};

/*! \struct ln_geo_posn
* \brief geocentric position
*/
struct ln_geo_posn
{
	double X;	/*!< Geocentric X coordinate */
	double Y;	/*!< Geocentric Y coordinate */
	double Z;	/*!< Geocentric Z coordinate */
};

/*!
* \struct ln_orbit
* \brief Orbital elements
*/
struct ln_orbit
{
	double a;	/*!< Semi major axis, in AU */
	double e;	/*!< Eccentricity */
	double i;	/*!< Inclination */
	double w;	/*!< Argument of perihelion */
	double omega;	/*!< Longitude of ascending node */
	double n;	/*!< Mean motion, in degrees/day */
};


/*! \defgroup version Libnova library version information
*/

/*! \fn char * get_ln_version (void)
* \brief Library Version Number
* \ingroup version
*/

char * get_ln_version (void);


/*! \defgroup calendar General Calendar Functions 
*/
 
/*! \fn double get_julian_day (struct ln_date * date)
* \ingroup calendar
* \brief get the Julian day from date.
*/
double get_julian_day (struct ln_date * date);

/*! \fn void get_date (double JD, struct ln_date * date)
* \ingroup calendar
* \brief get the date from the julian day
*/
void get_date (double JD, struct ln_date * date);

/*! \fn unsigned int get_day_of_week (struct ln_date * date)
* \ingroup calendar
* \brief get day of the week from date (0 = Sunday .. 6 = Saturday)
*/
unsigned int get_day_of_week (struct ln_date *date);
	
/*! \fn double get_julian_from_sys ()
* \brief get julian day from system time
* \ingroup calendar
*/
double get_julian_from_sys ();


/*! \fn void get_ln_date_from_sys (struct ln_date * date)
* \brief get date from system date
* \ingroup calendar
*/
void get_ln_date_from_sys (struct ln_date * date);
	
/*! \fn double get_julian_from_timet (time_t * time)
* \brief get julian day from time_t
* \ingroup calendar
*/
double get_julian_from_timet (time_t * time);

/*! \fn void get_timet_from_julian (double JD, time_t * time)
* \brief get time_t from julian day
* \ingroup calendar
*/
void get_timet_from_julian (double JD, time_t * time);

/*! \defgroup misc Misc Functions
*/

/*! \fn double get_dec_location(char * s)
* \ingroup misc
* \brief Obtains Latitude, Longitude, RA or Declination from a string.
*
*  If the last char is N/S doesn't accept more than 90 degrees.            
*  If it is E/W doesn't accept more than 180 degrees.                      
*  If they are hours don't accept more than 24:00                          
*                                                                          
*  Any position can be expressed as follows:                               
*  (please use a 8 bits charset if you want                                
*  to view the degrees separator char '0xba')                              
*
*  42.30.35,53                                                             
*  90º0'0,01 W                                                             
*  42º30'35.53 N                                                           
*  42º30'35.53S                                                            
*  42º30'N                                                                 
*  - 42.30.35.53                                                           
*   42:30:35.53 S                                                          
*  + 42.30.35.53                                                           
*  +42º30 35,53                                                            
*   23h36'45,0                                                             
*                                                                          
*                                                                          
*  42:30:35.53 S = -42º30'35.53"                                           
*  + 42 30.35.53 S the same previous position, the plus (+) sign is        
*  considered like an error, the last 'S' has precedence over the sign     
*                                                                          
*  90º0'0,01 N ERROR: +- 90º0'00.00" latitude limit                        
*
*/
double get_dec_location(char *s);



/*! \fn char * get_humanr_location(double location)    
*  \ingroup misc
*  \brief obtains a human readable location in the form: ddºmm'ss.ss"             
*/

char *get_humanr_location(double location);


/*!
* \defgroup dynamical Dynamical Time 
*/

/*! \fn double get_dynamical_time_diff (double JD)
* \ingroup dynamical
* \brief get approximate Dynamical time difference from JD in seconds
*/
double get_dynamical_time_diff (double JD);

/*! \fn double get_jde (double JD)
* \brief get julian ephemeris day
* \ingroup dynamical 
*/
double get_jde (double JD);

/*! \defgroup sidereal Sidereal Time 
*/

/*! \fn double get_mean_sidereal_time (double JD)
* \brief get mean sidereal time from date.
* returns sidereal time in hours 
* \ingroup sidereal
*/ 
double get_mean_sidereal_time (double JD);

/*! \fn get_apparent_sidereal_time (double JD)
* \brief get apparent sidereal time from date.
* returns sidereal time in hours 
* \ingroup sidereal
*/
 
double get_apparent_sidereal_time (double JD);

/*! \defgroup conversion General Conversion Functions 
*/

/*! \fn double rad_to_deg (double radians)
* \brief radians to degrees
* \ingroup conversion
*/
double rad_to_deg (double radians);

/*! \fn double deg_to_rad (double radians)
* \brief degrees to radians
* \ingroup conversion
*/
double deg_to_rad (double degrees);

/*! \fn double hms_to_deg (struct ln_hms * hms)
* \brief hours to degrees 
* \ingroup conversion 
*/
double hms_to_deg (struct ln_hms * hms);

/*! \fn void deg_to_hms (double degrees, struct ln_hms * hms)
* \brief degrees to hours 
* \ingroup conversion 
*/
void deg_to_hms (double degrees, struct ln_hms * hms);

/*! \fn double hms_to_rad (struct ln_hms * hms)
* \brief hours to radians. 
* \ingroup conversion 
*/
double hms_to_rad (struct ln_hms * hms);

/*! \fn void deg_to_hms (double radians, struct ln_hms * hms)
* \brief radians to hours 
* \ingroup conversion 
*/
void rad_to_hms (double radians, struct ln_hms * hms);

/*! \fn double dms_to_deg (struct ln_dms * dms)
* \brief dms to degrees 
* \ingroup conversion 
*/
double dms_to_deg (struct ln_dms * dms);

/*! \fn void deg_to_dms (double degrees, struct ln_dms * dms)
* \brief degrees to dms
* \ingroup conversion 
*/
void deg_to_dms (double degrees, struct ln_dms * dms);

/*! \fn double dms_to_rad (struct ln_dms * dms)
* \brief dms to radians
* \ingroup conversion 
*/
double dms_to_rad (struct ln_dms * dms);

/*! \fn void rad_to_dms (double radians, struct ln_dms * dms)
* \brief radians to dms 
* \ingroup conversion
*/
void rad_to_dms (double radians, struct ln_dms * dms);

/*! \fn void hequ_to_equ (struct lnh_equ_posn * hpos, struct ln_equ_posn * pos)
* \brief human readable equatorial position to double equatorial position
* \ingroup conversion
*/
void hequ_to_equ (struct lnh_equ_posn * hpos, struct ln_equ_posn * pos);
	
/*! \fn void equ_to_hequ (struct ln_equ_posn * pos, struct lnh_equ_posn * hpos)
* \brief human double equatorial position to human readable equatorial position
* \ingroup conversion
*/
void equ_to_hequ (struct ln_equ_posn * pos, struct lnh_equ_posn * hpos);
	
/*! \fn void hhrz_to_hrz (struct lnh_hrz_posn * hpos, struct ln_hrz_posn * pos)
* \brief human readable horizontal position to double horizontal position
* \ingroup conversion
*/
void hhrz_to_hrz (struct lnh_hrz_posn * hpos, struct ln_hrz_posn * pos);
	
/*! \fn void hrz_to_hhrz (struct ln_hrz_posn * pos, struct lnh_hrz_posn * hpos)
* \brief double horizontal position to human readable horizontal position
* \ingroup conversion
*/
void hrz_to_hhrz (struct ln_hrz_posn * pos, struct lnh_hrz_posn * hpos);
	
/*! \fn void hlnlat_to_lnlat (struct lnh_lnlat_posn * hpos, struct ln_lnlat_posn * pos)
* \brief human readable long/lat position to double long/lat position
* \ingroup conversion
*/
void hlnlat_to_lnlat (struct lnh_lnlat_posn * hpos, struct ln_lnlat_posn * pos);
	
/*! \fn void lnlat_to_hlnlat (struct ln_lnlat_posn * pos, struct lnh_lnlat_posn * hpos)
* \brief double long/lat position to human readable long/lat position
* \ingroup conversion
*/
void lnlat_to_hlnlat (struct ln_lnlat_posn * pos, struct lnh_lnlat_posn * hpos);
	
/*! \fn void add_secs_hms (struct ln_hms * hms, double seconds)
* \brief add seconds to hms 
* \ingroup conversion 
*/
void add_secs_hms (struct ln_hms * hms, double seconds);

/*! \fn void add_hms (struct ln_hms * source, struct ln_hms * dest)
* \brief add hms to hms 
* \ingroup conversion 
*/
void add_hms (struct ln_hms * source, struct ln_hms * dest);

/*! \fn void range_degrees (double angle)
* \brief puts a large angle in the correct range 0 - 360 degrees 
* \ingroup conversion 
*/
double range_degrees (double angle);

/*! \fn void range_radians (double angle)
* \brief puts a large angle in the correct range 0 - 2PI radians 
* \ingroup conversion 
*/
double range_radians (double angle);
double range_radians2 (double angle);

/*! \defgroup transform Transformation of Coordinates using apparent positions 
*/

/*! \fn void get_hrz_from_equ (struct ln_equ_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_hrz_posn *position);
* \brief get horizontal coordinates from equatorial coordinates 
* \ingroup transform 
*/
/* Equ 12.5,12.6 pg 88 */
void get_hrz_from_equ 
	 (struct ln_equ_posn * object, 
	 struct ln_lnlat_posn * observer, 
	 double JD, struct ln_hrz_posn *position);

/*! \fn void get_equ_from_ecl (struct ln_lnlat_posn * object, double JD, struct ln_equ_posn * position);
* \brief get equatorial coordinates from ecliptical coordinates 
* \ingroup transform
*/
/* Equ 12.3, 12.4 pg 89 */
void get_equ_from_ecl 
	(struct ln_lnlat_posn * object, 
	double JD,
	struct ln_equ_posn * position);

/*! \fn void get_ecl_from_equ (struct ln_equ_posn * object, double JD, struct ln_lnlat_posn * position);
* \brief get ecliptical cordinates from equatorial coordinates 
* \ingroup transform
*/
/* Equ 12.1, 12.2 Pg 88 */
void get_ecl_from_equ 
	(struct ln_equ_posn * object,
	double JD,
	struct ln_lnlat_posn * position);

/*! \fn void get_equ_from_hrz (struct ln_hrz_posn *object, struct ln_lnlat_posn * observer, double JD, struct ln_equ_posn * position); 
* \brief get equatorial coordinates from horizontal coordinates  
* \ingroup transform
*/
/* Pg 89 */
void get_equ_from_hrz 
	(struct ln_hrz_posn *object, 
	struct ln_lnlat_posn * observer, 
	double JD,
	struct ln_equ_posn * position); 

/*! \fn void get_geo_from_helio (struct ln_helio_posn *object, double JD, struct ln_geo_posn * position); 
* \brief get geocentric coordinates from heliocentric coordinates  
* \ingroup transform
*/
/* Pg ?? */
void get_geo_from_helio 
	(struct ln_helio_posn *object,  
	double JD,
	struct ln_geo_posn * position); 



/*! \defgroup VSOP87 VSOP87 Theory
*/

/*! \fn void vsop87_to_fk5 (struct ln_helio_posn * position, double JD);
* \ingroup VSOP87
* \brief transform from VSOP87 to FK5 
*/
/* equation 31.3 Pg 207         */
/* JD Julian Day */
void vsop87_to_fk5 (struct ln_helio_posn * position, double JD);


/*! \defgroup earth Earth
*/

/*! \fn void get_earth_centre_dist (float height, double latitude, double * p_sin_angle, double * p_cos_angle);
* \ingroup earth
* \brief get Earth globe centre dist
*/
void get_earth_centre_dist (float height, double latitude, double * p_sin_angle, double * p_cos_angle);

/*! \defgroup nutation Nutation
*/

/*! \fn void get_nutation (double JD, double * longitude, double * obliquity, double * ecliptic);
* \ingroup nutation
* \brief get nutation in degrees for longitude, obliquity and ecliptic 
*/
void get_nutation (double JD, double * longitude, double * obliquity, double * ecliptic);

/*! \defgroup apparent Apparent position of an Object
*/

/*! \fn void get_apparent_posn (struct ln_equ_posn * mean_position, struct ln_equ_posn * proper_motion, double JD, struct ln_equ_posn * position);
* \brief get the apparent position of an object from its mean position, uses mean equatorial coordinates 
* \ingroup apparent
*/
void get_apparent_posn 
	(struct ln_equ_posn * mean_position, 
	struct ln_equ_posn * proper_motion, 
	double JD,
	struct ln_equ_posn * position);


/*! \defgroup precession Precession.
*/

/*! \fn void get_equ_prec (struct ln_equ_posn * mean_position, double JD, struct ln_equ_posn * position);
* \brief get equatorial coordinates with the effects of precession for a given Julian Day
*  uses mean equatorial coordinates
* \ingroup precession
*/

/* Equ 20.2, 20.3, 20.4 pg 126 */
void get_equ_prec 
	(struct ln_equ_posn * mean_position, 
	double JD,
	struct ln_equ_posn * position);

/*! \fn void get_ecl_prec (struct ln_lnlat_posn * mean_position, double JD, struct ln_lnlat_posn * position); 
* \brief get ecliptical coordinates with the effects of precession for a given Julian Day
*  uses mean ecliptical coordinates
* \ingroup precession
*/
/* Equ 20.5, 20.6 pg 128 */
void get_ecl_prec 
	(struct ln_lnlat_posn * mean_position, 
	double JD,
	struct ln_lnlat_posn * position); 


/*! \defgroup motion Proper Motion.
*/

/*! \fn void get_equ_pm (struct ln_equ_posn * mean_position, struct ln_equ_posn * proper_motion, double JD, struct ln_equ_posn * position);
* \brief get equatorial coordinates with the effects of proper motion for a given Julian Day 
* uses mean equatorial coordinates
* \ingroup motion
*/
/* Equ 20.2, 20.3, 20.4 pg 126 */
void get_equ_pm 
	(struct ln_equ_posn * mean_position, 
	struct ln_equ_posn * proper_motion, 
	double JD,
	struct ln_equ_posn * position);


/*! \defgroup aberration Aberration
*/

/*! \fn void get_equ_aber (struct ln_equ_posn * mean_position, double JD, struct ln_equ_posn * position);
* \brief get equatorial coordinates with the effects of Aberration and nutation for a given Julian Day. 
* has low and high accuracy methods for equatorial coordinates
* uses mean equatorial coordinates
* \ingroup aberration
*/
/* Equ 22.1, 22.3, 22.4 an Ron-Vondrak expression */
void get_equ_aber 
	(struct ln_equ_posn * mean_position, 
	double JD,
	struct ln_equ_posn * position);


/*! \fn void get_ecl_aber (struct ln_lnlat_posn * mean_position, double JD, struct ln_lnlat_posn * position); 
* \brief get ecliptical coordinates with the effects of Aberration and nutation for a given Julian Day. 
* uses mean ecliptical coordinates
* \ingroup aberration
*/
/* Equ 22.1, 22.2 pg 139 */
void get_ecl_aber 
	(struct ln_lnlat_posn * mean_position, 
	double JD,
	struct ln_lnlat_posn * position); 


/*! \defgroup solar Solar.
*
* Get the solar ecliptical/equatorial coordinates for a given julian date.  
* Accuracy 0.01 arc second error - uses VSOP87. 
*/

/*! \fn void get_geom_solar_coords (double JD, struct ln_helio_posn * position);
* \brief geometric longitude, latitude and radius vector 
* \ingroup solar 
*/
void get_geom_solar_coords 
	(double JD,
	struct ln_helio_posn * position);

/*! \fn void get_equ_solar_coords (double JD, struct ln_equ_posn * position);
* \brief apparent equatorial coordinates
* \ingroup solar
*/ 
void get_equ_solar_coords 
	(double JD,
	struct ln_equ_posn * position);

/*! \fn void get_ecl_solar_coords (double JD, struct ln_lnlat_posn * position);
* \brief apparent ecliptical coordinates
* \ingroup solar
*/ 
void get_ecl_solar_coords 
	(double JD,
	struct ln_lnlat_posn * position);

/*! \fn void get_geo_solar_coords (double JD, struct ln_geo_posn * position)
* \brief Calculate geocentric coordinates (rectangular)
* \ingroup solar
*/
void get_geo_solar_coords (double JD, struct ln_geo_posn * position);
	

/*! \defgroup mercury Mercury
*/

/*! \fn void get_mercury_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get Mercury heliocentric coordinates
* \ingroup mercury
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_mercury_helio_coords 
	(double JD,
	struct ln_helio_posn * position);


/*! \fn void get_mercury_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Mercury equatorial coordinates
* \ingroup mercury
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_mercury_equ_coords 
	(double JD,
	struct ln_equ_posn * position);

/*! \fn double get_mercury_earth_dist (double JD);
* \brief Calculate the distance between mercury and the earth in AU
* \ingroup mercury
* \return distance in AU
*/ 
/* Chapter ?? */
double get_mercury_earth_dist (double JD);
	
/*! \fn double get_mercury_sun_dist (double JD);
* \brief Calculate the distance between mercury and the sun in AU
* \ingroup mercury
* \return distance in AU
*/ 
/* Chapter ?? */
double get_mercury_sun_dist (double JD);
	
/*! \fn double get_mercury_magnitude (double JD);
* \brief Calculate the visible magnitude of Mercury
* \ingroup mercury
* \return magnitude of Mercury
*/ 
/* Chapter ?? */
double get_mercury_magnitude (double JD);	
	
/*! \fn double get_mercury_disk (double JD);
* \brief Calculate the illuminated fraction of Mercury's disk
* \ingroup mercury
* \return illuminated fraction of mercurys disk
*/ 
/* Chapter 41 */
double get_mercury_disk (double JD);

/*! \fn double get_mercury_phase (double JD);
* \brief Calculate the phase angle of mercury (sun - mercury - earth)
* \ingroup mercury
* \return phase angle of mercury (degrees)
*/ 
/* Chapter 41 */
double get_mercury_phase (double JD);

/*! \defgroup venus Venus
*/

/*! \fn void get_venus_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get venus heliocentric coordinates 
* \ingroup venus
*/
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_venus_helio_coords 
	(double JD,
	struct ln_helio_posn * position);


/*! \fn void get_venus_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Venus heliocentric coordinates
* \ingroup venus
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_venus_equ_coords 
	(double JD,
	struct ln_equ_posn * position);

/*! \fn double get_venus_earth_dist (double JD);
* \brief Calculate the distance between venus and the earth in AU
* \ingroup venus
* \return distance in AU
*/ 
/* Chapter ?? */
double get_venus_earth_dist (double JD);
	
/*! \fn double get_venus_sun_dist (double JD);
* \brief Calculate the distance between venus and the sun in AU
* \ingroup venus
* \return distance in AU
*/ 
/* Chapter ?? */
double get_venus_sun_dist (double JD);
	
/*! \fn double get_venus_magnitude (double JD);
* \brief Calculate the visible magnitude of Venus
* \ingroup venus
* \return magnitude of Venus
*/ 
/* Chapter ?? */
double get_venus_magnitude (double JD);

/*! \fn double get_venus_disk (double JD);
* \brief Calculate the illuminated fraction of Venus disk
* \ingroup venus
* \return illuminated fraction of venus disk
*/ 
/* Chapter 41 */
double get_venus_disk (double JD);

/*! \fn double get_venus_phase (double JD);
* \brief Calculate the phase angle of venus (sun - venus - earth)
* \ingroup venus
* \return phase angle of venus (degrees)
*/ 
/* Chapter 41 */
double get_venus_phase (double JD);

/*
** Earth
*/

/*! \fn void get_earth_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get earth heliocentric coordinates
* \ingroup earth
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_earth_helio_coords 
	(double JD,
	struct ln_helio_posn * position);

/*! \fn void get_earth_sun_dist (double JD);
* \brief Calculate the distance between earth and the sun in AU
* \ingroup earth
* \return distance in AU
*/ 
/* Chapter ?? */
double get_earth_sun_dist (double JD);

/*! \defgroup mars Mars
*/

/*! \fn void get_mars_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get mars heliocentric coordinates
* \ingroup mars
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_mars_helio_coords 
	(double JD,
	struct ln_helio_posn * position);

/*! \fn void get_mars_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Mars heliocentric coordinates
* \ingroup mars
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_mars_equ_coords 
	(double JD,
	struct ln_equ_posn * position);

	/*! \fn double get_mars_earth_dist (double JD);
* \brief Calculate the distance between mars and the earth in AU
* \ingroup mars
* \return distance in AU
*/ 
/* Chapter ?? */
double get_mars_earth_dist (double JD);
	
/*! \fn double get_mars_sun_dist (double JD);
* \brief Calculate the distance between mars and the sun in AU
* \ingroup mars
* \return distance in AU
*/ 
/* Chapter ?? */
double get_mars_sun_dist (double JD);
	
/*! \fn double get_mars_magnitude (double JD);
* \brief Calculate the visible magnitude of Mars
* \ingroup mars
* \return magnitude of Mars
*/ 
/* Chapter ?? */
double get_mars_magnitude (double JD);

/*! \fn double get_mars_disk (double JD);
* \brief Calculate the illuminated fraction of Mars disk
* \ingroup mars
* \return illuminated fraction of mars disk
*/ 
/* Chapter 41 */
double get_mars_disk (double JD);

/*! \fn double get_mars_phase (double JD);
* \brief Calculate the phase angle of mars (sun - mars - earth)
* \ingroup mars
* \return phase angle of mars (degrees)
*/ 
/* Chapter 41 */
double get_mars_phase (double JD);

/*! \defgroup jupiter Jupiter
*/

/*! \fn void get_jupiter_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get jupiter heliocentric coordinates
* \ingroup jupiter
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_jupiter_helio_coords 
	(double JD, struct ln_helio_posn * position);

/*! \fn void get_jupiter_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Jupiter equatorial coordinates coordinates
* \ingroup jupiter
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_jupiter_equ_coords 
	(double JD,
	struct ln_equ_posn * position);

/*! \fn double get_jupiter_earth_dist (double JD);
* \brief Calculate the distance between jupiter and the earth in AU
* \ingroup jupiter
* \return distance in AU
*/ 
/* Chapter ?? */
double get_jupiter_earth_dist (double JD);
	
/*! \fn double get_jupiter_sun_dist (double JD);
* \brief Calculate the distance between jupiter and the sun in AU
* \ingroup jupiter
* \return distance in AU
*/ 
/* Chapter ?? */
double get_jupiter_sun_dist (double JD);
	
/*! \fn double get_jupiter_magnitude (double JD);
* \brief Calculate the visible magnitude of Jupiter
* \ingroup jupiter
* \return magnitude of Jupiter
*/ 
/* Chapter ?? */
double get_jupiter_magnitude (double JD);

/*! \fn double get_jupiter_disk (double JD);
* \brief Calculate the illuminated fraction of Jupiter's disk
* \ingroup jupiter
* \return illuminated fraction of jupiters disk
*/ 
/* Chapter 41 */
double get_jupiter_disk (double JD);

/*! \fn double get_jupiter_phase (double JD);
* \brief Calculate the phase angle of jupiter (sun - jupiter - earth)
* \ingroup jupiter
* \return phase angle of jupiter (degrees)
*/ 
/* Chapter 41 */
double get_jupiter_phase (double JD);

/*! \defgroup saturn Saturn
*/

/*! \fn void get_saturn_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get saturn heliocentric coordinates
* \ingroup saturn
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_saturn_helio_coords 
	(double JD, struct ln_helio_posn * position);


/*! \fn void get_saturn_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Saturn heliocentric coordinates
* \ingroup saturn
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_saturn_equ_coords 
	(double JD,
	struct ln_equ_posn * position);

/*! \fn double get_saturn_earth_dist (double JD);
* \brief Calculate the distance between saturn and the earth in AU
* \ingroup saturn
* \return distance in AU
*/ 
/* Chapter ?? */
double get_saturn_earth_dist (double JD);
	
/*! \fn double get_saturn_sun_dist (double JD);
* \brief Calculate the distance between Saturn and the sun in AU
* \ingroup saturn
* \return distance in AU
*/ 
/* Chapter ?? */
double get_saturn_sun_dist (double JD);
	
/*! \fn double get_saturn_magnitude (double JD);
* \brief Calculate the visible magnitude of Saturn
* \ingroup saturn
* \return magnitude of Saturn
*/ 
/* Chapter ?? */
double get_saturn_magnitude (double JD);

/*! \fn double get_saturn_disk (double JD);
* \brief Calculate the illuminated fraction of Saturn's disk
* \ingroup saturn
* \return illuminated fraction of Saturns disk
*/ 
/* Chapter 41 */
double get_saturn_disk (double JD);

/*! \fn double get_saturn_phase (double JD);
* \brief Calculate the phase angle of saturn (sun - saturn - earth)
* \ingroup saturn
* \return phase angle of saturn (degrees)
*/ 
/* Chapter 41 */
double get_saturn_phase (double JD);

/*! \defgroup uranus Uranus
*/

/*! \fn void get_uranus_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get Uranus heliocentric coordinates
* \ingroup uranus
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_uranus_helio_coords 
	(double JD, struct ln_helio_posn * position);

/*! \fn void get_uranus_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Uranus heliocentric coordinates
* \ingroup uranus
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_uranus_equ_coords 
	(double JD,
	struct ln_equ_posn * position);

/*! \fn double get_uranus_earth_dist (double JD);
* \brief Calculate the distance between uranus and the earth in AU
* \ingroup uranus
* \return distance in AU
*/ 
/* Chapter ?? */
double get_uranus_earth_dist (double JD);
	
/*! \fn double get_uranus_sun_dist (double JD);
* \brief Calculate the distance between uranus and the sun in AU
* \ingroup uranus
* \return distance in AU
*/ 
/* Chapter ?? */
double get_uranus_sun_dist (double JD);
	
/*! \fn double get_uranus_magnitude (double JD);
* \brief Calculate the visible magnitude of Uranus
* \ingroup uranus
* \return magnitude of Uranus
*/ 
/* Chapter ?? */
double get_uranus_magnitude (double JD);

/*! \fn double get_uranus_disk (double JD);
* \brief Calculate the illuminated fraction of Uranus's disk
* \ingroup uranus
* \return illuminated fraction of uranus disk
*/ 
/* Chapter 41 */
double get_uranus_disk (double JD);

/*! \fn double get_uranus_phase (double JD);
* \brief Calculate the phase angle of uranus (sun - uranus - earth)
* \ingroup uranus
* \return phase angle of uranus (degrees)
*/ 
/* Chapter 41 */
double get_uranus_phase (double JD);

/*! \defgroup neptune Neptune
*/

/*! \fn void get_neptune_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get neptune heliocentric coordinates
* \ingroup neptune
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_neptune_helio_coords 
	(double JD, struct ln_helio_posn * position);


/*! \fn void get_neptune_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Neptune heliocentric coordinates
* \ingroup neptune
*/ 
/* Chapter 31 Pg 206-207 Equ 31.1 31.2 , 31.3 using VSOP 87 */
void get_neptune_equ_coords 
	(double JD,
	struct ln_equ_posn * position);
		
/*! \fn double get_neptune_earth_dist (double JD);
* \brief Calculate the distance between neptune and the earth in AU
* \ingroup neptune
* \return distance in AU
*/ 
/* Chapter ?? */
double get_neptune_earth_dist (double JD);
	
/*! \fn double get_neptune_sun_dist (double JD);
* \brief Calculate the distance between neptune and the sun in AU
* \ingroup neptune
* \return distance in AU
*/ 
/* Chapter ?? */
double get_neptune_sun_dist (double JD);
	
/*! \fn double get_neptune_magnitude (double JD);
* \brief Calculate the visible magnitude of Neptune
* \ingroup neptune
* \return magnitude of Neptune
*/ 
/* Chapter ?? */
double get_neptune_magnitude (double JD);

/*! \fn double get_neptune_disk (double JD);
* \brief Calculate the illuminated fraction of Neptune's disk
* \ingroup neptune
* \return illuminated fraction of neptunes disk
*/ 
/* Chapter 41 */
double get_neptune_disk (double JD);

/*! \fn double get_neptune_phase (double JD);
* \brief Calculate the phase angle of neptune (sun - neptune - earth)
* \ingroup neptune
* \return phase angle of neptune (degrees)
*/ 
/* Chapter 41 */
double get_neptune_phase (double JD);

/*! \defgroup pluto Pluto
*/

/*! \fn void get_pluto_helio_coords (double JD, struct ln_helio_posn * position);
* \brief get pluto heliocentric coordinates
* \ingroup pluto
*/ 
/* Chapter 37 Pg 263  */
void get_pluto_helio_coords 
	(double JD, struct ln_helio_posn * position);

/*! \fn void get_pluto_equ_coords (double JD, struct ln_equ_posn * position);
* \brief get Pluto heliocentric coordinates
* \ingroup pluto
*/ 
/* Chapter 37 */
void get_pluto_equ_coords 
	(double JD,
	struct ln_equ_posn * position);
		
/*! \fn double get_pluto_earth_dist (double JD);
* \brief Calculate the distance between pluto and the earth in AU
* \ingroup pluto
* \return distance in AU
*/ 
/* Chapter 37 */
double get_pluto_earth_dist (double JD);
	
/*! \fn double get_pluto_sun_dist (double JD);
* \brief Calculate the distance between pluto and the sun in AU
* \ingroup pluto
* \return distance in AU
*/ 
/* Chapter 37 */
double get_pluto_sun_dist (double JD);
	
/*! \fn double get_pluto_magnitude (double JD);
* \brief Calculate the visible magnitude of Pluto
* \ingroup pluto
* \return magnitude of Pluto
*/ 
/* Chapter 41 */
double get_pluto_magnitude (double JD);

/*! \fn double get_pluto_disk (double JD);
* \brief Calculate the illuminated fraction of Pluto's disk
* \ingroup pluto
* \return illuminated fraction of plutos disk
*/ 
/* Chapter 41 */
double get_pluto_disk (double JD);

/*! \fn double get_pluto_phase (double JD);
* \brief Calculate the phase angle of pluto (sun - pluto - earth)
* \ingroup pluto
* \return phase angle of pluto (degrees)
*/ 
/* Chapter 41 */
double get_pluto_phase (double JD);

/*! \defgroup lunar Lunar
*/

/*! \fn void get_lunar_geo_posn (double JD, struct ln_geo_posn * moon, double precision);
* \brief Calculate the rectangular geocentric lunar cordinates.
* \ingroup lunar
*/ 
/* ELP 2000-82B theory */
void get_lunar_geo_posn (double JD, struct ln_geo_posn * moon, double precision);

/*! \fn void get_lunar_equ_coords (double JD, struct ln_equ_posn * position, double precision);
* \brief get Lunar equatorial coordinates
* \ingroup lunar
*/ 
void get_lunar_equ_coords 
	(double JD,
	struct ln_equ_posn * position, double precision);

/*! \fn void get_lunar_ecl_coords (double JD, struct ln_lnlat_posn * position, double precision);
* \brief get Lunar ecliptical coordinates
* \ingroup lunar
*/ 
void get_lunar_ecl_coords 
	(double JD,
	struct ln_lnlat_posn * position, double precision);

/*! \fn double get_lunar_phase (double JD);
* \brief Calculate the phase angle of the moon (sun - moon - earth)
* \ingroup lunar
*/ 
double get_lunar_phase (double JD);

/*! \fn double get_lunar_disk (double JD);
* \brief Calculate the illuminated fraction of the moons disk
* \ingroup lunar
*/ 
double get_lunar_disk (double JD);
	
/*! \fn double get_lunar_earth_dist (double JD);
* \brief Calculate the distance between the earth and the moon in km.
* \ingroup lunar
*/ 
double get_lunar_earth_dist (double JD);	
	
/*! \fn double get_lunar_bright_limb (double JD);
* \brief Calculate the position angle of the Moon's bright limb.
* \ingroup lunar
*/ 
double get_lunar_bright_limb (double JD);
	
#ifdef __cplusplus
};
#endif


#endif
