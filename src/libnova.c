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
 
#include "libnova.h" 
#include <string.h>
#include <math.h>

#ifndef WIN32
#include "config.h"
#else
#define VERSION "0.9.0"
#endif

static char ln_version[16];

/*! \fn char * get_ln_version (void)
* \return Null terminated version string.
*
* Return the libnova library version number string
* e.g. "0.4.0"
*/
const char * get_ln_version (void)
{
    strcpy (ln_version, VERSION);
    return (ln_version);
}


/* convert radians to degrees */
double rad_to_deg (double radians)
{
    double degrees;
    
    degrees = (radians / (M_PI * 2.0)) * 360.0;
    return (degrees);
}    

/* convert degrees to radians */
double deg_to_rad (double degrees)
{
    double radians;
    
    radians = (degrees / 360.0) * 2.0 * M_PI;
    return (radians);
}    

/* convert hours:mins:secs to degrees */
double hms_to_deg (struct ln_hms *hms)
{
    double degrees;
    
    degrees = ((double)hms->hours / 24) * 360;
    degrees += ((double)hms->minutes / 60) * 15;
    degrees += ((double)hms->seconds / 60) * 0.25;
    
    return (degrees);
}

/* convert hours:mins:secs to radians */
double hms_to_rad (struct ln_hms *hms)
{
    double radians;
  
    radians = ((double)hms->hours / 24.0) * 2.0 * M_PI;
    radians += ((double)hms->minutes / 60.0) * 2.0 * M_PI / 24.0;
    radians += ((double)hms->seconds / 60.0) * 2.0 * M_PI / 1440.0;
    
    return (radians);
}


/* convert degrees to hh:mm:ss */
void deg_to_hms (double degrees, struct ln_hms * hms)
{
    double dtemp;
        
    degrees = range_degrees (degrees);	
    
	/* divide degrees by 15 to get the hours */
    hms->hours = dtemp = degrees / 15.0;
    dtemp -= hms->hours;
    
    /* divide remainder by 60 to get minutes */
    hms->minutes = dtemp = dtemp * 60.0;
    dtemp -= hms->minutes;

    
    /* divide remainder by 60 to get seconds */
    hms->seconds = dtemp * 60.0;
}

/* convert radians to hh:mm:ss */
void rad_to_hms (double radians, struct ln_hms * hms)
{
    double dtemp;
    double degrees;
         
    radians = range_radians(radians);
   
    degrees = radians * 360.0 / (2.0 * M_PI);		
  
    /* divide radians by PI / 12 to get the hours */
    hms->hours = dtemp = degrees / 15.0;
    dtemp -= hms->hours;
    
    /* divide remainder by 60 to get minutes */
    hms->minutes = dtemp = dtemp * 60.0;
    dtemp -= hms->minutes;

    /* divide remainder by 60 to get seconds */
    hms->seconds = dtemp * 60.0;
}


/* convert dms to degrees */
double dms_to_deg (struct ln_dms *dms)
{
    double degrees;
    
    degrees =  (double)dms->degrees;
    degrees += (double)dms->minutes / 60;
    degrees += (double)dms->seconds / 3600;
	
	// negative ?
	if (dms->sign ==0)
		degrees *= -1.0;
	
    return (degrees);
}

/* convert dms to radians */
double dms_to_rad (struct ln_dms *dms)
{
    double radians;
    
    radians =  (double)dms->degrees / 360.0 * 2.0 * M_PI;
    radians += (double)dms->minutes / 21600.0 * 2.0 * M_PI;
    radians += (double)dms->seconds / 1296000.0 * 2.0 * M_PI;
	
	// negative ?
	if (dms->sign == 0)
		radians *= -1.0;
      
    return (radians);
}



