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

Copyright 2000 Liam Girdwood  */


/*
 * libnova test suite -- pretty scrappy at the moment, needs some proper tests 
 * We need tons of real data to test against. I've only tested against the examples
 * in the Meeus book and some data from the MPC.
 */

#include "libnova.h"
#include <stdio.h>
#include <stdlib.h>

double compare_results (double calc, double expect, double tolerance)
{
	if (calc - expect > tolerance || calc - expect < (tolerance * -1.0))
		return (calc - expect);
	else
		return (0);
}

int test_result (char * test, double calc, double expect, double tolerance)
{
	double diff;
	
	printf ("TEST %s....", test);
	
	diff = compare_results (calc, expect, tolerance);
	if (diff)
	{
		printf ("[FAILED]\n");
		printf ("	Expected %8.8f but calculated %8.8f %0.12f error.\n\n", expect, calc, diff);
		return 1;
	}
	else
	{
		printf ("[PASSED]\n");
		printf ("	Expected and calculated %8.8f.\n\n", calc);
		return 0;
	}
}

/* test julian day calculations */
int julian_test (void)
{ 
	double JD;
	int wday, failed = 0;
	struct ln_date date, pdate;

	/* Get julian day for 04/10/1957 19:00:00 */
	date.years = 1957;
	date.months = 10;
	date.days = 4; 
	date.hours = 19;
	date.minutes = 0;
	date.seconds = 0;
	JD = get_julian_day (&date);
	failed += test_result ("(Julian Day) JD for 4/10/1957 19:00:00", JD, 2436116.29166667, 0.00001);

	/* Get julian day for 27/01/333 12:00:00 */
	date.years = 333;
	date.months = 1;
	date.days = 27;
	date.hours = 12;
	JD = get_julian_day (&date);
	failed += test_result ("(Julian Day) JD for 27/01/333 12:00:00", JD, 1842713.0, 0.1);

	/* Get julian day for 30/06/1954 00:00:00 */
	date.years = 1954;
	date.months = 6;
	date.days = 30;
	date.hours = 0;
	JD = get_julian_day (&date);
	failed += test_result ("(Julian Day) JD for 30/06/1954 00:00:00", JD, 2434923.5, 0.1);
	
	wday = get_day_of_week(&date);
	printf("TEST: (Julian Day) Weekday No is %d",wday);

	get_date (JD, &pdate);
	printf(" for %d/%d/%d  %d:%d:%f\n\n",pdate.days, pdate.months, pdate.years, pdate.hours, pdate.minutes, pdate.seconds);

	return (failed);
}

int dynamical_test ()
{
	struct ln_date date;
	double TD,JD;
	int failed = 0;

	/* Dynamical Time test for 01/01/2000 00:00:00 */
	date.years = 2000;
	date.months = 1;
	date.days = 1;
	date.hours = 0;
	date.minutes = 0;
	date.seconds = 0;

	JD = get_julian_day (&date);
	TD = get_jde (JD);
	failed += test_result ("(Dynamical Time) TD for 01/01/2000 00:00:00", TD, 2451544.50065885, 0.000001);
	return (failed);
}

int nutation_test (void)
{
	double JDE, JD;
	struct ln_nutation nutation;
	int failed = 0;
		
	JD = 2446895.5;
	JDE = get_jde (JD);

	get_nutation (JD, &nutation);
	failed += test_result ("(Nutation) longitude (deg) for JD 2446895.5", nutation.longitude, -0.00100561, 0.00000001);
	
	failed += test_result ("(Nutation) obliquity (deg) for JD 2446895.5", nutation.obliquity, 0.00273297, 0.00000001);
	
	failed += test_result ("(Nutation) ecliptic (deg) for JD 2446895.5", nutation.ecliptic, 23.44367936, 0.00000001);
	return (failed);
}

