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
#include <math.h>
#include <stdio.h>


/*! \fn void get_geom_solar_coords (double JD, struct ln_helio_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate geometric coordinates and radius vector
* accuracy 0.01 arc second error - uses VSOP87 solution.
*
* Latitude and Longitude returned are in degrees, whilst radius
* vector returned is in AU.
*/

void get_geom_solar_coords (double JD, struct ln_helio_posn * position)
{		
	/* get earths heliocentric position */
	get_earth_helio_coords (JD, position);

	position->L += 180.0;
	position->L = range_degrees (position->L);
	position->B *= -1.0;
}

/*! \fn void get_equ_solar_coords (double JD, struct ln_equ_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate apparent equatorial solar coordinates for given julian day.
* This function includes the effects of aberration and nutation. 
*/
void get_equ_solar_coords (double JD, struct ln_equ_posn * position)
{
	struct ln_helio_posn sol;
	struct ln_lnlat_posn LB;
	struct ln_nutation nutation;
	double aberration;
	
	/* get geometric coords */
	get_geom_solar_coords (JD, &sol);
	
	/* add nutation */
	get_nutation (JD, &nutation);
	sol.L += nutation.longitude;

	/* aberration */
	aberration = (20.4898 / (360 * 60 * 60)) / sol.R;
	sol.L -= aberration;
	
	/* transform to equatorial */
	LB.lat = sol.B;
	LB.lng = sol.L;
	get_equ_from_ecl (&LB, JD, position);
}

/*! \fn void get_ecl_solar_coords (double JD, struct ln_lnlat_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate apparent ecliptical solar coordinates for given julian day.
* This function includes the effects of aberration and nutation. 
*/
void get_ecl_solar_coords (double JD, struct ln_lnlat_posn * position)
{
	struct ln_helio_posn sol;
	struct ln_nutation nutation;
	double aberration;
	
	/* get geometric coords */
	get_geom_solar_coords (JD, &sol);
	
	/* add nutation */
	get_nutation (JD, &nutation);
	sol.L += nutation.longitude;

	/* aberration */
	aberration = (20.4898 / (360 * 60 * 60)) / sol.R;
	sol.L -= aberration;
	
	position->lng = sol.L;
	position->lat = sol.B;
}

/*! \fn void get_geo_solar_coords (double JD, struct ln_rect_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate geocentric coordinates (rectangular) for given julian day.
* Accuracy 0.01 arc second error - uses VSOP87 solution.
* Position returned is in units of AU.
*/
void get_geo_solar_coords (double JD, struct ln_rect_posn * position)
{		
	/* get earths's heliocentric position */
	struct ln_helio_posn sol;
	get_earth_helio_coords (JD, &sol);
	
	/* now get rectangular coords */
	get_rect_from_helio (&sol, position);
	position->X *=-1.0;
	position->Y *=-1.0;
	position->Z *=-1.0;
}

int get_solar_rst_horizont (double JD, struct ln_lnlat_posn * observer, double horizont, struct ln_rst_time * rst)
{
	return get_body_rst_horizont (JD, observer, get_equ_solar_coords, horizont, rst);
}


/*! \fn double get_solar_rst (double JD, struct ln_lnlat_posn * observer, struct ln_rst_time * rst);
 * Calls get_solar_rst_horizont with horizont set to SOLAR_STANDART_HORIZONT.
 */

int get_solar_rst (double JD, struct ln_lnlat_posn * observer, struct ln_rst_time * rst)
{
	return get_solar_rst_horizont (JD, observer, SOLAR_STANDART_HORIZONT, rst);
}

/*! \fn double get_solar_sdiam (double JD)
* \param JD Julian day
* \return Semidiameter in arc seconds
*
* Calculate the semidiameter of the Sun in arc seconds for the 
* given julian day.
*/
double get_solar_sdiam (double JD)
{
	double S, So = 959.63; /* at 1 AU */
	double dist;
	
	dist = get_earth_sun_dist (JD);
	S = So / dist;
	
	return (S);
}
