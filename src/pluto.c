/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Libary General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 

Copyright (C) 2001 Liam Girdwood <liam@nova-ioe.org>

*/

#include <math.h>
#include <stdlib.h>
#include "pluto.h"
#include "libnova.h"

#define PLUTO_COEFFS 43

/* cache variables */
static double cJD = 0, cL = 0, cB = 0, cR = 0;

static const struct pluto_argument argument[PLUTO_COEFFS] = {
	{0, 0, 1},
	{0, 0, 2},
	{0, 0, 3},
	{0, 0, 4},
	{0, 0, 5},
	{0, 0, 6},
	{0, 1, -1},
	{0, 1, 0},
	{0, 1, 1},
	{0, 1, 2},
	{0, 1, 3},
	{0, 2, -2},
	{0, 2, -1},
	{0, 2, 0},
	{1, -1, 0},
	{1, -1, 1},
	{1, 0, -3},
	{1, 0, -2},
	{1, 0, -1},
	{1, 0, 0},
	{1, 0, 1},
	{1, 0, 2},
	{1, 0, 3},
	{1, 0, 4},
	{1, 1, -3},
	{1, 1, -2},
	{1, 1, -1},
	{1, 1, 0},
	{1, 1, 1},
	{1, 1, 3},
	{2, 0, -6},
	{2, 0, -5},
	{2, 0, -4},
	{2, 0, -3},
	{2, 0, -2},
	{2, 0, -1},
	{2, 0, 0},
	{2, 0, 1},
	{2, 0, 2},
	{2, 0, 3},
	{3, 0, -2},
	{3, 0, -1},
	{3, 0, 0}
};


static const struct pluto_longitude longitude[PLUTO_COEFFS] = {
	{-19799805, 19850055},
	{897144, -4954829},
	{611149, 1211027},
	{-341243, -189585},
	{129287, -34992},
	{-38164, 30893},
	{20442, -9987},
	{-4063, -5071},
	{-6016, -3336},
	{-3956, 3039},
	{-667, 3572},
	{1276, 501},
	{1152, -917},
	{630, -1277},
	{2571, -459},
	{899, -1449},
	{-1016, 1043},
	{-2343, -1012},
	{7042, 788},
	{1199, -338},
	{418, -67},
	{120, -274},
	{-60, -159},
	{-82, -29},
	{-36, -20},
	{-40, 7},
	{-14, 22},
	{4, 13},
	{5,2},
	{-1,0},
	{2,0},
	{-4, 5},
	{4, -7},
	{14, 24},
	{-49, -34},
	{163, -48},
	{9, 24},
	{-4, 1},
	{-3,1},
	{1,3},
	{-3, -1},
	{5, -3},
	{0,0}
};

static const struct pluto_latitude latitude[PLUTO_COEFFS] = {
	{-5452852, -14974862},
	{3527812, 1672790},
	{-1050748, 327647},
	{178690, -292153},
	{18650, 100340},
	{-30697, -25823},
	{4878, 11248},
	{226, -64},
	{2030, -836},
	{69, -604},
	{-247, -567},
	{-57, 1},
	{-122, 175},
	{-49, -164},
	{-197, 199},
	{-25, 217},
	{589, -248},
	{-269, 711},
	{185, 193},
	{315, 807},
	{-130, -43},
	{5, 3},
	{2, 17},
	{2, 5},
	{2, 3},
	{3, 1},
	{2, -1},
	{1, -1},
	{0, -1},
	{0, 0},
	{0, -2},
	{2, 2},
	{-7, 0},
	{10, -8},
	{-3, 20},
	{6, 5},
	{14, 17},
	{-2, 0},
	{0, 0},
	{0, 0},
	{0, 1},
	{0, 0},
	{1, 0}
}; 	