/* convert degrees to dms */
void deg_to_dms (double degrees, struct ln_dms * dms)
{
    double dtemp;
    
	/* floor degrees */
    dms->degrees = (int)degrees;
    dtemp = degrees - dms->degrees;
	
	/* positive values only */
    if (dtemp < 0)
		dtemp *= -1;
	
    /* divide remainder by 60 to get minutes */
    dms->minutes = dtemp = dtemp * 60;
    dtemp -= dms->minutes;
    
    /* divide remainder by 60 to get seconds */
    dms->seconds = dtemp * 60;
	
	if (degrees > 0)
		dms->sign = 0;
	else
		dms->sign = 1;
}

/* convert radians to dms */
void rad_to_dms (double radians, struct ln_dms * dms)
{
    double dtemp;
    double degrees;
    
    degrees = radians * 360.0 / (2.0 * M_PI);
    
   /* floor degrees */
    dms->degrees = (int)degrees ;
    dtemp = degrees - dms->degrees;
	if (dtemp < 0)
		dtemp *= -1;
    
    /* divide remainder by 60 to get minutes */
    dms->minutes = dtemp = dtemp * 60;
    dtemp -= dms->minutes;

    /* divide remainder by 60 to get seconds */
    dms->seconds = dtemp * 60;
	
	if (radians > 0)
		dms->sign = 0;
	else
		dms->sign = 1;
}


/* puts a large angle in the correct range 0 - 360 degrees */
double range_degrees (double angle)
{
    double temp;
    double range;
    
    
    if (angle >= 0.0 && angle < 360.0)
    	return(angle);
 
	temp = (int)(angle / 360);
	
	if ( angle < 0.0 )
	   	temp --;

    temp *= 360;
	range = angle - temp;
    return (range);
}

/* puts a large angle in the correct range 0 - 2PI radians */
double range_radians (double angle)
{
    double temp;
    double range;
    
    if (angle >= 0.0 && angle < (2.0 * M_PI))
    	return(angle);
    
	temp = (int)(angle / (M_PI * 2.0));

	if ( angle < 0.0 )
		temp --;
	temp *= (M_PI * 2.0);
	range = angle - temp;

	return (range);
}

/* puts a large angle in the correct range -2PI - 2PI radians */
/* preserve sign */
double range_radians2 (double angle)
{
    double temp;
    double range;
    
    if (angle > (-2.0 * M_PI) && angle < (2.0 * M_PI))
    	return(angle);
    
	temp = (int)(angle / (M_PI * 2.0));
	temp *= (M_PI * 2.0);
	range = angle - temp;

	return (range);
}


/* add seconds to hms */
void add_secs_hms (struct ln_hms * hms, double seconds)
{
    struct ln_hms source_hms;
    
    /* breaks double seconds int hms */
    source_hms.hours = seconds / 3600;
    seconds -= source_hms.hours * 3600;
    source_hms.minutes = seconds / 60;
    seconds -= source_hms.minutes * 60; 
    source_hms.seconds = seconds;
    
    /* add hms to hms */
    add_hms (&source_hms, hms);
}


/* add hms to hms */
void add_hms (struct ln_hms * source, struct ln_hms * dest)
{
    dest->seconds += source->seconds;
    if (dest->seconds >= 60)
    {
        /* carry */
	    source->minutes ++;
	    dest->seconds -= 60;
	}
	else
	{
	    if (dest->seconds < 0)
	    {
	        /* carry */
		    source->minutes --;
		    dest->seconds += 60;
		}
	}
	
	dest->minutes += source->minutes;
    if (dest->minutes >= 60)
    {
        /* carry */
	    source->hours ++;
	    dest->minutes -= 60;
	}
	else
	{
	    if (dest->seconds < 0)
	    {
	        /* carry */
		    source->hours --;
		    dest->minutes += 60;
		}
	}
    
    dest->hours += source->hours;
}

