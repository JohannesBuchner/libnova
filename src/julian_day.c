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

Copyright (C) 2000 Liam Girdwood <liam@nova-ioe.org>

*/

#include "julian_day.h"
#include "libnova.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

/*! \fn double get_julian_day (struct ln_date * date)
* \param date Date required.
* \return Julian day
*
* Calculate the julian day from a calendar day. 
* Valid for positive and negative years but not for negative JD.
*/
/* Formula 7.1 on pg 61 
*/
double get_julian_day (struct ln_date * date)
{
    double JD;
    double days;
    int a,b;
	struct ln_date local_date;
		
	/* create local copy */
    memcpy (&local_date, date, sizeof (struct ln_date));
		
    /* check for month = January or February */
    if (local_date.months < 3 )
    {
        local_date.years--;
	    local_date.months += 12;
	}
	
	a = local_date.years / 100;
	
	/* check for Julian or Gregorian calendar (starts Oct 4th 1582) */
	if (local_date.years > 1582 || 
		(local_date.years == 1582 && 
		(local_date.months > 10 || 
		(local_date.months == 10 &&
		local_date.days >= 4))))
	{
	    /* Gregorian calendar */    
	    b = 2 - a + (a / 4);
	}
	else
	{
	    /* Julian calendar */
	    b = 0;
	}
	
	/* add a fraction of hours, minutes and secs to days*/
	days = local_date.days + (double)(local_date.hours / 24.0) + (double)(local_date.minutes / 1440.0) + (double)(local_date.seconds /  86400.0);

	/* now get the JD */
	JD = (int)(365.25 * (local_date.years + 4716)) + 
	    (int)(30.6001 * (local_date.months + 1)) + days + b - 1524.5;
	
	return (JD);
}


/*! \fn unsigned int get_day_of_week (struct ln_date *date)
* \param date Date required
* \return Day of the week
*
* Calculate the day of the week. 
* Returns 0 = Sunday .. 6 = Saturday
*/
unsigned int get_day_of_week (struct ln_date *date)
{
    unsigned int day;
    double JD;
    
    /* get julian day */
    JD = get_julian_day (date);
    
    JD += 1.5;
    
    day = (int)JD % 7; 
    
    return (day);
}	

/*! \fn void get_date (double JD, struct ln_date * date)
* \param JD Julian day
* \param date Pointer to new calendar date.
*
* Calculate the date from the Julian day  
*/
void get_date (double JD, struct ln_date * date)
{
   int A,a,B,C,D,E;
   double F,Z;
   
   JD += 0.5;
   Z = (int) JD;
   F = JD - Z;
   
   if (Z < 2299161)
   {
       A = Z;
   }
   else
   {
       a = (int) ((Z - 1867216.25) / 36524.25);
       A = Z + 1 + a - (int)(a / 4);
   }
   
   B = A + 1524;
   C = (int) ((B - 122.1) / 365.25);
   D = (int) (365.25 * C);
   E = (int) ((B - D) / 30.6001);
   
   /* get the hms */
   date->hours = F * 24;
   F -= (double)date->hours / 24;
   date->minutes = F * 1440;
   F -= (double)date->minutes / 1440;
   date->seconds = F * 86400;
   
   /* get the day */
   date->days = B - D - (int)(30.6001 * E);
   
   /* get the month */
   if (E < 14)
   {
       date->months = E - 1;
   }
   else
   {
       date->months = E - 13;
   }
   
   /* get the year */
   if (date->months > 2)
   {
       date->years = C - 4716;
   }
   else
   {
       date->years = C - 4715;
   }    
}	

#if 0
/*! \fn double get_julian_from_sys ()
* \return Julian day.
*
* Calculate local julian day from system time.
*/
double get_julian_from_sys ()
{
	double JD;
	struct ln_date date;
	
	/* get sys date */
	get_ln_date_from_sys (&date);
	
	JD = get_julian_day (&date);
	
	return (JD);
}
#endif


/*! \fn void get_ln_date_from_sys (struct ln_date * date)
* \param date Pointer to store date.
*
* Calculate local date from system date.
*/
void get_ln_date_from_sys (struct ln_date * date)
{
	time_t curtime;
	struct tm * loctime;
		
	/* get current time */
	curtime = time (NULL);

	/* convert to local time representation */
	loctime = localtime(&curtime);
	
	/* fill in date struct */
	date->seconds = loctime->tm_sec;
	date->minutes = loctime->tm_min;
	date->hours = loctime->tm_hour;
	date->days = loctime->tm_mday;
	date->months = loctime->tm_mon + 1;
	date->years = loctime->tm_year + 1900;
}