static const struct pluto_radius radius[PLUTO_COEFFS] = {
	{66865439, 68951812},
	{-11827535, -332538},
	{1593179, -1438890},
	{-18444, 483220},
	{-65977, -85431},
	{31174, -6032},
	{-5794, 22161},
	{4601, 4032},
	{-1729, 234},
	{-415, 702},
	{239, 723},
	{67, -67},
	{1034, -451},
	{-129, 504},
	{480, -231},
	{2, -441},
	{-3359, 265},
	{7856, -7832},
	{36, 45763},
	{8663, 8547},
	{-809, -769},
	{263, -144},
	{-126, 32},
	{-35, -16},
	{-19, -4},
	{-15, 8},
	{-4, 12},
	{5, 6},
	{3, 1},
	{6, -2},
	{2, 2},
	{-2, -2},
	{14, 13},
	{-63, 13},
	{136, -236},
	{273, 1065},
	{251, 149},
	{-25, -9},
	{9, -2},
	{-8, 7},
	{2, -10},
	{19, 35},
	{10, 2}
};

                
/*! \fn void get_pluto_equ_coords (double JD, struct ln_equ_posn * position);
* \param JD julian Day
* \param position Pointer to store position
*
* Calculates Pluto's equatorial position for the given julian day.
*/ 
void get_pluto_equ_coords 
	(double JD,
	struct ln_equ_posn * position)
{
	struct ln_helio_posn h_sol, h_pluto;
	struct ln_rect_posn g_sol, g_pluto;
	double a,b,c;
	double ra, dec, delta, diff, last, t = 0;
	
	/* need typdef for solar heliocentric coords */
	get_geom_solar_coords (JD, &h_sol);
	get_rect_from_helio (&h_sol, JD,  &g_sol);
	
	do
	{
		last = t;
		get_pluto_helio_coords (JD - t, &h_pluto);
		get_rect_from_helio (&h_pluto, JD - t, &g_pluto);

		/* equ 33.10 pg 229 */
		a = g_sol.X + g_pluto.X;
		b = g_sol.Y + g_pluto.Y;
		c = g_sol.Z + g_pluto.Z;
	
		delta = a*a + b*b + c*c;
		delta = sqrt (delta);
		t = delta * 0.0057755183;
		diff = t - last;
	}
	while (diff > 0.0001 || diff < -0.0001);
		
	ra = b/a;
	ra = atan (ra);
	/* check for correct quadrant */
	if (a<0)
		ra += M_PI;

	dec = c / delta;
	dec = asin (dec);

	/* back to hours, degrees */
	position->ra = rad_to_deg (ra);
	position->dec = rad_to_deg (dec);
}
	
	
/*! \fn void get_pluto_helio_coords (double JD, struct ln_helio_posn * position)
* \param JD Julian Day
* \param position Pointer to store new heliocentric position
*
* Calculate Pluto's heliocentric coordinates for the given julian day. 
* This function is accurate to within 0.07" in longitude, 0.02" in latitude 
* and 0.000006 AU in radius vector.
*
* Note: This function is not valid outside the period of 1885-2099. 
*/
/* Chap 37. Equ 37.1
*/

void get_pluto_helio_coords (double JD, struct ln_helio_posn * position)
{
	double sum_longitude = 0, sum_latitude = 0, sum_radius = 0;
	double J, S, P;
	double t, a, sin_a, cos_a;
	int i;
		
	/* check cache first */
	if (JD == cJD)
	{
		/* cache hit */
		position->L = cL;
		position->B = cB;
		position->R = cR;
		return;
	}
	
	/* get julian centuries since J2000 */
	t = (JD - 2451545) / 36525;
	
	/* calculate mean longitudes for jupiter, saturn and pluto */
	J =  34.35 + 3034.9057 * t;
		  
   	S =  50.08 + 1222.1138 * t;

   	P = 238.96 +  144.9600 * t;
 		
	/* calc periodic terms in table 37.A */
	for (i=0; i < PLUTO_COEFFS; i++)
	{
		a = argument[i].J * J + argument[i].S * S + argument[i].P * P;
		sin_a = sin (deg_to_rad(a));
		cos_a = cos (deg_to_rad(a));
		
		
		/* longitude */
		sum_longitude += longitude[i].A * sin_a + longitude[i].B * cos_a;
		
		/* latitude */
		sum_latitude += latitude[i].A * sin_a + latitude[i].B * cos_a;
		
		/* radius */
		sum_radius += radius[i].A * sin_a + radius[i].B * cos_a;
	}
	
	/* calc L, B, R */
	position->L = 238.958116 + 144.96 * t + sum_longitude * 0.000001;
	position->B = -3.908239 + sum_latitude * 0.000001;
	position->R = 40.7241346 + sum_radius * 0.0000001; 
	
	/* save cache */
	cJD = JD;
	cL = position->L;
	cB = position->B;
	cR = position->R;
}

