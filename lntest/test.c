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
	if (calc != expect)
		return ( 1.0 / (expect / (calc - expect)));
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
		printf ("	Expected %8.8f but got %8.8f %f %% error.\n", expect, calc, diff);
		return 1;
	}
	else
	{
		printf ("[OK]\n");
		return 0;
	}
}

int julian_test (void)
{ 
	double JD;
	int wday;
	struct ln_date date, pdate;

	date.years = 1957;
	date.months = 10;
	date.days = 4; 
	date.hours = 19;
	date.minutes = 0;
	date.seconds = 0;
	JD = get_julian_day (&date);
	test_result ("JD for 4/10/1957 19:00:00", JD, 2456789.0);

	date.years = 333;
	date.months = 1;
	date.days = 27;
	date.hours = 12;

	JD = get_julian_day (&date);

	printf("JD is %f \n",JD);  

	date.years = 1954;
	date.months = 6;
	date.days = 30;
	date.hours = 0;

	JD = get_julian_day (&date);

	printf("JD is %f \n",JD); 
	wday = get_day_of_week(&date);
	printf("Weekday No is %d\n",wday);

	JD = 2436116.31;
	get_date (JD, &pdate);
	printf("%d:%d:%d:%d:%d:%f\n",pdate.years, pdate.months, pdate.days, pdate.hours, pdate.minutes, pdate.seconds);
}

void dynamical_test ()
{
	struct ln_date date;
	double TD,JD;

	printf("\n\nDynamical time test.....\n");

	date.years = 2000;
	date.months = 0;
	date.days = 0;
	date.hours = 0;
	date.minutes = 0;
	date.seconds = 0;

	JD = get_julian_day (&date);
	printf("JD is %f\n",JD);

	TD = get_jde (JD);
	printf("JDE is %f\n",TD);
}

void nutation_test (void)
{
	double JDE, JD, nutation_longitude, nutation_obliquity, nutation_ecliptic;
	struct ln_dms hmst;
		
	printf("\n\nNutation test.....\n");
	JD = 2446895.5;

	JDE = get_jde (JD);
	printf("JDE is %f\n",JDE);

	get_nutation (JD, &nutation_longitude, &nutation_obliquity, &nutation_ecliptic);
	printf("Nutation in longitude (deg) %f \n",nutation_longitude);
	deg_to_dms (nutation_longitude, &hmst);
	printf("                      H:M:S %d:%d:%12.12f\n", hmst.degrees, hmst.minutes, hmst.seconds);
	printf("Nutation in obliquity (deg) %f \n",nutation_obliquity);
	deg_to_dms (nutation_obliquity, &hmst);
	printf("                      H:M:S %d:%d:%12.12f\n", hmst.degrees, hmst.minutes, hmst.seconds);
	printf("Nutation in ecliptic (deg) %f \n",nutation_ecliptic);
	deg_to_dms (nutation_ecliptic, &hmst);
	printf("                      H:M:S %d:%d:%12.12f\n", hmst.degrees, hmst.minutes, hmst.seconds);
}

