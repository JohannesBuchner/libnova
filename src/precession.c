/*
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 

Copyright 2000 Liam Girdwood
 
*/

#include "libnova.h"
#include "precession.h"
#include <math.h>


/*
** Precession
*/

/*! \fn void get_equ_prec (struct ln_equ_posn * mean_position, double JD, struct ln_equ_posn * position)
* \param mean_position Mean object position
* \param JD Julian day
* \param position Pointer to store new object position.
*
* Calculate equatorial coordinates with the effects of precession for a given Julian Day. 
* Uses mean equatorial coordinates and is
* only for initial epoch J2000.0 
*/
/* Equ 20.2, 20.3, 20.4 pg 126 
*/
void get_equ_prec (struct ln_equ_posn * mean_position, double JD, struct ln_equ_posn * position)
{
	double t, t2, t3, A, B, C, zeta, eta, theta, ra, dec, mean_ra, mean_dec, T;
	
	/* change original ra and dec to radians */
	mean_ra = deg_to_rad(mean_position->ra);
	mean_dec = deg_to_rad(mean_position->dec);

	/* calc t, T, zeta, eta and theta for J2000.0 Equ 20.3 */
	t = (JD - 2451545.0) / 36525.0;
	T = t * 100.0;
	t *= 1.0 / 3600.0;
	t2 = t * t;
	t3 = t2 *t;
	zeta = 2306.2181 * t + 0.30188 * t2 + 0.017998 * t3;
	eta = 2306.2181 * t + 1.09468 * t2 + 0.041833 * t3;
	theta = 2004.3109 * t - 0.42665 * t2 - 0.041833 * t3;
	zeta = deg_to_rad(zeta);
	eta = deg_to_rad(eta);
	theta = deg_to_rad(theta); 
/*	zeta = range_radians(zeta);
	eta = range_radians(eta);
	theta = range_radians(theta);
*/	

	/* calc A,B,C equ 20.4 */
	A = cos(mean_dec) * sin(mean_ra + zeta);
	B = cos(theta) * cos(mean_dec) * cos(mean_ra + zeta) - sin(theta) * sin (mean_dec);
	C = sin(theta) * cos (mean_dec) * cos(mean_ra + zeta) + cos(theta) * sin(mean_dec);
	
	ra = atan2 (A,B) + eta;
	
	/* check for object near celestial pole */
	if (mean_dec > (0.4 * M_PI) || mean_dec < (-0.4 * M_PI))
	{
		/* close to pole */
		dec = acos(sqrt(A * A + B * B));
	}
	else
	{
		/* not close to pole */
		dec = asin (C);
	}

	/* change to degrees */
	position->ra = rad_to_deg (ra);
	position->dec = rad_to_deg (dec);
}

/*! \fn void get_ecl_prec (struct ln_lnlat_posn * mean_position, double JD, struct ln_lnlat_posn * position)
* \param mean_position Mean object position
* \param JD Julian day
* \param position Pointer to store new object position.
*
* Calculate ecliptical coordinates with the effects of precession for a given Julian Day. 
* Uses mean ecliptical coordinates and is
* only for initial epoch J2000.0  
* \todo To be implemented. 
*/
/* Equ 20.5, 20.6 pg 128
*/
void get_ecl_prec (struct ln_lnlat_posn * mean_position, double JD, struct ln_lnlat_posn * position)
{

}


