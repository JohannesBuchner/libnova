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
#include <config.h>
 

static char ln_version[10];

/*! \fn char * get_ln_version (void)
* \return Null terminated version string.
*
* Return a version number string
*/
char * get_ln_version (void)
{
    strcpy (ln_version, VERSION);
    return (ln_version);
}


/* convert radians to degrees */
double rad_to_deg (double radians)
{
    double degrees;
    
    degrees = (radians / (M_PI * 2.0)) * 360;
    return (degrees);
}    

/* convert degrees to radians */
double deg_to_rad (double degrees)
{
    double radians;
    
    radians = (degrees / 360) * 2.0 * M_PI;
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
      
    return (degrees);
}

/* convert dms to radians */
double dms_to_rad (struct ln_dms *dms)
{
    double radians;
    
    radians =  (double)dms->degrees / 360.0 * 2.0 * M_PI;
    radians += (double)dms->minutes / 21600.0 * 2.0 * M_PI;
    radians += (double)dms->seconds / 1296000.0 * 2.0 * M_PI;
      
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

/*! \fn void hequ_to_equ (struct lnh_equ_position * hpos, struct ln_equ_position * pos)
* \brief human readable equatorial position to double equatorial position
* \ingroup conversion
*/
void hequ_to_equ (struct lnh_equ_position * hpos, struct ln_equ_position * pos)
{
	pos->ra = hms_to_deg (&hpos->ra);
	pos->dec = dms_to_deg (&hpos->dec);
}
	
/*! \fn void equ_to_hequ (struct ln_equ_position * pos, struct lnh_equ_position * hpos)
* \brief human double equatorial position to human readable equatorial position
* \ingroup conversion
*/
void equ_to_hequ (struct ln_equ_position * pos, struct lnh_equ_position * hpos)
{
	deg_to_hms (pos->ra, &hpos->ra);
	deg_to_dms (pos->dec, &hpos->dec);
}
	
/*! \fn void hhoriz_to_horiz (struct lnh_horiz_position * hpos, struct ln_horiz_position * pos)
* \brief human readable horizontal position to double horizontal position
* \ingroup conversion
*/
void hhoriz_to_horiz (struct lnh_horiz_position * hpos, struct ln_horiz_position * pos)
{
	pos->alt = dms_to_deg (&hpos->alt);
	pos->az = dms_to_deg (&hpos->az);
}

/*! \fn void horiz_to_hhoriz (struct ln_horiz_position * pos, struct lnh_horiz_position * hpos)
* \brief double horizontal position to human readable horizontal position
* \ingroup conversion
*/
void horiz_to_hhoriz (struct ln_horiz_position * pos, struct lnh_horiz_position * hpos)
{
	deg_to_dms (pos->alt, &hpos->alt);
	deg_to_dms (pos->az, &hpos->az);
}
	
/*! \fn void hlong_lat_to_lng_lat (struct lnh_long_lat_position * hpos, struct ln_long_lat_position * pos)
* \brief human readable long/lat position to double long/lat position
* \ingroup conversion
*/
void hlong_lat_to_long_lat (struct lnh_long_lat_position * hpos, struct ln_long_lat_position * pos)
{
	pos->lng = dms_to_deg (&hpos->lng);
	pos->lat = dms_to_deg (&hpos->lat);
}
	
/*! \fn void long_lat_to_hlong_lat (struct ln_long_lat_position * pos, struct lnh_long_lat_position * hpos)
* \brief double long/lat position to human readable long/lat position
* \ingroup conversion
*/
void long_lat_to_hlong_lat (struct ln_long_lat_position * pos, struct lnh_long_lat_position * hpos)
{
	deg_to_dms (pos->lng, &hpos->lng);
	deg_to_dms (pos->lat, &hpos->lat);
}
 

