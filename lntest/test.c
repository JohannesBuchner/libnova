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


/* libnova test suite -- pretty scrappy at the moment, needs some proper tests */

#include "libnova.h"
#include <stdio.h>
#include <stdlib.h>

double compare_results (double calc, double expect)
{
	if (calc - expect > 0.00000001 || calc - expect < -0.00000001)
		return (calc - expect);
	else
		return (0);
}

int test_result (char * test, double calc, double expect)
{
	double diff;
	
	printf ("TEST %s....", test);
	
	diff = compare_results (calc, expect);
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
void julian_test (void)
{ 
	double JD;
	int wday;
	struct ln_date date, pdate;

	/* Get julian day for 04/10/1957 19:00:00 */
	date.years = 1957;
	date.months = 10;
	date.days = 4; 
	date.hours = 19;
	date.minutes = 0;
	date.seconds = 0;
	JD = get_julian_day (&date);
	test_result ("(Julian Day) JD for 4/10/1957 19:00:00", JD, 2436116.29166667);

	/* Get julian day for 27/01/333 12:00:00 */
	date.years = 333;
	date.months = 1;
	date.days = 27;
	date.hours = 12;
	JD = get_julian_day (&date);
	test_result ("(Julian Day) JD for 27/01/333 12:00:00", JD, 1842713.0);

	/* Get julian day for 30/06/1954 00:00:00 */
	date.years = 1954;
	date.months = 6;
	date.days = 30;
	date.hours = 0;
	JD = get_julian_day (&date);
	test_result ("(Julian Day) JD for 30/06/1954 00:00:00", JD, 2434923.5);
	
	wday = get_day_of_week(&date);
	printf("TEST: (Julian Day) Weekday No is %d",wday);

	get_date (JD, &pdate);
	printf(" for %d/%d/%d  %d:%d:%f\n\n",pdate.days, pdate.months, pdate.years, pdate.hours, pdate.minutes, pdate.seconds);
}

void dynamical_test ()
{
	struct ln_date date;
	double TD,JD;

	/* Dynamical Time test for 01/01/2000 00:00:00 */
	date.years = 2000;
	date.months = 1;
	date.days = 1;
	date.hours = 0;
	date.minutes = 0;
	date.seconds = 0;

	JD = get_julian_day (&date);
	TD = get_jde (JD);
	test_result ("(Dynamical Time) TD for 01/01/2000 00:00:00", TD, 2451544.50065885);
}

void nutation_test (void)
{
	double JDE, JD, nutation_longitude, nutation_obliquity, nutation_ecliptic;
	struct ln_dms hmst;
		
	JD = 2446895.5;
	JDE = get_jde (JD);

	get_nutation (JD, &nutation_longitude, &nutation_obliquity, &nutation_ecliptic);
	test_result ("(Nutation) longitude (deg) for JD 2446895.5", nutation_longitude, -0.00100558);
	
	deg_to_dms (nutation_longitude, &hmst);
	test_result ("(Nutation) obliquity (deg) for JD 2446895.5", nutation_obliquity, 0.00273287);
	
	test_result ("(Nutation) ecliptic (deg) for JD 2446895.5", nutation_ecliptic, 23.44367926);
}

void transform_test(void)
{
	struct lnh_equ_posn hobject, hpollux;
	struct lnh_lnlat_posn hobserver, hecl;
	struct ln_equ_posn object, pollux, equ;
	struct ln_hrz_posn hrz;
	struct ln_lnlat_posn observer, ecl;
	double JD;
	struct ln_date date;

	/* observers position */
	hobserver.lng.degrees = 77;
	hobserver.lng.minutes = 03;
	hobserver.lng.seconds = 56;
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
	test_result ("(Transforms) Equ to Horiz ALT ", hrz.alt, 15.12426264);
	test_result ("(Transforms) Equ to Horiz AZ ", hrz.az, 68.03429264);

	get_equ_from_hrz (&hrz, &observer, JD, &equ);
	test_result ("(Transforms) Horiz to Equ RA ", equ.ra, -12.68072670);
	test_result ("(Transforms) Horiz to Equ DEC", equ.dec, -6.71989167);
	
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
	test_result ("(Transforms) Equ to Ecl longitude ", ecl.lng, 113.21542105);
	test_result ("(Transforms) Equ to Ecl latitude", ecl.lat, 6.68002735);

	get_equ_from_ecl(&ecl, JD, &equ);
	test_result ("(Transforms) Ecl to Equ RA ", equ.ra, 116.32894167);
	test_result ("(Transforms) Ecl to Equ DEC", equ.dec, 28.02618333);
}    

void sidereal_test ()
{
	struct ln_date date;
	double sd;
	double JD;

	/* 10/04/1987 19:21:00 */
	date.years = 1987;
	date.months = 4;
	date.days = 10;
	date.hours = 19;
	date.minutes = 21;
	date.seconds = 0;

	JD = get_julian_day (&date);
	sd = get_mean_sidereal_time (JD);

	test_result ("(Sidereal) mean hours on 10/04/1987 19:21:00 ", sd, 8.58252488);
	sd = get_apparent_sidereal_time (JD);
	test_result ("(Sidereal) apparent hours on 10/04/1987 19:21:00 ", sd, 8.58252060);
}

void solar_coord_test (void)
{
	struct ln_helio_posn pos;

	get_geom_solar_coords (2448908.5, &pos);
	test_result ("(Solar Coords) longitude (deg) on JD 2448908.5  ", pos.L, 200.00669634);
	test_result ("(Solar Coords) latitude (deg) on JD 2448908.5  ", pos.B, 0.00056436);
	test_result ("(Solar Coords) radius vector (AU) on JD 2448908.5  ", pos.R, 0.99760852);
}

void aberration_test (void)
{
	struct lnh_equ_posn hobject;
	struct ln_equ_posn object, pos;
	struct ln_date date;
	double JD;

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
	test_result ("(Aberration) RA  ", pos.ra, 41.55557139);
	test_result ("(Aberration) DEC  ", pos.dec, 49.35032196);
}

void precession_test(void)
{
	double JD;
	struct ln_equ_posn object, pos;
	struct lnh_equ_posn hobject;

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
	test_result ("(Precession) RA on JD 2462088.69  ", pos.ra, 41.54306131);
	test_result ("(Precession) DEC on JD 2462088.69  ", pos.dec, 49.34921505);
}

void apparent_position_test(void)
{
	double JD;
	struct lnh_equ_posn hobject, hpm;
	struct ln_equ_posn object, pm, pos;	

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
	
	test_result ("(Apparent Position) RA on JD 2462088.69  ", pos.ra, 41.55553862);
	test_result ("(Apparent Position) DEC on JD 2462088.69  ", pos.dec, 49.35034910);
}

void vsop87_test(void)
{
	struct ln_helio_posn pos;
	struct lnh_equ_posn hequ;
	struct ln_equ_posn equ;
	double JD = 2448976.5;
	double au;
#if 0
	struct ln_date date, pdate;
/*	JD = get_julian_from_sys();	*/
	date.years = 1992;
	date.months = 12;
	date.days = 20; 
	date.hours = 0;
	date.minutes = 0;
	date.seconds = 0;

	JD = get_julian_day (&date);
#endif
	
	get_equ_solar_coords (JD, &equ);
	test_result ("(Solar Position) RA on JD 2448976.5  ", equ.ra, 268.31990965);
	test_result ("(Solar Position) DEC on JD 2448976.5  ", equ.dec, -23.43182047);
	
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
}

int lunar_test ()
{
	double JD = 2448724.5;
	
	struct ln_rect_posn moon;
	struct ln_equ_posn equ;
	struct ln_lnlat_posn ecl;
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
	return 0;
}

int ecliptic_motion_test ()
{
	double r,v;
	double E, e_JD, o_JD;
	struct ln_orbit orbit;
	struct ln_rect_posn posn;
	struct ln_date epoch_date, obs_date;
	struct ln_equ_posn equ_posn;
		
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
	test_result ("(Equation of kepler) E when e is 0.1 and M is 5.0   ", E, 5.554589253872320);
	
	v = get_true_anomaly (0.1, E);
	test_result ("(True Anomaly) v when e is 0.1 and E is 5.5545   ", v, 5.554589253872320);
	
	r = get_radius_vector (0.5, 0.1, E);
	test_result ("(Radius Vector) r when v is , e is 0.1 and E is 5.5545   ", r, 5.554589253872320);
	
	get_geo_rect_posn (&orbit, o_JD, &posn);
	printf ("X %0.15f Y %0.15f Z %0.15f\n",posn.X, posn.Y, posn.Z);
	test_result ("(Geocentric Rect Coords X) for comet Enckle   ", posn.X, 5.554589253872320);
	test_result ("(Geocentric Rect Coords Y) for comet Enckle   ", posn.Y, 5.554589253872320);
	test_result ("(Geocentric Rect Coords Z) for comet Enckle   ", posn.Z, 5.554589253872320);
	
	get_body_equ_coords (o_JD, &orbit, &equ_posn);
	printf (" RA %0.15f Dec %0.15f\n",equ_posn.ra, equ_posn.dec);
}

int main ()
{
	julian_test();
	dynamical_test();
	sidereal_test();
	nutation_test();
	transform_test();
	solar_coord_test ();
	aberration_test();
	precession_test();
	apparent_position_test ();
	vsop87_test();
	lunar_test ();
	ecliptic_motion_test();
	return (0);
}
