/*
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 *  Copyright (C) 2000 - 2005 Liam Girdwood  
 */

#include <math.h>
#include <libnova/rise_set.h>
#include <libnova/utility.h>
#include <libnova/dynamical_time.h>
#include <libnova/sidereal_time.h>
#include <libnova/transform.h>

/*! \fn int ln_get_object_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst);
* \param JD Julian day
* \param observer Observers position
* \param object Object position
* \param rst Pointer to store Rise, Set and Transit time in JD
* \return 0 for success, 1 for circumpolar (above the horizon), -1 for circumpolar (bellow the horizon)
*
* Calculate the time the rise, set and transit (crosses the local meridian at upper culmination)
* time of the object for the given Julian day.
*
* Note: this functions returns 1 if the object is circumpolar, that is it remains the whole
* day above the horizon. Returns -1 when it remains the whole day bellow the horizon.
*/
int ln_get_object_rst (double JD, struct ln_lnlat_posn *observer, struct ln_equ_posn *object, struct ln_rst_time *rst)
{
	return ln_get_object_rst_horizon (JD, observer, object, -0.5667, rst);	/* standard altitude of stars */
}

/*! \fn int ln_get_object_rst_horizon (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, long double horizon, struct ln_rst_time * rst);
* \param JD Julian day
* \param observer Observers position
* \param object Object position
* \param horizon Horizon height
* \param rst Pointer to store Rise, Set and Transit time in JD
* \return 0 for success, 1 for circumpolar (above the horizon), -1 for circumpolar (bellow the horizon)
*
* Calculate the time the rise, set and transit (crosses the local meridian at upper culmination)
* time of the object for the given Julian day and horizon.
*
* Note: this functions returns 1 if the object is circumpolar, that is it remains the whole
* day above the horizon. Returns -1 when it remains whole day bellow the horizon.
*/
int ln_get_object_rst_horizon (double JD, struct ln_lnlat_posn * observer,
	struct ln_equ_posn * object, long double horizon, struct ln_rst_time * rst)
{
	int jd;
	long double O, JD_UT, H0, H1;
	double Hat, Har, Has, altr, alts;
	double mt, mr, ms, mst, msr, mss;
	double dmt, dmr, dms;

	/* convert local sidereal time into degrees
		 for 0h of UT on day JD */
	jd = (int) JD;
	JD_UT = jd + 0.5;
	O = ln_get_apparent_sidereal_time (JD_UT);
	O *= 15.0;

	/* equ 15.1 */
	H0 = (sin (ln_deg_to_rad (horizon)) -
		 sin (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (object->dec)));
	H1 = (cos (ln_deg_to_rad (observer->lat)) * cos (ln_deg_to_rad (object->dec)));

	H1 = H0 / H1;

	/* check if body is circumpolar; see note 2 in chapter 15 */
	if (fabs(H1) > 1.0)
	{
		if (observer->lat > 0)
		{
			if (object->dec < 0)
				return -1;
		}
		else if (observer->lat < 0)
		{
			if (object->dec > 0)
				return -1;
		}
		// on equator, object cannot be always bellow horizon
		return 1;
	}

	H0 = acos (H1);
	H0 = ln_rad_to_deg (H0);

	/* equ 15.2 */
	mt = (object->ra - observer->lng - O) / 360.0;
	mr = mt - H0 / 360.0;
	ms = mt + H0 / 360.0;

	/* put in correct range */
	if (mt > 1.0)
		mt--;
	else if (mt < 0)
		mt++;
	if (mr > 1.0)
		mr--;
	else if (mr < 0)
		mr++;
	if (ms > 1.0)
		ms--;
	else if (ms < 0)
		ms++;

	/* find sidereal time at Greenwich, in degrees, for each m */
	mst = O + 360.985647 * mt;
	msr = O + 360.985647 * mr;
	mss = O + 360.985647 * ms;