/*! \fn double get_pluto_earth_dist (double JD);
* \param JD Julian day
* \return Distance in AU
*
* Calculates the distance in AU between the Earth and Pluto for the
* given julian day.
*/
double get_pluto_earth_dist (double JD)
{
	struct ln_helio_posn h_pluto, h_earth;
	struct ln_rect_posn g_pluto, g_earth;
	double x, y, z, au;
	
	/* get heliocentric positions */
	get_pluto_helio_coords (JD, &h_pluto);
	get_earth_helio_coords (JD, &h_earth);
	
	/* get geocentric coords */
	get_rect_from_helio (&h_pluto, JD, &g_pluto);
	get_rect_from_helio (&h_earth, JD, &g_earth);
	
	/* use pythag */
	x = g_pluto.X - g_earth.X;
	y = g_pluto.Y - g_earth.Y;
	z = g_pluto.Z - g_earth.Z;;
	x = x * x;
	y = y * y;
	z = z * z;

	au = sqrt (x + y + z);
	
	return (au);
}
	
/*! \fn double get_pluto_sun_dist (double JD);
* \param JD Julian day
* \return Distance in AU
*
* Calculates the distance in AU between the Sun and Pluto for the
* given julian day.
*/ 
double get_pluto_sun_dist (double JD)
{
	struct ln_helio_posn h_pluto;

	/* get heliocentric position */
	get_pluto_helio_coords (JD, &h_pluto);
	
	return (h_pluto.R);
	
}
	
/*! \fn double get_pluto_magnitude (double JD);
* \param JD Julian day
* \return Visible magnitude of Pluto
*
* Calculate the visible magnitude of Pluto for the given
* julian day.
*/ 
double get_pluto_magnitude (double JD)
{
	double mag, delta, r;
	
	/* get distances */
	r = get_pluto_sun_dist (JD);
	delta = get_pluto_earth_dist (JD);

	mag = -1.0 + 5 * log10 (r * delta);
	
	return (mag);
}

/*! \fn double get_pluto_disk (double JD);
* \param JD Julian day
* \return Illuminated fraction of Plutos disk
*
* Calculate the illuminated fraction of Pluto's disk for
* the given julian day.
*/ 
/* Chapter 41 */
double get_pluto_disk (double JD)
{
	double k,r,delta,R;	
	
	/* get distances */
	R = get_earth_sun_dist (JD);
	r = get_pluto_sun_dist (JD);
	delta = get_pluto_earth_dist (JD);
	
	/* calc fraction angle */
	k = (((r + delta) * (r + delta)) - R * R) / (4 * r * delta);
	
	return (k);
}

/*! \fn double get_pluto_phase (double JD);
* \param JD Julian day
* \return Phase angle of Pluto (degrees)
*
* Calculate the phase angle of Pluto (Sun - Pluto - Earth)
* for the given julian day.
*/ 
/* Chapter 41 */
double get_pluto_phase (double JD)
{
	double i,r,delta,R;	
	
	/* get distances */
	R = get_earth_sun_dist (JD);
	r = get_pluto_sun_dist (JD);
	delta = get_pluto_earth_dist (JD);

	/* calc phase */
	i = (r * r + delta * delta - R * R) / (2 * r * delta);
	i = acos (i);
	i = rad_to_deg (i);
	
	return (i);
}


