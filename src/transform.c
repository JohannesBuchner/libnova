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

Copyright (C) 2000 Liam Girdwood <liam@gnova.org>

*/

#include <math.h>
#include <libnova/transform.h>
#include <libnova/utility.h>
#include <libnova/sidereal_time.h>
#include <libnova/nutation.h>

/*! \fn void ln_get_rect_from_helio (struct ln_helio_posn *object, struct ln_rect_posn * position); 
* \param object Object heliocentric coordinates
* \param position Pointer to store new position
*
* Transform an objects heliocentric ecliptical coordinates
* into heliocentric rectangular coordinates.
*/
/* Equ 37.1 Pg 264
*/
void ln_get_rect_from_helio 
	(struct ln_helio_posn *object,  
	struct ln_rect_posn * position)
{
	double sin_e, cos_e;
	double cos_B, sin_B, sin_L, cos_L;
	
	/* ecliptic J2000 */
	sin_e = 0.397777156;
	cos_e = 0.917482062;

	/* calc common values */
	cos_B = cos(ln_deg_to_rad(object->B));
	cos_L = cos(ln_deg_to_rad(object->L));
	sin_B = sin(ln_deg_to_rad(object->B));
	sin_L = sin(ln_deg_to_rad(object->L));
	
	/* equ 37.1 */
	position->X = object->R * cos_L * cos_B;
	position->Y = object->R * (sin_L * cos_B * cos_e - sin_B * sin_e);
	position->Z = object->R * (sin_L * cos_B * sin_e + sin_B * cos_e);
}

/*! \fn void ln_get_hrz_from_equ (struct ln_equ_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_hrz_posn * position)
* \param object Object coordinates.
* \param observer Observer cordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects equatorial coordinates into horizontal coordinates
* for the given julian day and observers position.
* 
* 0 deg azimuth = south, 90 deg = west.
*/
/* Equ 12.1,12.2 pg 88 
*
* TODO:
* Transform horizontal coordinates to galactic coordinates.
* Transfrom equatorial coordinates to galactic coordinates.
*/

void ln_get_hrz_from_equ (struct ln_equ_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_hrz_posn * position)
{
	double sidereal;

	/* get mean sidereal time in hours*/
	sidereal = ln_get_mean_sidereal_time (JD);
	ln_get_hrz_from_equ_sidereal_time (object, observer, sidereal, position);
}


void ln_get_hrz_from_equ_sidereal_time (struct ln_equ_posn * object, struct ln_lnlat_posn * observer, double sidereal, struct ln_hrz_posn * position)
{
	long double H, ra, latitude, declination, A, Ac, As, h, Z, Zs;

	/* change sidereal_time from hours to radians*/
	sidereal *= 2.0 * M_PI / 24.0;

	/* calculate hour angle of object at observers position */
	ra = ln_deg_to_rad (object->ra);
	H = sidereal + ln_deg_to_rad (observer->lng) - ra;

	/* hence formula 12.5 and 12.6 give */
	/* convert to radians - hour angle, observers latitude, object declination */
	latitude = ln_deg_to_rad (observer->lat);
	declination = ln_deg_to_rad (object->dec);

	/* formula 12.6 *; missuse of A (you have been warned) */
	A = sin (latitude) * sin (declination) + cos (latitude) * cos (declination) * cos (H);
	h = asin (A);

	/* covert back to degrees */
	position->alt = ln_rad_to_deg (h);   

	/* zenith distance, Telescope Control 6.8a */
	Z = acos (A);

	/* is'n there better way to compute that? */
	Zs = sin (Z);

	/* sane check for zenith distance; don't try to divide by 0 */

	if (Zs < 1e-5) {
		if (observer->lat > 0)
			position->az = 180;
		else
			position->az = 0;
		return;
	}

	/* formulas TC 6.8d Taff 1991, pp. 2 and 13 - vector transformations */
	As = (cos (declination) * sin (H)) / Zs;
	Ac = (sin (latitude) * cos (declination) * cos (H) - cos (latitude) * sin (declination)) / Zs;

	// don't blom at atan2
	if (fabs(As) < 1e-5) {
		position->az = 0;
		return;
	}
	A = atan2 (As, Ac);

	// normalize A
	A = (A < 0) ? 2 * M_PI + A : A;

	/* covert back to degrees */
	position->az = ln_range_degrees(ln_rad_to_deg (A));
}

