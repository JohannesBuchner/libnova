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

/*!
* \fn double get_asteroid_mag (double JD, struct ln_ell_orbit * orbit, double H, double G)
* \param JD Julian day.
* \param orbit Orbital parameters
* \param H Mean absolute visual magnitude
* \param G Slope parameter
* \return The visual magnitude. 
*
* Calculate the visual magnitude of an asteroid.
*/
double get_asteroid_mag (double JD, struct ln_ell_orbit * orbit, double H, double G)
{
	double mag;
	double t1,t2;
	double b,r,d;
	double E,M;
	
	/* get phase angle */
	b = get_ell_body_phase_angle (JD, orbit);
	b = deg_to_rad (b);
	
	/* get mean anomaly */
	if (orbit->n == 0)
		orbit->n = get_ell_mean_motion (orbit->a);
	M = get_ell_mean_anomaly (orbit->n, JD - orbit->JD);
	
	/* get eccentric anomaly */
	E = solve_kepler (orbit->e, M);
	
	/* get radius vector */
	r = get_ell_radius_vector (orbit->a, orbit->e, E);
	d = get_ell_body_solar_dist (JD, orbit);
	
	t1 = exp (-3.33 * pow (tan (b / 2.0), 0.63));
	t2 = exp (-.187 * pow (tan (b / 2.0), 1.22));
	
	mag = H + 5 * log10 (r * d) - 2.5 * log10 ((1.0 - G) * t1 + G * t2);
	
	return (mag);
}
