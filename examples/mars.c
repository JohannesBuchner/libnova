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

Copyright (C) 2003 Liam Girdwood <liam@nova-ioe.org>


A simple example showing some planetary calculations.

*/

#include <stdio.h>
#include "libnova.h"

int main (int argc, char * argv[])
{
	struct ln_helio_posn pos;
	struct lnh_equ_posn hequ;
	struct ln_equ_posn equ;
	double JD;
	double au;
	
	/* get Julian day from local time */
	JD = get_julian_from_sys();	

	/* longitude, latitude and radius vector */
	get_mars_helio_coords(JD, &pos);	
	printf("Mars L %f B %f R %f\n", pos.L, pos.B, pos.R);
	
	/* RA, DEC */
	get_mars_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Mars RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	
	/* Earth - Mars dist AU */
	au = get_mars_earth_dist (JD);
	printf ("mars -> Earth dist (AU) %f\n",au);
	
	/* Sun - Mars Dist AU */
	au = get_mars_sun_dist (JD);
	printf ("mars -> Sun dist (AU) %f\n",au);
	
	/* Mars disk, magnitude and phase */
	au = get_mars_disk (JD);
	printf ("mars -> illuminated disk %f\n",au);
	au = get_mars_magnitude (JD);
	printf ("mars -> magnitude %f\n",au);
	au = get_mars_phase (JD);
	printf ("mars -> phase %f\n",au);
	
	return 0;
}