/*! \fn double get_julian_from_timet (time_t * in_time)
* \param time The time_t.
* \return Julian day.
*
* Calculate Julian day from time_t.
*/
double get_julian_from_timet (time_t * in_time)
{
	struct tm * loctime;
	struct ln_date date;
	double JD;
	
	/* convert to local time representation */
	loctime = localtime(in_time);
    
	/* fill in date struct */
	date.seconds = loctime->tm_sec;
	date.minutes = loctime->tm_min;
	date.hours = loctime->tm_hour;
	date.days = loctime->tm_mday;
	date.months = loctime->tm_mon + 1;
	date.years = loctime->tm_year + 1900;
	
	JD = get_julian_day (&date);
	
	return (JD);
}

/*! \fn void get_timet_from_julian (double JD, time_t * in_time)
* \param JD Julian day
* \param time Pointer to store time_t
*
* Calculate time_t from julian day
*/
void get_timet_from_julian (double JD, time_t * in_time)
{
	struct tm loctime;
	struct ln_date date;
		
	get_date (JD, &date);
	
	/* fill in date struct */
	if (date.years < 1900)
	{
		*in_time = 0;
		return;
	}
	loctime.tm_sec = date.seconds;
	loctime.tm_min = date.minutes;
	loctime.tm_hour = date.hours;
	loctime.tm_mday =date.days;
	loctime.tm_mon = date.months -1;
	loctime.tm_year = date.years - 1900;
	loctime.tm_isdst = 0;
	
	*in_time = mktime (&loctime);
}

/*! \fn double get_julian_from_sys()
* \return Julian day 
*
* Calculate the julian day from the local system time
*/
double get_julian_from_sys()
{
	double JD;
	time_t curtime;
	struct tm * loctime;
	struct ln_date date;
		
	/* get sys date */
	get_ln_date_from_sys (&date);
	JD = get_julian_day (&date);
	
	/* add day light savings time and hour angle */
	curtime = time (NULL);
	loctime = localtime(&curtime);
	
	JD += ((double)loctime->tm_gmtoff) / (24.0 * 60.0 * 60.0);
	return (JD);
}

/*! \fn double get_julian_local_date(struct ln_date* date)
* \param date Local date
* \return Julian day (UT)
*
* Calculate Julian day from local date
*/
double get_julian_local_date(struct ln_date* date)
{
	double JD;
	time_t curtime;
	struct tm * loctime;
		
	JD = get_julian_day (date);
	
	/* add day light savings time and hour angle */
	curtime = time (NULL);
	loctime = localtime(&curtime);
	JD += ((double)loctime->tm_gmtoff) / (24.0 * 60.0 * 60.0);
	return JD;
}

/*! \fn void get_local_date (double JD, struct ln_date * date)
* \param JD Julian day
* \param date Pointer to new calendar date.
*
* Calculate the local date from the Julian day  
*/
void get_local_date (double JD, struct ln_date * date)
{
	time_t curtime;
	struct tm * loctime;
		
	/* add day light savings time and hour angle */
	curtime = time (NULL);
	loctime = localtime(&curtime);
	
	/* add timezone to JD */
	JD += ((double)loctime->tm_gmtoff) / (24.0 * 60.0 * 60.0);
	
	get_date (JD, date);
}

/*! \fn int get_date_from_mpc (struct ln_date* date, char* mpc_date)
* \param date Pointer to new calendar date.
* \param mpc_date Pointer to string MPC date
* \return 0 for valid date
*
* Calculate the local date from the a MPC packed date.
* See http://cfa-www.harvard.edu/iau/info/PackedDates.html for info.
*/
int get_date_from_mpc (struct ln_date* date, char* mpc_date)
{
	char year[3];
	char month[2];
	char day[2];
	
	/* is mpc_date correct length */
	if (strlen(mpc_date) !=5)
		return -1;
	
	/* get the century */
	switch (*mpc_date) {
		case 'I':
			date->years = 1800;
		break;
		case 'J':
			date->years = 1900;
		break;
		case 'K':
			date->years = 2000;
		break;
		default:
			return -1;
	}
	
	/* get the year */
	year[0] = *(mpc_date + 1);
	year[1] = *(mpc_date + 2);
	year[3] = 0;
	date->years += strtol (year,0,10);
	
	/* month */
	month[0] = *(mpc_date + 3);
	month[1] = 0;
	date->months = strtol (month, 0, 16);
	
	/* day */
	day[0] = *(mpc_date + 4);
	day[1] = 0;
	date->days = strtol (day, 0, 31);
	
	/* reset hours,min,secs to 0 */
	date->hours = 0;
	date->minutes = 0;
	date->seconds = 0;
	return 0;
}

/*! \fn double get_julian_from_mpc (char* mpc_date)
* \param date Pointer to new calendar date.
* \param mpc_date Pointer to string MPC date
* \return Julian day.
*
* Calculate the julian day from the a MPC packed date.
* See http://cfa-www.harvard.edu/iau/info/PackedDates.html for info.
*/
double get_julian_from_mpc (char* mpc_date)
{
	struct ln_date date;
	double JD;
	
	get_date_from_mpc(&date, mpc_date);	
	JD = get_julian_day(&date);
	
	return JD;
}