	/* find local hour angle */
	Hat = mst + observer->lng - object->ra;
	Har = msr + observer->lng - object->ra;
	Has = mss + observer->lng - object->ra;

	/* find altitude for rise and set */
	altr = sin (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (object->dec)) +
		cos (ln_deg_to_rad (observer->lat)) * cos (ln_deg_to_rad (object->dec)) *
		cos (ln_deg_to_rad (Har));
	alts = sin (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (object->dec)) +
		cos (ln_deg_to_rad (observer->lat)) * cos (ln_deg_to_rad (object->dec)) *
		cos (ln_deg_to_rad (Has));

	/* must be in degrees */
	altr = ln_rad_to_deg (altr);
	alts = ln_rad_to_deg (alts);

	/* corrections for m */
	ln_range_degrees (Hat);
	if (Hat > 180.0)
		Hat -= 360;

	dmt = -(Hat / 360.0);
	dmr = (altr - horizon) / (360 * cos (ln_deg_to_rad (object->dec)) * cos (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (Har)));
	dms = (alts - horizon) / (360 * cos (ln_deg_to_rad (object->dec)) * cos (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (Has)));

	/* add corrections and change to JD */
	mt += dmt;
	mr += dmr;
	ms += dms;

	rst->rise = JD_UT + mr;
	rst->transit = JD_UT + mt;
	rst->set = JD_UT + ms;

	/* not circumpolar */
	return 0;
}

// helper for ln_get_object_next_rst
void __ln__check_rst (double JD, double *val)
{
	while (*val > JD + LN_SIDEREAL_DAY_DAY)
		*val -= LN_SIDEREAL_DAY_DAY;
	while (*val < JD)
		*val += LN_SIDEREAL_DAY_DAY;
}

/*! \fn int ln_get_object_next_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst);
* \brief Calculate the time of next rise, set and transit for an object not orbiting the Sun.
* E.g. it's sure, that rise, set and transit will be in <JD, JD+1> range.
* This function is not too precise, it's good to get general idea when object will rise.
* \ingroup rst
*/
int ln_get_object_next_rst (double JD, struct ln_lnlat_posn *observer, struct ln_equ_posn *object, struct ln_rst_time *rst)
{
	return ln_get_object_next_rst_horizon (JD, observer, object, LN_STAR_STANDART_HORIZON, rst);
}

//helper functions for ln_get_object_next_rst_horizon
void set_next_rst (struct ln_rst_time * rst, double diff, struct ln_rst_time * out)
{
	out->rise = rst->rise + diff;
	out->transit = rst->transit + diff;
	out->set = rst->set + diff;
}

double find_next (double JD, double jd1, double jd2, double jd3)
{
	if (JD < jd1)
		return jd1;
	if (JD < jd2)
		return jd2;
	return jd3;
}

/*! \fn int ln_get_object_next_rst_horizon (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, double horizon, struct ln_rst_time * rst);
* \brief Calculate the time of next rise, set and transit for an object not orbiting the Sun.
* E.g. it's sure, that rise, set and transit will be in <JD, JD+1> range.
* This function is not too precise, it's good to get general idea when object will rise.
* \ingroup rst
*/
int ln_get_object_next_rst_horizon (double JD, struct ln_lnlat_posn *observer, struct ln_equ_posn *object,
    double horizon, struct ln_rst_time *rst)
{
	int ret;
	struct ln_rst_time rst_1, rst_2;
	ret = ln_get_object_rst_horizon (JD, observer, object, horizon, rst);
	if (ret)
		// circumpolar
		return ret;

	if (rst->rise > JD || rst->transit > JD || rst->set > JD)
		ln_get_object_rst_horizon (JD - 1, observer, object, horizon, &rst_1);
	else
		set_next_rst (rst, -1, &rst_1);

	if (rst->rise < JD || rst->transit < JD || rst->set < JD)
		ln_get_object_rst_horizon (JD + 1, observer, object, horizon, &rst_2);
	else
		set_next_rst (rst, +1, &rst_2);

	rst->rise = find_next (JD, rst_1.rise, rst->rise, rst_2.rise);
	rst->transit = find_next (JD, rst_1.transit, rst->transit, rst_2.transit);
	rst->set = find_next (JD, rst_1.set, rst->set, rst_2.set);
	return 0;
}

