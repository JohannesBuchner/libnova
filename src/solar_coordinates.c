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
#include "solar_coordinates.h"
#include <math.h>


/*! \fn void get_geometric_solar_coordinates (double JD, struct ln_solar_position * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate geometric coordinates and radius vector
* accuracy 0.01 arc second error - uses VSOP87 
*/

void get_geometric_solar_coordinates (double JD, struct ln_heliocentric_position * position)
{		
	/* get earths heliocentric position */
	get_earth_heliocentric_coordinates (JD, position);

	position->L += 180.0;
	position->L = range_degrees (position->L);
	/*position->B *= -1.0;*/
	/*position->R = 0;*/
	
	/* convert to fk5 
	vsop87_to_fk5 (position, JD);*/
}

/*! \fn void get_apparent_solar_coordinates_equatorial (double JD, struct ln_equ_position * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate apparent equatorial solar coordinates. 
*/
void get_solar_coordinates_equatorial (double JD, struct ln_equ_position * position)
{
	struct ln_heliocentric_position sol;
	struct ln_long_lat_position LB;
	double aberration, longitude, obliquity, ecliptic;
	
	/* get geometric coords */
	get_geometric_solar_coordinates (JD, &sol);
	
	/* add nutation */
	get_nutation (JD, &longitude, &obliquity, &ecliptic);
	sol.L += longitude;

	/* aberration */
	aberration = (20.4898 / (360 * 60 * 60)) / sol.R;
	sol.L -= aberration;

	sol.B *= -1.0; /* not sure about this */ 
	
	/* transform to equatorial */
	LB.lat = sol.B;
	LB.lng = sol.L;
	get_equatorial_from_ecliptical (&LB, JD, position);
}

/*! \fn void get_apparent_solar_coordinates_ecliptical (double JD, struct ln_long_lat_position * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate apparent ecliptical solar coordinates. 
*/
void get_solar_coordinates_ecliptical (double JD, struct ln_long_lat_position * position)
{
	struct ln_heliocentric_position sol;
	double aberration, longitude, obliquity, ecliptic;
	
	/* get geometric coords */
	get_geometric_solar_coordinates (JD, &sol);
	
	/* add nutation */
	get_nutation (JD, &longitude, &obliquity, &ecliptic);
	sol.L += longitude;

	/* aberration */
	aberration = (20.4898 / (360 * 60 * 60)) / sol.R;
	sol.L -= aberration;
	
	position->lng = sol.L;
	position->lat = sol.B;
}

/*! \fn void get_geocentric_solar_coordinates (double JD, struct ln_solar_position * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate geocentric coordinates (rectangular)
* accuracy 0.01 arc second error - uses VSOP87 
*/
void get_geocentric_solar_coordinates (double JD, struct ln_geocentric_position * position)
{		
	/* get earths's heliocentric position */
	struct ln_heliocentric_position sol;
	get_earth_heliocentric_coordinates (JD, &sol);
	sol.L +=180.0;
	sol.B *=-1.0;
	sol.R = 0;
	
	/* now get rectangular coords */
	get_geocentric_from_heliocentric (&sol, JD, position);
}