/*! \fn void ln_get_equ_from_hrz (struct ln_hrz_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_equ_posn * position)
* \param object Object coordinates.
* \param observer Observer cordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects horizontal coordinates into equatorial coordinates
* for the given julian day and observers position.
*/
void ln_get_equ_from_hrz (struct ln_hrz_posn * object, struct ln_lnlat_posn * observer, double JD, struct ln_equ_posn * position)
{
	long double H, longitude, declination, latitude, A, h, sidereal;

	/* change observer/object position into radians */

	/* object alt/az */
	A = ln_deg_to_rad (object->az);
	h = ln_deg_to_rad (object->alt);

	/* observer long / lat */
	longitude = ln_deg_to_rad (observer->lng);
	latitude = ln_deg_to_rad (observer->lat);

	/* equ on pg89 */
	H = atan2 (sin (A), ( cos(A) * sin (latitude) + tan(h) * cos (latitude)));
	declination = sin(latitude) * sin(h) - cos(latitude) * cos(h) * cos(A);
	declination = asin (declination);

	/* get ra = sidereal - longitude + H and change sidereal to radians*/
	sidereal = ln_get_apparent_sidereal_time(JD);
	sidereal *= 2.0 * M_PI / 24.0;

	position->ra = ln_rad_to_deg (sidereal - H + longitude);
	position->dec = ln_rad_to_deg (declination);
}

/*! \fn void ln_get_equ_from_ecl (struct ln_lnlat_posn * object, double JD, struct ln_equ_posn * position)
* \param object Object coordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects ecliptical coordinates into equatorial coordinates
* for the given julian day.
*/
/* Equ 12.3, 12.4 pg 89 
*/
void ln_get_equ_from_ecl (struct ln_lnlat_posn * object, double JD, struct ln_equ_posn * position)
{
	double ra, declination, longitude, latitude;
	struct ln_nutation nutation;

	/* get obliquity of ecliptic and change it to rads */
	ln_get_nutation (JD, &nutation);
	nutation.ecliptic = ln_deg_to_rad (nutation.ecliptic); 

	/* change object's position into radians */

	/* object */
	longitude = ln_deg_to_rad(object->lng);
	latitude = ln_deg_to_rad(object->lat);

	/* Equ 12.3, 12.4 */
	ra = atan2 ((sin(longitude) * cos(nutation.ecliptic) - tan(latitude) * sin(nutation.ecliptic)), cos (longitude));
	declination = sin(latitude) * cos(nutation.ecliptic) + cos(latitude) * sin(nutation.ecliptic) * sin(longitude);
	declination = asin(declination);
	
	/* store in position */
	position->ra = ln_range_degrees(ln_rad_to_deg(ra));
	position->dec = ln_rad_to_deg(declination);
}

/*! \fn void ln_get_ecl_from_equ (struct ln_equ_posn * object, double JD, struct ln_lnlat_posn * position)
* \param object Object coordinates.
* \param JD Julian day
* \param position Pointer to store new position.
*
* Transform an objects equatorial cordinates into ecliptical coordinates
* for the given julian day.
*/
/* Equ 12.1, 12.2 Pg 88 
*/
void ln_get_ecl_from_equ (struct ln_equ_posn * object, double JD, struct ln_lnlat_posn * position)
{
	double ra, declination, latitude, longitude;
	struct ln_nutation nutation;
	
	/* object position */
	ra = ln_deg_to_rad (object->ra);
	declination = ln_deg_to_rad (object->dec);
	ln_get_nutation(JD, &nutation);
	nutation.ecliptic = ln_deg_to_rad (nutation.ecliptic);

	/* Equ 12.1, 12.2 */
	longitude = atan2 ((sin(ra) * cos(nutation.ecliptic) + tan(declination) * sin(nutation.ecliptic)), cos(ra));
	latitude = sin(declination) * cos(nutation.ecliptic) - cos(declination) * sin(nutation.ecliptic) * sin(ra);
	latitude = asin(latitude);

	/* store in position */
	position->lat = ln_rad_to_deg (latitude);
	position->lng = ln_range_degrees(ln_rad_to_deg (longitude));
}

/*! \fn void ln_get_ecl_from_rect (struct ln_rect_posn * rect, struct ln_lnlat_posn * posn)
* \param rect Rectangular coordinates.
* \param posn Pointer to store new position.
*
* Transform an objects rectangular coordinates into ecliptical coordinates.
*/
/* Equ 33.2
*/
void ln_get_ecl_from_rect (struct ln_rect_posn * rect, struct ln_lnlat_posn * posn)
{
	double t;
	
	t = sqrt (rect->X * rect->X + rect->Y * rect->Y);
	posn->lng = ln_range_degrees(ln_rad_to_deg (atan2 (rect->X, rect->Y)));
	posn->lat = ln_rad_to_deg (atan2 (t, rect->Z));
}