/*! \fn int ln_get_body_rst_horizon (double JD, struct ln_lnlat_posn *observer, void (*get_equ_body_coords) (double, struct ln_equ_posn *), double horizon, struct ln_rst_time *rst); 
* \param JD Julian day 
* \param observer Observers position 
* \param get_equ_body_coords Pointer to get_equ_body_coords() function
* \param horizon Sun horizon use *_HORIZONT constants from libnova.h 
* \param rst Pointer to store Rise, Set and Transit time in JD 
* \return 0 for success, else 1 for circumpolar.
*
* Calculate the time the rise, set and transit (crosses the local meridian at
* upper culmination) time of the body for the given Julian day and given
* horizon.
*
* Note 1: this functions returns 1 if the body is circumpolar, that is it remains
* the whole day either above or below the horizon.
*
* Note 2: This function will not work for body, which ra changes more
* then 180 deg in one day (get_equ_body_coords changes so much). But
* you should't use that function for any body which moves to fast..use
* some special function for such things.
*/
int ln_get_body_rst_horizon (double JD, struct ln_lnlat_posn *observer,
	void (*get_equ_body_coords) (double,struct ln_equ_posn *), double horizon,
	struct ln_rst_time *rst)
{
	int jd;
	double T, O, JD_UT, H0, H1;
	double Hat, Har, Has, altr, alts;
	double mt, mr, ms, mst, msr, mss, nt, nr, ns;
	struct ln_equ_posn sol1, sol2, sol3, post, posr, poss;
	double dmt, dmr, dms;

	/* dynamical time diff */
	T = ln_get_dynamical_time_diff (JD);

	/* convert local sidereal time into degrees
		 for 0h of UT on day JD */
	jd = (int) JD;
	JD_UT = jd + 0.5;
	O = ln_get_apparent_sidereal_time (JD_UT);
	O *= 15.0;

	/* get body coords for JD_UT -1, JD_UT and JD_UT + 1 */
	get_equ_body_coords (JD_UT - 1.0, &sol1);
	get_equ_body_coords (JD_UT, &sol2);
	get_equ_body_coords (JD_UT + 1.0, &sol3);

	/* equ 15.1 */
	H0 =
		(sin (ln_deg_to_rad (horizon)) -
		 sin (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (sol2.dec)));
	H1 = (cos (ln_deg_to_rad (observer->lat)) * cos (ln_deg_to_rad (sol2.dec)));

	H1 = H0 / H1;

	/* check if body is circumpolar */
	if (H1 > 1.0)
		return 1;
	if (H1 < -1.0)
		return -1;

	H0 = acos (H1);
	H0 = ln_rad_to_deg (H0);

	/* equ 15.2 */
	mt = (sol2.ra - observer->lng - O) / 360.0;
	mr = mt - H0 / 360.0;
	ms = mt + H0 / 360.0;

	/* put in correct range */
	if (mt > 1.0)
		mt--;
	else if (mt < 0)
		mt++;
	if (mr > 1.0)
		mr--;
	else if (mr < 0)
		mr++;
	if (ms > 1.0)
		ms--;
	else if (ms < 0)
		ms++;

	/* find sidereal time at Greenwich, in degrees, for each m */
	mst = O + 360.985647 * mt;
	msr = O + 360.985647 * mr;
	mss = O + 360.985647 * ms;

	/* correct ra values for interpolation	- put them to the same side of circle */
	if ((sol1.ra - sol2.ra) > 180.0)
		sol2.ra += 360;

	if ((sol2.ra - sol3.ra) > 180.0)
		sol3.ra += 360;

