/*
This program is free software; you can redistribute it and/or modify
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
#include "transform.h"
#include "libnova.h"
#include <math.h>

/*! \fn void get_rect_from_helio (struct ln_helio_posn *object, double JD, struct ln_geo_posn * position); 
* \param object Object heliocentric coordinates
* \param JD Julian Day
* \param position Pointer to store new position
*
* Transform an objects heliocentric ecliptical coordinates
* into heliocentric rectangular coordinates.
*/
/* Equ 37.1 Pg 264
*/
void get_rect_from_helio 
	(struct ln_helio_posn *object,  
	double JD,
	struct ln_rect_posn * position)
{
	double sin_e, cos_e;
	double cos_B, sin_B, sin_L, cos_L;
	
	/* ecliptic J2000 */
	sin_e = 0.397777156;
	cos_e = 0.917482062;

	/* calc common values */
	cos_B = cos(deg_to_rad(object->B));
	cos_L = cos(deg_to_rad(object->L));
	sin_B = sin(deg_to_rad(object->B));
	sin_L = sin(deg_to_rad(object->L));
	
	/* equ 37.1 */
	position->X = object->R * cos_L * cos_B;
	position->Y = object->R * (sin_L * cos_B * cos_e - sin_B * sin_e);
	position->Z = object->R * (sin_L * cos_B * sin_e + sin_B * cos_e);
}

/*! \fn void get_hrz_from_equ (struct ln_equ_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_hrz_posn * position)
* \param object Object coordinates.
* \param observer Observer cordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects equatorial coordinates into horizontal coordinates
* for the given julian day and observers position.
*/
/* Equ 12.1,12.2 pg 88 
*
* TODO:
* Transform horizontal coordinates to galactic coordinates.
* Transfrom equatorial coordinates to galactic coordinates.
*/

void get_hrz_from_equ 
	(struct ln_equ_posn * object, 
	struct ln_lnlat_posn * observer, 
	double JD, 
	struct ln_hrz_posn * position)
{
	double H, ra, longitude, latitude, declination, A, h, sidereal;


	/* get mean sidereal time in hours and change it to radians*/
	sidereal = get_mean_sidereal_time (JD);
	sidereal *= 2.0 * M_PI / 24.0;

	/* calculate hour angle of object at observers position */
	ra = deg_to_rad (object->ra);
	longitude = deg_to_rad (observer->lng);
	H = sidereal - longitude - ra;


	/* hence formula 12.5 and 12.6 give */
	/* convert to radians - hour angle, observers latitude, object declination */

	latitude = deg_to_rad (observer->lat);
	declination = deg_to_rad (object->dec);

	/* formula 12.5 */
	A = (sin (H)) / ((cos (H) * sin (latitude)) - (tan (declination) * cos (latitude)));
	A = atan (A);
	
	/* formula 12.6 */
	h = sin (latitude) * sin (declination) + cos (latitude) * cos (declination) * cos (H);
	h = asin (h);

	/* covert back to degrees */
	position->alt = rad_to_deg (h);   
	position->az = rad_to_deg (A);
}

/*! \fn void get_equ_from_hrz (struct ln_hrz_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_equ_posn * position)
* \param object Object coordinates.
* \param observer Observer cordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects horizontal coordinates into equatorial coordinates
* for the given julian day and observers position.
*/
void get_equ_from_hrz 
	(struct ln_hrz_posn * object, 
	struct ln_lnlat_posn * observer, 
	double JD,
	struct ln_equ_posn * position)
	
{
	double H, ra, longitude, declination, latitude, A, h, sidereal;


	/* change observer/object position into radians */

	/* object alt/az */
	A = deg_to_rad (object->az);
	h = deg_to_rad (object->alt);

	/* observer long / lat */
	longitude = deg_to_rad (observer->lng);
	latitude = deg_to_rad (observer->lat);

	/* equ on pg89 */
	H = sin (A) / ( cos(A) * sin (latitude) + tan(h) * cos (latitude));
	H = atan (H);
	declination = sin(latitude) * sin(h) - cos(latitude) * cos(h) * cos(A);
	declination = asin (declination);

