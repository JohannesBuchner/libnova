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

Copyright 2002 Liam Girdwood
 
*/

#include "libnova.h"
#include <math.h>

/*
* \fn double get_comet_mag (double JD, struct ln_orbit * orbit, double g, double k)
* \param JD Julian day.
* \param orbit Orbital parameters
* \param g Absolute magnitude
* \param k Comet constant
* \return The visual magnitude. 
*
* Calculate the visual magnitude of a comet.
*/
double get_comet_mag (double JD, struct ln_orbit * orbit, double g, double k)
{
	double mag;
	double d, r;
	double E,M;
	
	/* get mean anomaly */
	if (orbit->n == 0)
		orbit->n = get_mean_motion (orbit->a);
	M = get_mean_anomaly (orbit->n, JD - orbit->JD);
	
	/* get eccentric anomaly */
	E = solve_kepler (orbit->e, M);
	
	/* get radius vector */
	r = get_radius_vector (orbit->a, orbit->e, E);
	d = get_body_solar_dist (JD, orbit);
	
	mag = g + 5.0 * log (d) + k * log (r);
}