int transform_test(void)
{
	struct lnh_equ_posn hobject, hpollux;
	struct lnh_lnlat_posn hobserver, hecl;
	struct ln_equ_posn object, pollux, equ;
	struct ln_hrz_posn hrz;
	struct ln_lnlat_posn observer, ecl;
	double JD;
	struct ln_date date;
	int failed = 0;
	
	/* observers position */
	hobserver.lng.degrees = 282;
	hobserver.lng.minutes = 56;
	hobserver.lng.seconds = 4;
	hobserver.lat.degrees = 38;
	hobserver.lat.minutes = 55;
	hobserver.lat.seconds = 17;

	/* object position */
	hobject.ra.hours = 23;
	hobject.ra.minutes = 9;
	hobject.ra.seconds = 16.641;
	hobject.dec.degrees = -6;
	hobject.dec.minutes = -43;
	hobject.dec.seconds = -11.61;

	/* date and time */
	date.years = 1987;
	date.months = 4;
	date.days = 10;
	date.hours = 19;
	date.minutes = 21;
	date.seconds = 0;

	JD = get_julian_day (&date);
	hequ_to_equ (&hobject, &object);
	hlnlat_to_lnlat (&hobserver, &observer);
	
	get_hrz_from_equ (&object, &observer, JD, &hrz);
	failed += test_result ("(Transforms) Equ to Horiz ALT ", hrz.alt, 15.12426274, 0.00000001);
	failed += test_result ("(Transforms) Equ to Horiz AZ ", hrz.az, 68.03429264, 0.00000001);

	get_equ_from_hrz (&hrz, &observer, JD, &equ);
	failed += test_result ("(Transforms) Horiz to Equ RA ", equ.ra, 347.31926752, 0.00000001);
	failed += test_result ("(Transforms) Horiz to Equ DEC", equ.dec, -6.71989167, 0.00000001);
	
	/* Equ position of Pollux */
	hpollux.ra.hours = 7;
	hpollux.ra.minutes = 45;
	hpollux.ra.seconds = 18.946;
	hpollux.dec.degrees = 28;
	hpollux.dec.minutes = 1;
	hpollux.dec.seconds = 34.26;

	hequ_to_equ (&hpollux, &pollux);
	get_ecl_from_equ(&pollux, JD, &ecl);
	
	lnlat_to_hlnlat (&ecl, &hecl);
	failed += test_result ("(Transforms) Equ to Ecl longitude ", ecl.lng, 113.21542105, 0.00000001);
	failed += test_result ("(Transforms) Equ to Ecl latitude", ecl.lat, 6.68002727, 0.00000001);

	get_equ_from_ecl(&ecl, JD, &equ);
	failed += test_result ("(Transforms) Ecl to Equ RA ", equ.ra, 116.32894167, 0.00000001);
	failed += test_result ("(Transforms) Ecl to Equ DEC", equ.dec, 28.02618333, 0.00000001);
	
	return (failed);
}    

int sidereal_test ()
{
	struct ln_date date;
	double sd;
	double JD;
	int failed = 0;
	
	/* 10/04/1987 19:21:00 */
	date.years = 1987;
	date.months = 4;
	date.days = 10;
	date.hours = 19;
	date.minutes = 21;
	date.seconds = 0;

	JD = get_julian_day (&date);
	sd = get_mean_sidereal_time (JD);

	failed += test_result ("(Sidereal) mean hours on 10/04/1987 19:21:00 ", sd, 8.58252488, 0.000001);
	sd = get_apparent_sidereal_time (JD);
	failed += test_result ("(Sidereal) apparent hours on 10/04/1987 19:21:00 ", sd, 8.58252060, 0.000001);
	return (failed);
}

int solar_coord_test (void)
{
	struct ln_helio_posn pos;
	int failed = 0;
	
	get_geom_solar_coords (2448908.5, &pos);
	failed += test_result ("(Solar Coords) longitude (deg) on JD 2448908.5  ", pos.L, 200.00810889, 0.00000001);
	failed += test_result ("(Solar Coords) latitude (deg) on JD 2448908.5  ", pos.B, 0.00018690, 0.00000001);
	failed += test_result ("(Solar Coords) radius vector (AU) on JD 2448908.5  ", pos.R, 0.99760852, 0.00000001);
	return (failed);
}