	/* get ra = sidereal - longitude + H and change sidereal to radians*/
	sidereal = get_apparent_sidereal_time(JD);
	sidereal *= 2.0 * M_PI / 24.0;

	position->ra = rad_to_deg (sidereal - H - longitude);
	position->dec = rad_to_deg (declination);
}

/*! \fn void get_equ_from_ecl (struct ln_lnlat_posn * object, double JD, struct ln_equ_posn * position)
* \param object Object coordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects ecliptical coordinates into equatorial coordinates
* for the given julian day.
*/
/* Equ 12.3, 12.4 pg 89 
*/
void get_equ_from_ecl 
	(struct ln_lnlat_posn * object,
	 double JD,
	 struct ln_equ_posn * position)
	 
{
	double ra, declination, nutation_longitude, nutation_obliquity, nutation_ecliptic, longitude, latitude;


	/* get obliquity of ecliptic and change it to rads */
	get_nutation (JD, &nutation_longitude, &nutation_obliquity, &nutation_ecliptic);
	nutation_ecliptic = deg_to_rad (nutation_ecliptic); 


	/* change object's position into radians */

	/* object */
	longitude = deg_to_rad(object->lng);
	latitude = deg_to_rad(object->lat);

	/* Equ 12.3, 12.4 */
	ra = (sin(longitude) * cos(nutation_ecliptic) - tan(latitude) * sin(nutation_ecliptic)) / cos (longitude);
	ra = atan (ra);
	
	if (ra < 0)
		ra += M_PI;
	if (longitude > M_PI)
		ra += M_PI;


	declination = sin(latitude) * cos(nutation_ecliptic) + cos(latitude) * sin(nutation_ecliptic) * sin(longitude);
	declination = asin(declination);
	
	/* store in position */
	position->ra = rad_to_deg(ra);
	position->dec = rad_to_deg(declination);
}

/*! \fn void get_ecl_from_equ (struct ln_equ_posn * object, double JD, struct ln_lnlat_posn * position)
* \param object Object coordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects equatorial cordinates into ecliptical coordinates
* for the given julian day.
*/
/* Equ 12.1, 12.2 Pg 88 
*/
void get_ecl_from_equ 
	(struct ln_equ_posn * object, 
	double JD,
	struct ln_lnlat_posn * position)
	
{
	double ra, declination, nutation_longitude, nutation_obliquity, nutation_ecliptic, latitude, longitude;

	/* object position */
	ra = deg_to_rad (object->ra);
	declination = deg_to_rad (object->dec);
	get_nutation(JD, &nutation_longitude, &nutation_obliquity, &nutation_ecliptic);
	nutation_ecliptic = deg_to_rad (nutation_ecliptic);

	/* Equ 12.1, 12.2 */
	longitude = (sin(ra) * cos(nutation_ecliptic) + tan(declination) * sin(nutation_ecliptic)) / cos(ra);
	longitude = atan(longitude);
	if(longitude < 0)
		longitude += M_PI;
	
	latitude = sin(declination) * cos(nutation_ecliptic) - cos(declination) * sin(nutation_ecliptic) * sin(ra);
	latitude = asin(latitude);

	/* store in position */
	position->lat = rad_to_deg (latitude);
	position->lng = rad_to_deg (longitude);
}

/*! \fn void get_ecl_from_rect (struct ln_rect_posn * rect, struct ln_lnlat_posn * posn)
* \param rect Rectangular coordinates.
* \param posn Pointer to store new position.
*
* Transform an objects rectangular coordinates into ecliptical coordinates.
*/
/* Equ 33.2
*/
void get_ecl_from_rect (struct ln_rect_posn * rect, struct ln_lnlat_posn * posn)
{
	double t;
	
	t = sqrt (rect->X * rect->X + rect->Y * rect->Y);
	posn->lng = rad_to_deg (atan2(rect->Y, rect->X));
	posn->lat = rad_to_deg (atan2 (rect->Z, t));
}
