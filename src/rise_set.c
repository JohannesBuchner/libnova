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

Copyright 2002 Liam Girdwood
 
*/

#include "libnova.h"
#include <math.h>

/*! \fn double get_object_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst);
* \param JD Julian day
* \param observer Observers position
* \param object Object position
* \param rst Pointer to store Rise, Set and Transit time in JD
* \return 0 for success, else 1 for circumpolar.
*
* Calculate the time the rise, set and transit (crosses the local meridian at upper culmination)
* time of the object for the given Julian day.
*
* Note: this functions returns 1 if the object is circumpolar, that is it remains the whole
* day either above or below the horizon.
*/
int get_object_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst)
{
	int jd;
	double T, O, JD_UT, H0, H1;
	double mt, mr, ms;
	double h = -0.5667; /* standard altitude of stars */
		
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
	
	/* equ 15.1 */
	H0 = (sin (deg_to_rad (h)) - sin (deg_to_rad(observer->lat)) * sin (deg_to_rad(object->dec)));
	H1 = (cos(deg_to_rad(observer->lat)) * cos (deg_to_rad(object->dec)));

	/* check if object is circumpolar */
	if (H1 > 1.0)
		return (1);
	
	H0 = acos (H0/H1);
	H0 = rad_to_deg(H0);

	/* equ 15.2 */
	mt = (object->ra + observer->lng - O) / 360.0;
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
	
	rst->rise = JD_UT + mr;
	rst->transit = JD_UT + mt;
	rst->set = JD_UT + ms;
	
	/* not circumpolar */
	return (0);
}
	