/*! \fn void hequ_to_equ (struct lnh_equ_posn * hpos, struct ln_equ_posn * pos)
* \brief human readable equatorial position to double equatorial position
* \ingroup conversion
*/
void hequ_to_equ (struct lnh_equ_posn * hpos, struct ln_equ_posn * pos)
{
	pos->ra = hms_to_deg (&hpos->ra);
	pos->dec = dms_to_deg (&hpos->dec);
}
	
/*! \fn void equ_to_hequ (struct ln_equ_posn * pos, struct lnh_equ_posn * hpos)
* \brief human double equatorial position to human readable equatorial position
* \ingroup conversion
*/
void equ_to_hequ (struct ln_equ_posn * pos, struct lnh_equ_posn * hpos)
{
	deg_to_hms (pos->ra, &hpos->ra);
	deg_to_dms (pos->dec, &hpos->dec);
}
	
/*! \fn void hhrz_to_hrz (struct lnh_hrz_posn * hpos, struct ln_hrz_posn * pos)
* \brief human readable horizontal position to double horizontal position
* \ingroup conversion
*/
void hhrz_to_hrz (struct lnh_hrz_posn * hpos, struct ln_hrz_posn * pos)
{
	pos->alt = dms_to_deg (&hpos->alt);
	pos->az = dms_to_deg (&hpos->az);
}

/*! \fn void hrz_to_hhrz (struct ln_hrz_posn * pos, struct lnh_hrz_posn * hpos)
* \brief double horizontal position to human readable horizontal position
* \ingroup conversion
*/
void hrz_to_hhrz (struct ln_hrz_posn * pos, struct lnh_hrz_posn * hpos)
{
	deg_to_dms (pos->alt, &hpos->alt);
	deg_to_dms (pos->az, &hpos->az);
}

/*! \fn const char * hrz_to_nswe (struct ln_hrz_posn * pos);
 * \brief returns direction of given azimut - like N,S,W,E,NSW,...
 * \ingroup conversion
 */ 
const char * hrz_to_nswe (struct ln_hrz_posn * pos)
{
	char * directions[] = {"S", "SSW", "SW", "SWW", "W", "NWW", "NW", "NNW", "N", "NNE", "NE", "NEE", 
		"E", "SEE", "SE", "SSE"};
	return directions[(int)(pos->az / 22.5)];
}
	
/*! \fn void hlnlat_to_lnlat (struct lnh_lnlat_posn * hpos, struct ln_lnlat_posn * pos)
* \brief human readable long/lat position to double long/lat position
* \ingroup conversion
*/
void hlnlat_to_lnlat (struct lnh_lnlat_posn * hpos, struct ln_lnlat_posn * pos)
{
	pos->lng = dms_to_deg (&hpos->lng);
	pos->lat = dms_to_deg (&hpos->lat);
}
	
/*! \fn void lnlat_to_hlnlat (struct ln_lnlat_posn * pos, struct lnh_lnlat_posn * hpos)
* \brief double long/lat position to human readable long/lat position
* \ingroup conversion
*/
void lnlat_to_hlnlat (struct ln_lnlat_posn * pos, struct lnh_lnlat_posn * hpos)
{
	deg_to_dms (pos->lng, &hpos->lng);
	deg_to_dms (pos->lat, &hpos->lat);
}

/*
* \fn double get_rect_distance (struct ln_rect_posn * a, struct ln_rect_posn * b)
* \param a First recatngular coordinate
* \param b Second rectangular coordinate
* \return Distance between a and b.
*
* Calculate the distance between rectangular points a and b.
*/
double get_rect_distance (struct ln_rect_posn * a, struct ln_rect_posn * b)
{
	double x,y,z;
	
	x = a->X - b->X;
	y = a->Y - b->Y;
	z = a->Z - b->Z;
	
	x *=x;
	y *=y;
	z *=z;
	
	return (sqrt (x + y + z));
}

/*
* \fn double get_light_time (double dist)
* \param dist Distance in AU
* \return Distance in light days.
*
* Convert units of AU into light days.
*/
double get_light_time (double dist)
{
	return (dist * 0.005775183);
}