void transform_test(void)
{
	struct lnh_equ_posn hobject, hpollux, hequ;
	struct lnh_hrz_posn hhrz;
	struct lnh_lnlat_posn hobserver, hecl;
	struct ln_equ_posn object, pollux, equ;
	struct ln_hrz_posn hrz;
	struct ln_lnlat_posn observer, ecl;
	double JD;
	struct ln_date date;

	printf ("\n\nTransformations Test \n");

	hobserver.lng.degrees = 77;
	hobserver.lng.minutes = 03;
	hobserver.lng.seconds = 56;
	hobserver.lat.degrees = 38;
	hobserver.lat.minutes = 55;
	hobserver.lat.seconds = 17;

	hobject.ra.hours = 23;
	hobject.ra.minutes = 9;
	hobject.ra.seconds = 16.641;
	hobject.dec.degrees = -6;
	hobject.dec.minutes = -43;
	hobject.dec.seconds = -11.61;

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
	hrz_to_hhrz (&hrz, &hhrz);
	printf("Alt %d:%d:%f\nAz %d:%d:%f\n",hhrz.alt.degrees, hhrz.alt.minutes, hhrz.alt.seconds, hhrz.az.degrees, hhrz.az.minutes, hhrz.az.seconds);

	get_equ_from_hrz (&hrz, &observer, JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Ra %d:%d:%f\nDec %d:%d:%f\n",hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);

	hpollux.ra.hours = 7;
	hpollux.ra.minutes = 45;
	hpollux.ra.seconds = 18.946;
	hpollux.dec.degrees = 28;
	hpollux.dec.minutes = 1;
	hpollux.dec.seconds = 34.26;

	hequ_to_equ (&hpollux, &pollux);
	get_ecl_from_equ(&pollux, JD, &ecl);
	
	lnlat_to_hlnlat (&ecl, &hecl);
	printf("Long %d:%d:%f\nLat %d:%d:%f\n",hecl.lng.degrees, hecl.lng.minutes, hecl.lng.seconds, hecl.lat.degrees, hecl.lat.minutes, hecl.lat.seconds);
	get_equ_from_ecl(&ecl, JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Ra %d:%d:%f\nDec %d:%d:%f\n",hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
}    

void sidereal_test ()
{
	struct ln_date date;
	double sd;
	double JD;

	printf("\n\nSidereal time test......\n");

	date.years = 1987;
	date.months = 4;
	date.days = 10;
	date.hours = 19;
	date.minutes = 21;
	date.seconds = 0;

	JD = get_julian_day (&date);
	printf("JD is %f\n",JD);

	sd = get_mean_sidereal_time (JD);

	printf("SD mean %f hours \n",sd);

	sd = get_apparent_sidereal_time (JD);

	printf("SD apparent %f hours \n",sd);

}

void solar_coord_test (void)
{
	struct ln_helio_posn pos;
	printf("\n\nSolar Coords Test....\n");
	get_geom_solar_coords (2448908.5, &pos);
	printf("long %f lat %f radius vec %f\n", pos.L, pos.B, pos.R);
}

void aberration_test (void)
{
	struct lnh_equ_posn hobject, hpos;
	struct ln_equ_posn object, pos;
	struct ln_date date;
	double JD;

	printf("\n\nAberration Test....\n");

	hobject.ra.hours = 2;
	hobject.ra.minutes = 46;
	hobject.ra.seconds = 11.331;
	hobject.dec.degrees = 49;
	hobject.dec.minutes = 20;
	hobject.dec.seconds = 54.54;

	date.years = 2028;
	date.months = 11;
	date.days = 13;
	date.hours = 4;
	date.minutes = 31;
	date.seconds = 0;

	JD = get_julian_day (&date);

	hequ_to_equ (&hobject, &object);
	get_equ_aber (&object, JD, &pos);
	equ_to_hequ(&pos, &hpos);
	printf("Ra %d:%d:%f\nDec %d:%d:%f\n",hpos.ra.hours, hpos.ra.minutes, hpos.ra.seconds, hpos.dec.degrees, hpos.dec.minutes, hpos.dec.seconds);
}

void precession_test(void)
{
	double JD;
	struct ln_equ_posn object, pos;
	struct lnh_equ_posn hobject, hpos;

	printf("\n\nPrecession test\n");

	hobject.ra.hours = 2;
	hobject.ra.minutes = 44;
	hobject.ra.seconds = 11.986;
	hobject.dec.degrees = 49;
	hobject.dec.minutes = 13;
	hobject.dec.seconds = 42.48;

	JD = 2462088.69;

	get_equ_prec (&object, JD, &pos);
	equ_to_hequ (&pos, &hpos);
	printf("Ra %d:%d:%f\nDec %d:%d:%f\n",hpos.ra.hours, hpos.ra.minutes, hpos.ra.seconds, hpos.dec.degrees, hpos.dec.minutes, hpos.dec.seconds);	
}

void apparent_position_test(void)
{
	double JD;
	struct lnh_equ_posn hobject, hpm, hpos;
	struct ln_equ_posn object, pm, pos;	

	printf("\n\nApparent Position test\n");

	hobject.ra.hours = 2;
	hobject.ra.minutes = 44;
	hobject.ra.seconds = 11.986;
	hobject.dec.degrees = 49;
	hobject.dec.minutes = 13;
	hobject.dec.seconds = 42.48;

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
	equ_to_hequ (&pos, &hpos);
	printf("Ra %d:%d:%f\nDec %d:%d:%f\n",hpos.ra.hours, hpos.ra.minutes, hpos.ra.seconds, hpos.dec.degrees, hpos.dec.minutes, hpos.dec.seconds);
}

void vsop87_test(void)
{
	struct ln_helio_posn pos;
	struct lnh_equ_posn hequ;
	struct ln_equ_posn equ;
	double JD = 2448976.5;
	double au;
	struct ln_date date, pdate;
/*	JD = get_julian_from_sys();	*/
#if 0
	date.years = 1992;
	date.months = 12;
	date.days = 20; 
	date.hours = 0;
	date.minutes = 0;
	date.seconds = 0;

	JD = get_julian_day (&date);
#endif
	printf("\n\nVSOP87 Test... for Julian Day %f\n",JD);
	
	get_equ_solar_coords (JD, &equ);
	equ_to_hequ (&equ, &hequ);
	printf("Sun RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
	
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
	return (0);
}