int aberration_test (void)
{
	struct lnh_equ_posn hobject;
	struct ln_equ_posn object, pos;
	struct ln_date date;
	double JD;
	int failed = 0;
	
	/* object position */
	hobject.ra.hours = 2;
	hobject.ra.minutes = 46;
	hobject.ra.seconds = 11.331;
	hobject.dec.degrees = 49;
	hobject.dec.minutes = 20;
	hobject.dec.seconds = 54.54;

	/* date */
	date.years = 2028;
	date.months = 11;
	date.days = 13;
	date.hours = 4;
	date.minutes = 31;
	date.seconds = 0;

	JD = get_julian_day (&date);

	hequ_to_equ (&hobject, &object);
	get_equ_aber (&object, JD, &pos);
	failed += test_result ("(Aberration) RA  ", pos.ra, 41.55557139, 0.00000001);
	failed += test_result ("(Aberration) DEC  ", pos.dec, 49.35032196, 0.00000001);
	return (failed);
}

int precession_test(void)
{
	double JD;
	struct ln_equ_posn object, pos;
	struct lnh_equ_posn hobject;
	int failed = 0;
	
	/* object position */
	hobject.ra.hours = 2;
	hobject.ra.minutes = 44;
	hobject.ra.seconds = 11.986;
	hobject.dec.degrees = 49;
	hobject.dec.minutes = 13;
	hobject.dec.seconds = 42.48;

	JD = 2462088.69;
	hequ_to_equ (&hobject, &object);
	get_equ_prec (&object, JD, &pos);
	failed += test_result ("(Precession) RA on JD 2462088.69  ", pos.ra, 41.54306131, 0.00000001);
	failed += test_result ("(Precession) DEC on JD 2462088.69  ", pos.dec, 49.34921505, 0.00000001);
	return (failed);
}

int apparent_position_test(void)
{
	double JD;
	struct lnh_equ_posn hobject, hpm;
	struct ln_equ_posn object, pm, pos;	
	int failed = 0;
	
	/* objects position */
	hobject.ra.hours = 2;
	hobject.ra.minutes = 44;
	hobject.ra.seconds = 11.986;
	hobject.dec.degrees = 49;
	hobject.dec.minutes = 13;
	hobject.dec.seconds = 42.48;

	/* proper motion of object */
	hpm.ra.hours = 0;
	hpm.ra.minutes = 0;
	hpm.ra.seconds = 0.03425;
	hpm.dec.degrees = 0;
	hpm.dec.minutes = 0;
	hpm.dec.seconds = -0.0895;
    
	JD = 2462088.69;
	hequ_to_equ (&hobject, &object);
	hequ_to_equ (&hpm, &pm);
	get_apparent_posn (&object, &pm, JD, &pos);
	
	failed += test_result ("(Apparent Position) RA on JD 2462088.69  ", pos.ra, 41.55553862, 0.00000001);
	failed += test_result ("(Apparent Position) DEC on JD 2462088.69  ", pos.dec, 49.35034910, 0.00000001);
	return (failed);
}

