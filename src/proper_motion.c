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
#include "proper_motion.h"
#include <stdlib.h>

/*
** Proper Motion.
*/

/*! \fn void get_equ_pm (struct ln_equ_posn * mean_position, struct ln_equ_posn * proper_motion, double JD, struct ln_equ_posn * position)
* \param mean_position Mean position of object.
* \param proper_motion Proper motion of object.
* \param JD Julian Day.
* \param position Pointer to store new object position.
*
* Calculate a stars equatorial coordinates from it's mean coordinates with the
* effects of proper motion for a given Julian Day. 
*/ 
/* Equ 20.2, 20.3, 20.4 pg 126 
*/
void get_equ_pm 
	(struct ln_equ_posn * mean_position, 
	struct ln_equ_posn * proper_motion, 
	double JD,
	struct ln_equ_posn * position)
{
	double mean_ra, mean_dec, T;
	
	T = (JD - 2451545.0) / 365.25;
	
	/* change original ra and dec to radians */
	mean_ra = deg_to_rad(mean_position->ra);
	mean_dec = deg_to_rad(mean_position->dec);

	/* calc proper motion */
	mean_ra += T * deg_to_rad(proper_motion->ra);
	mean_dec += T * deg_to_rad(proper_motion->dec);
	
	/* change to degrees */
	position->ra = rad_to_deg (mean_ra);
	position->dec = rad_to_deg (mean_dec);
}


