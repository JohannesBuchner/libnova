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

Copyright (C) 2003 Liam Girdwood <liam@gnova.org>


A simple example showing some lunar calculations.

*/

#include <stdio.h>
#include <libnova/lunar.h>
#include <libnova/julian_day.h>
#include <libnova/rise_set.h>
#include <libnova/transform.h>

void print_date (char * title, struct ln_zonedate* date)
{
	printf ("\n%s\n",title);
	printf (" Year    : %d\n", date->years);
	printf (" Month   : %d\n", date->months);
	printf (" Day     : %d\n", date->days);
	printf (" Hours   : %d\n", date->hours);
	printf (" Minutes : %d\n", date->minutes);
	printf (" Seconds : %f\n", date->seconds);
}

int main (int argc, char* argv[])
{
	double JD;
	struct ln_rect_posn moon;
	struct ln_equ_posn equ;
	struct ln_lnlat_posn ecl;
	struct ln_lnlat_posn observer;
	struct ln_rst_time rst;
	struct ln_zonedate rise, transit, set;
	
	/* observers location (Edinburgh), used to calc rst */
	observer.lat = 55.92; /* 55.92 N */
	observer.lng = -3.18; /* 3.18 W */
	
	/* get the julian day from the local system time */
	JD = ln_get_julian_from_sys();
	printf ("JD %f\n",JD);
	
	/* get the lunar geopcentric position in km, earth is at 0,0,0 */
	ln_get_lunar_geo_posn (JD, &moon, 0);
	printf ("lunar x %f  y %f  z %f\n",moon.X, moon.Y, moon.Z);
	
	/* Long Lat */
	ln_get_lunar_ecl_coords (JD, &ecl, 0);
	printf ("lunar long %f  lat %f\n",ecl.lng, ecl.lat);
	
	/* RA, DEC */
	ln_get_lunar_equ_coords (JD, &equ);
	printf ("lunar RA %f  Dec %f\n",equ.ra, equ.dec);
	
	/* moon earth distance */
	printf ("lunar distance km %f\n", ln_get_lunar_earth_dist(JD));
	
	/* lunar disk, phase and bright limb */
	printf ("lunar disk %f\n", ln_get_lunar_disk(JD));
	printf ("lunar phase %f\n", ln_get_lunar_phase(JD));
	printf ("lunar bright limb %f\n", ln_get_lunar_bright_limb(JD));
	
	/* rise, set and transit time */
	if (ln_get_lunar_rst (JD, &observer, &rst) == 1) 
		printf ("Moon is circumpolar\n");
	else {
		ln_get_local_date (rst.rise, &rise);
		ln_get_local_date (rst.transit, &transit);
		ln_get_local_date (rst.set, &set);
		print_date ("Rise", &rise);
		print_date ("Transit", &transit);
		print_date ("Set", &set);
	}
	
	/* rise, set and transit time */
	if (ln_get_lunar_rst (JD - 24, &observer, &rst) == 1) 
		printf ("Moon is circumpolar\n");
	else {
		ln_get_local_date (rst.rise, &rise);
		ln_get_local_date (rst.transit, &transit);
		ln_get_local_date (rst.set, &set);
		print_date ("Rise", &rise);
		print_date ("Transit", &transit);
		print_date ("Set", &set);
	}
	
	/* rise, set and transit time */
	if (ln_get_lunar_rst (JD - 25, &observer, &rst) == 1) 
		printf ("Moon is circumpolar\n");
	else {
		ln_get_local_date (rst.rise, &rise);
		ln_get_local_date (rst.transit, &transit);
		ln_get_local_date (rst.set, &set);
		print_date ("Rise", &rise);
		print_date ("Transit", &transit);
		print_date ("Set", &set);
	}
	
	return 0;
}
