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


A simple example showing some lunar calculations.

*/

#include <stdio.h>
#include "libnova.h"

int main (int argc, char* argv[])
{
	double JD;
	struct ln_rect_posn moon;
	struct ln_equ_posn equ;
	struct ln_lnlat_posn ecl;
		
	/* get the julian day from the local system time */
	JD = get_julian_from_sys();
	printf ("JD %f\n",JD);
	
	/* get the lunar geopcentric position in km, earth is at 0,0,0 */
	get_lunar_geo_posn (JD, &moon, 0);
	printf ("lunar x %f  y %f  z %f\n",moon.X, moon.Y, moon.Z);
	
	/* Long Lat */
	get_lunar_ecl_coords (JD, &ecl, 0);
	printf ("lunar long %f  lat %f\n",ecl.lng, ecl.lat);
	
	/* RA, DEC */
	get_lunar_equ_coords (JD, &equ, 0);
	printf ("lunar RA %f  Dec %f\n",equ.ra, equ.dec);
	
	/* moon earth distance */
	printf ("lunar distance km %f\n", get_lunar_earth_dist(JD));
	
	/* lunar disk, phase and bright limb */
	printf ("lunar disk %f\n", get_lunar_disk(JD));
	printf ("lunar phase %f\n", get_lunar_phase(JD));
	printf ("lunar bright limb %f\n", get_lunar_bright_limb(JD));
	
	return 0;
}
