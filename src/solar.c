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
#include <math.h>


/*! \fn void get_geom_solar_coords (double JD, struct ln_helio_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate geometric coordinates and radius vector
* accuracy 0.01 arc second error - uses VSOP87 solution.
*
* Latitude and Longitude returned are in degrees, whilst radius
* vector returned is in AU.
*/

void get_geom_solar_coords (double JD, struct ln_helio_posn * position)
{		
	/* get earths heliocentric position */
	get_earth_helio_coords (JD, position);

	position->L += 180.0;
	position->L = range_degrees (position->L);
	position->B *= -1.0;
}

/*! \fn void get_equ_solar_coords (double JD, struct ln_equ_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate apparent equatorial solar coordinates for given julian day.
* This function includes the effects of aberration and nutation. 
*/
void get_equ_solar_coords (double JD, struct ln_equ_posn * position)
{
	struct ln_helio_posn sol;
	struct ln_lnlat_posn LB;
	struct ln_nutation nutation;
	double aberration;
	
	/* get geometric coords */
	get_geom_solar_coords (JD, &sol);
	
	/* add nutation */
	get_nutation (JD, &nutation);
	sol.L += nutation.longitude;

	/* aberration */
	aberration = (20.4898 / (360 * 60 * 60)) / sol.R;
	sol.L -= aberration;
	
	/* transform to equatorial */
	LB.lat = sol.B;
	LB.lng = sol.L;
	get_equ_from_ecl (&LB, JD, position);
}

/*! \fn void get_ecl_solar_coords (double JD, struct ln_lnlat_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate apparent ecliptical solar coordinates for given julian day.
* This function includes the effects of aberration and nutation. 
*/
void get_ecl_solar_coords (double JD, struct ln_lnlat_posn * position)
{
	struct ln_helio_posn sol;
	struct ln_nutation nutation;
	double aberration;
	
	/* get geometric coords */
	get_geom_solar_coords (JD, &sol);
	
	/* add nutation */
	get_nutation (JD, &nutation);
	sol.L += nutation.longitude;

	/* aberration */
	aberration = (20.4898 / (360 * 60 * 60)) / sol.R;
	sol.L -= aberration;
	
	position->lng = sol.L;
	position->lat = sol.B;
}

/*! \fn void get_geo_solar_coords (double JD, struct ln_geo_posn * position)
* \param JD Julian day
* \param position Pointer to store calculated solar position.
*
* Calculate geocentric coordinates (rectangular) for given julian day.
* Accuracy 0.01 arc second error - uses VSOP87 solution.
* Position returned is in units of AU.
*/
void get_geo_solar_coords (double JD, struct ln_rect_posn * position)
{		
	/* get earths's heliocentric position */
	struct ln_helio_posn sol;
	get_earth_helio_coords (JD, &sol);
	
	/* now get rectangular coords */
	get_rect_from_helio (&sol, JD, position);
	position->X *=-1.0;
	position->Y *=-1.0;
	position->Z *=-1.0;
}

/*! \fn double get_solar_rst (double JD, struct ln_lnlat_posn * observer, struct ln_rst_time * rst);
* \param JD Julian day
* \param observer Observers position
* \param rst Pointer to store Rise, Set and Transit time in JD
* \return 0 for success, else 1 for circumpolar.
*
* Calculate the time the rise, set and transit (crosses the local meridian at upper culmination)
* time of the Sun for the given Julian day.
*
* Note: this functions returns 1 if the Sun is circumpolar, that is it remains the whole
* day either above or below the horizon.
*/
int get_solar_rst (double JD, struct ln_lnlat_posn * observer, struct ln_rst_time * rst)
{
	int jd;
	double T, O, JD_UT, H0, H1;
	double Hat, Har, Has, altr, alts;
	double mt, mr, ms, mst, msr, mss, nt, nr, ns;
	double h = -0.8333; /* standard altitude of the sun */
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
	
	/* get solar coords for JD_UT -1, JD_UT and JD_UT + 1 */
	get_equ_solar_coords (JD_UT - 1.0, &sol1);
	get_equ_solar_coords (JD_UT, &sol2);
	get_equ_solar_coords (JD_UT + 1.0, &sol3);
	
	/* equ 15.1 */
	H0 = (sin (deg_to_rad (h)) - sin (deg_to_rad(observer->lat)) * sin (deg_to_rad(sol2.dec)));
	H1 = (cos(deg_to_rad(observer->lat)) * cos (deg_to_rad(sol2.dec)));

	/* check if Sun is circumpolar */
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
	
/*! \fn double get_solar_sdiam (double JD)
* \param JD Julian day
* \return Semidiameter in arc seconds
*
* Calculate the semidiameter of the Sun in arc seconds for the 
* given julian day.
*/
double get_solar_sdiam (double JD)
{
	double S, So = 959.63; /* at 1 AU */
	double dist;
	
	dist = get_earth_sun_dist (JD);
	S = So / dist;
	
	return (S);
}
	
