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

Copyright (C) 2004 Liam Girdwood <liam@gnova.org>

*/

#ifndef _LN_JULIAN_DAY_H
#define _LN_JULIAN_DAY_H

#include <time.h>
#include <libnova/ln_types.h>

/*! \defgroup calendar General Calendar Functions 
*/
 
/*! \fn double ln_get_julian_day (struct ln_date * date)
* \ingroup calendar
* \brief Calculate the julian day from date.
*/
double ln_get_julian_day (struct ln_date * date);

/*! \fn void ln_get_date (double JD, struct ln_date * date)
* \ingroup calendar
* \brief Calculate the date from the julian day.
*/
void ln_get_date (double JD, struct ln_date * date);

/*! \fn void ln_get_local_date (double JD, struct ln_date * date)
* \ingroup calender
* \brief Calculate the local date from the Julian day  
*/
void ln_get_local_date (double JD, struct ln_date * date);

/*! \fn unsigned int ln_get_day_of_week (struct ln_date * date)
* \ingroup calendar
* \brief Calculate day of the week.
*/
unsigned int ln_get_day_of_week (struct ln_date *date);
	
/*! \fn double ln_get_julian_from_sys ()
* \brief Calculate julian day from system time.
* \ingroup calendar
*/
double ln_get_julian_from_sys ();


/*! \fn void ln_get_date_from_sys (struct ln_date * date)
* \brief Calculate date from system date
* \ingroup calendar
*/
void ln_get_date_from_sys (struct ln_date * date);
	
/*! \fn double ln_get_julian_from_timet (time_t * time)
* \brief Calculate julian day from time_t
* \ingroup calendar
*/
double ln_get_julian_from_timet (time_t * in_time);

/*! \fn void ln_get_timet_from_julian (double JD, time_t * time)
* \brief Calculate time_t from julian day
* \ingroup calendar
*/
void ln_get_timet_from_julian (double JD, time_t * in_time);

/*! \fn double ln_get_julian_local_date(struct ln_date* date)
* \brief Calculate Julian day from local date
* \ingroup calendar
*/
double ln_get_julian_local_date(struct ln_date* date);
	
/*! \fn int ln_get_date_from_mpc (struct ln_date* date, char* mpc_date)
* \brief Calculate the local date from the a MPC packed date.
* \ingroup calendar
*/
int ln_get_date_from_mpc (struct ln_date* date, char* mpc_date);
	
/*! \fn double ln_get_julian_from_mpc (char* mpc_date)
* \brief Calculate the julian day from the a MPC packed date.
* \ingroup calendar
*/
double ln_get_julian_from_mpc (char* mpc_date);

#endif