int vsop87_test(void)
{
	struct ln_helio_posn pos;
	struct lnh_equ_posn hequ;
	struct ln_equ_posn equ;
	double JD = 2448976.5;
	double au;
	int failed = 0;
	
#ifdef DATE
	struct ln_date date, pdate;
	date.years = 2003;
	date.months = 1;
	date.days = 29; 
	date.hours = 0;
	date.minutes = 0;
	date.seconds = 0;

	JD = get_julian_day (&date);
#endif
#ifdef SYS_TIME
	JD = get_julian_from_sys();	
#endif
	
	get_equ_solar_coords (JD, &equ);
	failed += test_result ("(Solar Position) RA on JD 2448976.5  ", equ.ra, 268.32146893, 0.00000001);
	failed += test_result ("(Solar Position) DEC on JD 2448976.5  ", equ.dec, -23.43026873, 0.00000001);
	
	get_mercury_helio_coords(JD, &pos);
	printf("Mercury L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_mercury_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Mercury RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_mercury_earth_dist (JD);
	printf ("mercury -> Earth dist (AU) %f\n",au);
	au = get_mercury_sun_dist (JD);
	printf ("mercury -> Sun dist (AU) %f\n",au);
	au = get_mercury_disk (JD);
	printf ("mercury -> illuminated disk %f\n",au);
	au = get_mercury_magnitude (JD);
	printf ("mercury -> magnitude %f\n",au);
	au = get_mercury_phase (JD);
	printf ("mercury -> phase %f\n",au);
	
	get_venus_helio_coords(JD, &pos);
	printf("Venus L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_venus_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Venus RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_venus_earth_dist (JD);
	printf ("venus -> Earth dist (AU) %f\n",au);
	au = get_venus_sun_dist (JD);
	printf ("venus -> Sun dist (AU) %f\n",au);
	au = get_venus_disk (JD);
	printf ("venus -> illuminated disk %f\n",au);
	au = get_venus_magnitude (JD);
	printf ("venus -> magnitude %f\n",au);
	au = get_venus_phase (JD);
	printf ("venus -> phase %f\n",au);
	
	get_earth_helio_coords(JD, &pos);
	printf("Earth L %f B %f R %f\n", pos.L, pos.B, pos.R);
	au = get_earth_sun_dist (JD);
	printf ("earth -> Sun dist (AU) %f\n",au);

	get_mars_helio_coords(JD, &pos);	
	printf("Mars L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_mars_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Mars RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_mars_earth_dist (JD);
	printf ("mars -> Earth dist (AU) %f\n",au);
	au = get_mars_sun_dist (JD);
	printf ("mars -> Sun dist (AU) %f\n",au);
	au = get_mars_disk (JD);
	printf ("mars -> illuminated disk %f\n",au);
	au = get_mars_magnitude (JD);
	printf ("mars -> magnitude %f\n",au);
	au = get_mars_phase (JD);
	printf ("mars -> phase %f\n",au);
	
	get_jupiter_helio_coords(JD, &pos);
	printf("Jupiter L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_jupiter_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Jupiter RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_jupiter_earth_dist (JD);
	printf ("jupiter -> Earth dist (AU) %f\n",au);
	au = get_jupiter_sun_dist (JD);
	printf ("jupiter -> Sun dist (AU) %f\n",au);
	au = get_jupiter_disk (JD);
	printf ("jupiter -> illuminated disk %f\n",au);
	au = get_jupiter_magnitude (JD);
	printf ("jupiter -> magnitude %f\n",au);
	au = get_jupiter_phase (JD);
	printf ("jupiter -> phase %f\n",au);
	
	get_saturn_helio_coords(JD, &pos);
	printf("Saturn L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_saturn_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Saturn RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_saturn_earth_dist (JD);
	printf ("saturn -> Earth dist (AU) %f\n",au);
	au = get_saturn_sun_dist (JD);
	printf ("saturn -> Sun dist (AU) %f\n",au);
	au = get_saturn_disk (JD);
	printf ("saturn -> illuminated disk %f\n",au);
	au = get_saturn_magnitude (JD);
	printf ("saturn -> magnitude %f\n",au);
	au = get_saturn_phase (JD);
	printf ("saturn -> phase %f\n",au);
	
	get_uranus_helio_coords(JD, &pos);	
	printf("Uranus L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_uranus_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Uranus RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_uranus_earth_dist (JD);
	printf ("uranus -> Earth dist (AU) %f\n",au);
	au = get_uranus_sun_dist (JD);
	printf ("uranus -> Sun dist (AU) %f\n",au);
	au = get_uranus_disk (JD);
	printf ("uranus -> illuminated disk %f\n",au);
	au = get_uranus_magnitude (JD);
	printf ("uranus -> magnitude %f\n",au);
	au = get_uranus_phase (JD);
	printf ("uranus -> phase %f\n",au);
	
	get_neptune_helio_coords(JD, &pos);
	printf("Neptune L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_neptune_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Neptune RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_neptune_earth_dist (JD);
	printf ("neptune -> Earth dist (AU) %f\n",au);
	au = get_neptune_sun_dist (JD);
	printf ("neptune -> Sun dist (AU) %f\n",au);
	au = get_neptune_disk (JD);
	printf ("neptune -> illuminated disk %f\n",au);
	au = get_neptune_magnitude (JD);
	printf ("neptune -> magnitude %f\n",au);
	au = get_neptune_phase (JD);
	printf ("neptune -> phase %f\n",au);
	
	get_pluto_helio_coords(JD, &pos);
	printf("Pluto L %f B %f R %f\n", pos.L, pos.B, pos.R);
	get_pluto_equ_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Pluto RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	au = get_pluto_earth_dist (JD);
	printf ("pluto -> Earth dist (AU) %f\n",au);
	au = get_pluto_sun_dist (JD);
	printf ("pluto -> Sun dist (AU) %f\n",au);
	au = get_pluto_disk (JD);
	printf ("pluto -> illuminated disk %f\n",au);
	au = get_pluto_magnitude (JD);
	printf ("pluto -> magnitude %f\n",au);
	au = get_pluto_phase (JD);
	printf ("pluto -> phase %f\n",au);
	
	return (failed);
}

int lunar_test ()
{
	double JD = 2448724.5;
	
	struct ln_rect_posn moon;
	struct ln_equ_posn equ;
	struct ln_lnlat_posn ecl;
	int failed = 0;
	
	/*	JD = get_julian_from_sys();*/
	/*JD=2448724.5;*/
	get_lunar_geo_posn (JD, &moon, 0);
	printf ("lunar x %f  y %f  z %f\n",moon.X, moon.Y, moon.Z);
	get_lunar_ecl_coords (JD, &ecl, 0);
	printf ("lunar long %f  lat %f\n",ecl.lng, ecl.lat);
	get_lunar_equ_coords (JD, &equ, 0);
	printf ("lunar RA %f  Dec %f\n",equ.ra, equ.dec);
	printf ("lunar distance km %f\n", get_lunar_earth_dist(JD));
	printf ("lunar disk %f\n", get_lunar_disk(JD));
	printf ("lunar phase %f\n", get_lunar_phase(JD));
	printf ("lunar bright limb %f\n", get_lunar_bright_limb(JD));
	return (failed);
}

int elliptic_motion_test ()
{
	double r,v,l,V,dist;
	double E, e_JD, o_JD;
	struct ln_ell_orbit orbit;
	struct ln_rect_posn posn;
	struct ln_date epoch_date, obs_date;
	struct ln_equ_posn equ_posn;
	int failed = 0;
		
	obs_date.years = 1990;
	obs_date.months = 10;
	obs_date.days = 6;
	obs_date.hours = 0;
	obs_date.minutes = 0;
	obs_date.seconds = 0;
		
	epoch_date.years = 1990;
	epoch_date.months = 10;
	epoch_date.days = 28;
	epoch_date.hours = 12;
	epoch_date.minutes = 30;
	epoch_date.seconds = 0;
	
	e_JD = get_julian_day (&epoch_date);
	o_JD = get_julian_day (&obs_date);
	
	orbit.JD = e_JD;
	orbit.a = 2.2091404;
	orbit.e = 0.8502196;
	orbit.i = 11.94525;
	orbit.omega = 334.75006;
	orbit.w = 186.23352;
	orbit.n = 0;
	
	E = solve_kepler (0.1, 5.0);
	failed += test_result ("(Equation of kepler) E when e is 0.1 and M is 5.0   ", E, 5.554589253872320, 0.000000000001);
	
	v = get_ell_true_anomaly (0.1, E);
	failed += test_result ("(True Anomaly) v when e is 0.1 and E is 5.5545   ", v, 6.13976152, 0.00000001);
	
	r = get_ell_radius_vector (0.5, 0.1, E);
	failed += test_result ("(Radius Vector) r when v is , e is 0.1 and E is 5.5545   ", r, 0.45023478, 0.00000001);
	
	get_ell_geo_rect_posn (&orbit, o_JD, &posn);
	failed += test_result ("(Geocentric Rect Coords X) for comet Enckle   ", posn.X, 0.72549850, 0.00000001);
	failed += test_result ("(Geocentric Rect Coords Y) for comet Enckle   ", posn.Y, -0.28443537, 0.00000001);
	failed += test_result ("(Geocentric Rect Coords Z) for comet Enckle   ", posn.Z, -0.27031656, 0.00000001);
	
	get_ell_helio_rect_posn (&orbit, o_JD, &posn);
	failed += test_result ("(Heliocentric Rect Coords X) for comet Enckle   ", posn.X, 0.25017473, 0.00000001);
	failed += test_result ("(Heliocentric Rect Coords Y) for comet Enckle   ", posn.Y, 0.48476422, 0.00000001);
	failed += test_result ("(Heliocentric Rect Coords Z) for comet Enckle   ", posn.Z, 0.35716517, 0.00000001);
	
	get_ell_body_equ_coords (o_JD, &orbit, &equ_posn);
	failed += test_result ("(RA) for comet Enckle   ", equ_posn.ra, 158.58242653, 0.00000001);
	failed += test_result ("(Dec) for comet Enckle   ", equ_posn.dec, 19.13924815, 0.00000001);
	
	l = get_ell_orbit_len (&orbit);
	failed += test_result ("(Orbit Length) for comet Enckle in AU   ", l, 10.75710334, 0.00000001);
	
	V = get_ell_orbit_pvel (&orbit);
	failed += test_result ("(Orbit Perihelion Vel) for comet Enckle in kms   ", V, 70.43130198, 0.00000001);
	
	V = get_ell_orbit_avel (&orbit);
	failed += test_result ("(Orbit Aphelion Vel) for comet Enckle in kms   ", V, 5.70160892, 0.00000001);
	
	V = get_ell_orbit_vel (o_JD, &orbit);
	failed += test_result ("(Orbit Vel JD) for comet Enckle in kms   ", V, 48.16148331, 0.00000001);
	
	dist = get_ell_body_solar_dist (o_JD, &orbit);
	failed += test_result ("(Body Solar Dist) for comet Enckle in AU   ", dist, 0.65203581, 0.00000001);
	
	dist = get_ell_body_earth_dist (o_JD, &orbit);
	failed += test_result ("(Body Earth Dist) for comet Enckle in AU   ", dist, 0.82481670, 0.00000001);
	return (failed);
}

/* need a proper parabolic orbit to properly test */
int parabolic_motion_test ()
{ 
	double r,v,dist;
	double e_JD, o_JD;
	struct ln_par_orbit orbit;
	struct ln_rect_posn posn;
	struct ln_date epoch_date, obs_date;
	struct ln_equ_posn equ_posn;
	int failed = 0;
		
	obs_date.years = 2003;
	obs_date.months = 1;
	obs_date.days = 11;
	obs_date.hours = 0;
	obs_date.minutes = 0;
	obs_date.seconds = 0;
		
	epoch_date.years = 2003;
	epoch_date.months = 1;
	epoch_date.days = 29;
	epoch_date.hours = 0;
	epoch_date.minutes = 6;
	epoch_date.seconds = 37.44;
	
	e_JD = get_julian_day (&epoch_date);
	o_JD = get_julian_day (&obs_date);
	
	orbit.q = 0.190082; 
	orbit.i = 94.1511;
	orbit.w = 187.5613; 
	orbit.omega = 119.0676; 
	orbit.JD = e_JD;
	
	v = get_par_true_anomaly (orbit.q, o_JD - e_JD);
	failed += test_result ("(True Anomaly) v when e is 0.1 and E is 5.5545   ", v, 247.18968605, 0.00000001);
	
	r = get_par_radius_vector (orbit.q, o_JD - e_JD);
	failed += test_result ("(Radius Vector) r when v is , e is 0.1 and E is 5.5545   ", r, 0.62085992, 0.00000001);
	
	get_par_geo_rect_posn (&orbit, o_JD, &posn);
	failed += test_result ("(Geocentric Rect Coords X) for comet C/2002 X5 (Kudo-Fujikawa)   ", posn.X, 0.29972461, 0.00000001);
	failed += test_result ("(Geocentric Rect Coords Y) for comet C/2002 X5 (Kudo-Fujikawa)   ", posn.Y, -0.93359772, 0.00000001);
	failed += test_result ("(Geocentric Rect Coords Z) for comet C/2002 X5 (Kudo-Fujikawa)   ", posn.Z, 0.24639194, 0.00000001);
	
	get_par_helio_rect_posn (&orbit, o_JD, &posn);
	failed += test_result ("(Heliocentric Rect Coords X) for comet C/2002 X5 (Kudo-Fujikawa)   ", posn.X, -0.04143700, 0.00000001);
	failed += test_result ("(Heliocentric Rect Coords Y) for comet C/2002 X5 (Kudo-Fujikawa)   ", posn.Y, -0.08736588, 0.00000001);
	failed += test_result ("(Heliocentric Rect Coords Z) for comet C/2002 X5 (Kudo-Fujikawa)   ", posn.Z, 0.61328397, 0.00000001);
	
	get_par_body_equ_coords (o_JD, &orbit, &equ_posn);
	failed += test_result ("(RA) for comet C/2002 X5 (Kudo-Fujikawa)   ", equ_posn.ra, 287.79617309, 0.00000001);
	failed += test_result ("(Dec) for comet C/2002 X5 (Kudo-Fujikawa)   ", equ_posn.dec, 14.11800859, 0.00000001);
	
	dist = get_par_body_solar_dist (o_JD, &orbit);
	failed += test_result ("(Body Solar Dist) for comet C/2002 X5 (Kudo-Fujikawa) in AU   ", dist, 0.62085992, 0.00001);
	
	dist = get_par_body_earth_dist (o_JD, &orbit);
	failed += test_result ("(Body Earth Dist) for comet C/2002 X5 (Kudo-Fujikawa) in AU   ", dist, 1.01101362, 0.00001);
	return (failed);
}

int rst_test ()
{
	struct ln_lnlat_posn observer;
	struct ln_rst_time time;
	int failed = 0;
	double JD = get_julian_from_sys ();
	
	observer.lng = 3;
	observer.lat = 51;
	
	get_solar_rst (JD, &observer, &time);
	return (failed);
}

int angular_test ()
{
	int failed = 0;
	double d;
	struct ln_equ_posn posn1, posn2;
		
	/* alpha Bootes (Arcturus) */
	posn1.ra = 213.9154;
	posn1.dec = 19.1825;
	
	/* alpha Virgo (Spica) */
	posn2.ra = 201.2983;
	posn2.dec = -11.1614;
	
	d = get_angular_separation(&posn1, &posn2);
	failed += test_result ("(Angular) Separation of Arcturus and Spica   ", d, 32.79302684, 0.00001);
	
	d = get_rel_posn_angle(&posn1, &posn2);
	failed += test_result ("(Angular) Position Angle of Arcturus and Spica   ", d, 22.39042787, 0.00001);
	
	return (failed);
}

int main ()
{
	int failed = 0;
	
	failed += julian_test();
	failed += dynamical_test();
	failed += sidereal_test();
	failed += nutation_test();
	failed += transform_test();
	failed += solar_coord_test ();
	failed += aberration_test();
	failed += precession_test();
	failed += apparent_position_test ();
	failed += vsop87_test();
	failed += lunar_test ();
	failed += elliptic_motion_test();
	failed += parabolic_motion_test ();
	failed += rst_test ();
	failed += angular_test();
	
	printf ("Test completed: %d errors.\n",failed);
		
	return (0);
}
