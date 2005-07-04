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


A simple example showing some comet calculations.

Comet Enckle

*/

#include <stdio.h>
#include <libnova/comet.h>
#include <libnova/julian_day.h>
#include <libnova/rise_set.h>
#include <libnova/transform.h>
#include <libnova/elliptic_motion.h>

void print_date (char * title, struct ln_zonedate* date)
{
	printf ("\n%s\n",title);
	printf (" Year    : %d\n", date->years);
	printf (" Month   : %d\n", date->months);
	printf (" Day     : %d\n", date->days);
	printf (" Hours   : %d\n", date->hours);
	printf (" Minutes : %d\n", date->minutes);
	printf (" Seconds : %f\n", date->seconds);
	printf("gmtoff %ld\n", date->gmtoff);
}

int main (int argc, char * argv[])
{
	struct ln_equ_posn equ;
	struct ln_rst_time rst;
	struct ln_zonedate rise, set, transit;
	struct ln_date epoch_date;
	struct ln_lnlat_posn observer;
	struct ln_ell_orbit orbit;
	struct ln_rect_posn posn;
	double JD, e_JD;
	double E, v, V, r, l, dist;
	
	/* observers location (Edinburgh), used to calc rst */
	observer.lat = 55.92; /* 55.92 N */
	observer.lng = -3.18; /* 3.18 W */
	
	/* get Julian day from local time */
	JD = ln_get_julian_from_sys();	
	printf ("JD %f\n", JD);
	
	/* calc epoch JD */
	epoch_date.years = 1990;
	epoch_date.months = 10;
	epoch_date.days = 28;
	epoch_date.hours = 12;
	epoch_date.minutes = 30;
	epoch_date.seconds = 0;
	e_JD = ln_get_julian_day (&epoch_date);
	
	/* Enckle orbital elements */
	orbit.JD = e_JD;
	orbit.a = 2.2091404;
	orbit.e = 0.8502196;
	orbit.i = 11.94525;
	orbit.omega = 334.75006;
	orbit.w = 186.23352;
	orbit.n = 0;
	
	/* solve kepler for orbit */
	E = ln_solve_kepler (0.1, 5.0);
	printf("(Equation of kepler) E when e is 0.1 and M is 5.0  %f\n ", E);
	
	/* true anomaly */
	v = ln_get_ell_true_anomaly (0.1, E);
	printf("(True Anomaly) v when e is 0.1 and E is 5.5545  %f\n ", v);
	
	/* radius vector */
	r = ln_get_ell_radius_vector (0.5, 0.1, E);
	printf ("(Radius Vector) r when v is , e is 0.1 and E is 5.5545  %f\n ", r);
	
	/* geocentric rect coords */
	ln_get_ell_geo_rect_posn (&orbit, JD, &posn);
	printf ("(Geocentric Rect Coords X) for comet Enckle  %f\n", posn.X);
	printf ("(Geocentric Rect Coords Y) for comet Enckle  %f\n", posn.Y);
	printf ("(Geocentric Rect Coords Z) for comet Enckle  %f\n", posn.Z);
	
	/* rectangular coords */
	ln_get_ell_helio_rect_posn (&orbit, JD, &posn);
	printf ("(Heliocentric Rect Coords X) for comet Enckle  %f\n ", posn.X);
	printf ("(Heliocentric Rect Coords Y) for comet Enckle  %f\n ", posn.Y);
	printf ("(Heliocentric Rect Coords Z) for comet Enckle  %f\n ", posn.Z);
	
	/* ra, dec */
	ln_get_ell_body_equ_coords (JD, &orbit, &equ);
	printf ("(RA) for comet Enckle  %f\n ", equ.ra);
	printf ("(Dec) for comet Enckle  %f\n ", equ.dec);
	
	/* orbit length */
	l = ln_get_ell_orbit_len (&orbit);
	printf ("(Orbit Length) for comet Enckle in AU  %f\n ", l);
	
	/* orbital velocity at perihelion */
	V = ln_get_ell_orbit_pvel (&orbit);
	printf ("(Orbit Perihelion Vel) for comet Enckle in kms  %f\n ", V);
	
	/* orbital velocity at aphelion */
	V = ln_get_ell_orbit_avel (&orbit);
	printf ("(Orbit Aphelion Vel) for comet Enckle in kms  %f\n ", V);
	
	/* average orbital velocity */
	V = ln_get_ell_orbit_vel (JD, &orbit);
	printf ("(Orbit Vel JD) for comet Enckle in kms  %f\n ", V);
	
	/* comet sun distance */
	dist = ln_get_ell_body_solar_dist (JD, &orbit);
	printf ("(Body Solar Dist) for comet Enckle in AU  %f\n ", dist);
	
	/* comet earth distance */
	dist = ln_get_ell_body_earth_dist (JD, &orbit);
	printf ("(Body Earth Dist) for comet Enckle in AU  %f\n ", dist);
	
	/* rise, set and transit */
	if (ln_get_ell_body_rst (JD, &observer, &orbit, &rst) == 1) 
		printf ("Comet is circumpolar\n");
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