	if ((sol3.ra - sol2.ra) > 180.0)
		sol3.ra -= 360;

	if ((sol2.ra - sol1.ra) > 180.0)
		sol3.ra -= 360;

	nt = mt + T / 86400.0;
	nr = mr + T / 86400.0;
	ns = ms + T / 86400.0;

	/* interpolate ra and dec for each m, except for transit dec (dec2) */
	posr.ra = ln_interpolate3 (nr, sol1.ra, sol2.ra, sol3.ra);
	posr.dec = ln_interpolate3 (nr, sol1.dec, sol2.dec, sol3.dec);
	post.ra = ln_interpolate3 (nt, sol1.ra, sol2.ra, sol3.ra);
	poss.ra = ln_interpolate3 (ns, sol1.ra, sol2.ra, sol3.ra);
	poss.dec = ln_interpolate3 (ns, sol1.dec, sol2.dec, sol3.dec);

	/* find local hour angle */
	Hat = mst + observer->lng - post.ra;
	Har = msr + observer->lng - posr.ra;
	Has = mss + observer->lng - poss.ra;

	/* find altitude for rise and set */
	altr = sin (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (posr.dec)) +
		cos (ln_deg_to_rad (observer->lat)) * cos (ln_deg_to_rad (posr.dec)) *
		cos (ln_deg_to_rad (Har));
	alts = sin (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (poss.dec)) +
		cos (ln_deg_to_rad (observer->lat)) * cos (ln_deg_to_rad (poss.dec)) *
		cos (ln_deg_to_rad (Has));

	/* must be in degrees */
	altr = ln_rad_to_deg (altr);
	alts = ln_rad_to_deg (alts);

	/* corrections for m */
	ln_range_degrees (Hat);
	if (Hat > 180.0)
		Hat -= 360;

	dmt = -(Hat / 360.0);
	dmr = (altr - horizon) / (360 * cos (ln_deg_to_rad (posr.dec)) * cos (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (Har)));
	dms = (alts - horizon) / (360 * cos (ln_deg_to_rad (poss.dec)) * cos (ln_deg_to_rad (observer->lat)) * sin (ln_deg_to_rad (Has)));

	/* add corrections and change to JD */
	mt += dmt;
	mr += dmr;
	ms += dms;
	rst->rise = JD_UT + mr;
	rst->transit = JD_UT + mt;
	rst->set = JD_UT + ms;

	/* not circumpolar */
	return 0;
}