/*! \fn double get_pluto_rst (double JD, struct ln_lnlat_posn * observer, struct ln_rst_time * rst);
* \param JD Julian day
* \param observer Observers position
* \param rst Pointer to store Rise, Set and Transit time in JD
* \return 0 for success, else 1 for circumpolar.
*
* Calculate the time the rise, set and transit (crosses the local meridian at upper culmination)
* time of Pluto for the given Julian day.
*
* Note: this functions returns 1 if Pluto is circumpolar, that is it remains the whole
* day either above or below the horizon.
*/
int get_pluto_rst (double JD, struct ln_lnlat_posn * observer, struct ln_rst_time * rst)
{
	int jd;
	double T, O, JD_UT, H0, H1;
	double Hat, Har, Has, altr, alts;
	double mt, mr, ms, mst, msr, mss, nt, nr, ns;
	double h = -0.5667; /* standard altitude of the Pluto */
	struct ln_equ_posn sol1, sol2, sol3, post, posr, poss;
	double dmt, dmr, dms;
		
	/* dynamical time diff */
	T = get_dynamical_time_diff (JD);
	
	/* convert local sidereal time into degrees
	for 0h of UT on day JD*/
	jd = (int)JD;
	if (JD - jd > 0.5)
		JD_UT = jd + 1.5 + (T / (24 * 60 * 60));
	else
		JD_UT = jd - 0.5 + (T / (24 * 60 * 60));
	O = get_apparent_sidereal_time (JD_UT);
	O *= 15.0;
	
	/* get Pluto coords for JD_UT -1, JD_UT and JD_UT + 1 */
	get_pluto_equ_coords (JD_UT - 1.0, &sol1);
	get_pluto_equ_coords (JD_UT, &sol2);
	get_pluto_equ_coords (JD_UT + 1.0, &sol3);
	
	/* equ 15.1 */
	H0 = (sin (deg_to_rad (h)) - sin (deg_to_rad(observer->lat)) * sin (deg_to_rad(sol2.dec)));
	H1 = (cos(deg_to_rad(observer->lat)) * cos (deg_to_rad(sol2.dec)));

	/* check if Pluto is circumpolar */
	if (H1 > 1.0)
		return (1);
	
	H0 = acos (H0/H1);
	H0 = rad_to_deg(H0);

	/* equ 15.2 */
	mt = (sol2.ra + observer->lng - O) / 360.0;
	mr = mt - H0 / 360.0;
	ms = mt + H0 / 360.0;
	
	/* put in correct range */
	if (mt > 1.0 )
		mt--;
	else if (mt < -1.0)
		mt++;
	if (mr > 1.0 )
		mr--;
	else if (mr < -1.0)
		mr++;
	if (ms > 1.0 )
		ms--;
	else if (ms < -1.0)
		ms++;
	
	/* find sidereal time at Greenwich, in degrees, for each m*/
	mst = O + 360.985647 * mt;
	msr = O + 360.985647 * mr;
	mss = O + 360.985647 * ms;
	
	/* calc n */
	nt = mt + T / 86400.0;
	nr = mr + T / 86400.0;
	ns = ms + T / 86400.0;

	/* interpolate ra and dec for each m, except for transit dec (dec2) */
	posr.ra = interpolate3 (nr, sol1.ra, sol2.ra, sol3.ra);
	posr.dec = interpolate3 (nr, sol1.dec, sol2.dec, sol3.dec);
	post.ra = interpolate3 (nt, sol1.ra, sol2.ra, sol3.ra);
	poss.ra = interpolate3 (ns, sol1.ra, sol2.ra, sol3.ra);
	poss.dec = interpolate3 (ns, sol1.dec, sol2.dec, sol3.dec);
	
	/* find local hour angle */
	Hat = mst - observer->lng - post.ra;
	Har = msr - observer->lng - posr.ra;
	Has = mss - observer->lng - poss.ra;

	/* find altitude for rise and set */
	altr =  sin (deg_to_rad(observer->lat)) * sin (deg_to_rad(posr.dec)) +
				cos (deg_to_rad(observer->lat)) * cos (deg_to_rad(posr.dec)) * cos (deg_to_rad (Har));
	alts =  sin (deg_to_rad(observer->lat)) * sin (deg_to_rad(poss.dec)) +
				cos (deg_to_rad(observer->lat)) * cos (deg_to_rad(poss.dec)) * cos (deg_to_rad (Has));

	/* corrections for m */
	dmt = - (Hat / 360.0);
	dmr = (altr - h) / (360 * cos (deg_to_rad(posr.dec)) * cos(deg_to_rad(observer->lat)) * sin (deg_to_rad(Har)));
	dms = (alts - h) / (360 * cos (deg_to_rad(poss.dec)) * cos(deg_to_rad(observer->lat)) * sin (deg_to_rad(Has)));

	/* add corrections and change to JD */
	mt += dmt;
	mr += dms;
	ms += dms;
	rst->rise = JD_UT + mr;
	rst->transit = JD_UT + mt;
	rst->set = JD_UT + ms;
	
	/* not circumpolar */
	return (0);
}


/*! \fn double get_pluto_sdiam (double JD)
* \param JD Julian day
* \return Semidiameter in arc seconds
*
* Calculate the semidiameter of Pluto in arc seconds for the 
* given julian day.
*/
double get_pluto_sdiam (double JD)
{
	double S, So = 2.07; /* at 1 AU */
	double dist;
	
	dist = get_pluto_earth_dist (JD);
	S = So / dist;
	
	return (S);
}
	