int ln_get_motion_body_rst_horizon (double JD, struct ln_lnlat_posn * observer, get_motion_body_coords_t get_motion_body_coords,
	void * orbit, double horizon, struct ln_rst_time * rst)
{
	int jd;
	double T, O, JD_UT, H0, H1;
	double Hat, Har, Has, altr, alts;
	double mt, mr, ms, mst, msr, mss, nt, nr, ns;
	struct ln_equ_posn sol1, sol2, sol3, post, posr, poss;
	double dmt, dmr, dms;
		
	/* dynamical time diff */
	T = ln_get_dynamical_time_diff (JD);
	
	/* convert local sidereal time into degrees
	for 0h of UT on day JD*/
	jd = (int)JD;
	JD_UT = jd + 0.5;
	O = ln_get_apparent_sidereal_time (JD_UT);
	O *= 15.0;
	
	/* get body coords for JD_UT -1, JD_UT and JD_UT + 1 */
	get_motion_body_coords (JD_UT - 1.0, orbit, &sol1);
	get_motion_body_coords (JD_UT, orbit, &sol2);
	get_motion_body_coords (JD_UT + 1.0, orbit, &sol3);
	
	/* equ 15.1 */
	H0 = (sin(ln_deg_to_rad (horizon)) - sin(ln_deg_to_rad(observer->lat)) * sin(ln_deg_to_rad(sol2.dec)));
	H1 = (cos(ln_deg_to_rad(observer->lat)) * cos(ln_deg_to_rad(sol2.dec)));

	/* check if body is circumpolar */
	if (fabs(H1) > 1.0)
	{
		if (observer->lat > 0)
		{
			if (sol2.dec < 0)
				return -1;
		}
		else if (observer->lat < 0)
		{
			if (sol2.dec > 0)
				return -1;
		}
		// on equator, object cannot be always bellow horizon
		return 1;
	}

	H0 = acos (H0/H1);
	H0 = ln_rad_to_deg(H0);

	/* equ 15.2 */
	mt = (sol2.ra - observer->lng - O) / 360.0;
	mr = mt - H0 / 360.0;
	ms = mt + H0 / 360.0;
	
	/* put in correct range */
	if (mt > 1.0 )
		mt--;
	else if (mt < 0.0)
		mt++;
	if (mr > 1.0 )
		mr--;
	else if (mr < 0.0)
		mr++;
	if (ms > 1.0 )
		ms--;
	else if (ms < 0.0)
		ms++;
	
	/* find sidereal time at Greenwich, in degrees, for each m*/
	mst = O + 360.985647 * mt;
	msr = O + 360.985647 * mr;
	mss = O + 360.985647 * ms;

	/* correct ra values for interpolation	- put them to the same side of circle */
	if ((sol1.ra - sol2.ra) > 180.0)
		sol2.ra += 360;

	if ((sol2.ra - sol3.ra) > 180.0)
		sol3.ra += 360;

	if ((sol3.ra - sol2.ra) > 180.0)
		sol3.ra -= 360;

	if ((sol2.ra - sol1.ra) > 180.0)
		sol3.ra -= 360;

	nt = mt + T / 86400.0;
	nr = mr + T / 86400.0;
	ns = ns + T / 86400.0;
	
	/* interpolate ra and dec for each m, except for transit dec (dec2) */
	posr.ra = ln_interpolate3 (nr, sol1.ra, sol2.ra, sol3.ra);
	posr.dec = ln_interpolate3 (nr, sol1.dec, sol2.dec, sol3.dec);
	post.ra = ln_interpolate3 (nt, sol1.ra, sol2.ra, sol3.ra);
	poss.ra = ln_interpolate3 (ns, sol1.ra, sol2.ra, sol3.ra);
	poss.dec = ln_interpolate3 (ns, sol1.dec, sol2.dec, sol3.dec);
	
	/* find local hour angle */
	Hat = mst + observer->lng - post.ra;
	Har = msr + observer->lng - posr.ra;
	Has = mss + observer->lng - poss.ra;

	/* find altitude for rise and set */
	altr = sin(ln_deg_to_rad(observer->lat)) * sin(ln_deg_to_rad(posr.dec)) +
				cos(ln_deg_to_rad(observer->lat)) * cos(ln_deg_to_rad(posr.dec)) * cos(ln_deg_to_rad (Har));
	alts = sin(ln_deg_to_rad(observer->lat)) * sin(ln_deg_to_rad(poss.dec)) +
				cos(ln_deg_to_rad(observer->lat)) * cos(ln_deg_to_rad(poss.dec)) * cos(ln_deg_to_rad (Has));

	/* corrections for m */
	dmt = - (Hat / 360.0);
	dmr = (altr - horizon) / (360 * cos(ln_deg_to_rad(posr.dec)) * cos(ln_deg_to_rad(observer->lat)) * sin(ln_deg_to_rad(Har)));
	dms = (alts - horizon) / (360 * cos(ln_deg_to_rad(poss.dec)) * cos(ln_deg_to_rad(observer->lat)) * sin(ln_deg_to_rad(Has)));

	/* add corrections and change to JD */
	mt += dmt;
	mr += dms;
	ms += dms;
	rst->rise = JD_UT + mr;
	rst->transit = JD_UT + mt;
	rst->set = JD_UT + ms;
	
	/* not circumpolar */
	return 0;
}
