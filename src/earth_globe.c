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

#include "earth_globe.h"
#include "math.h"

/*! \fn void get_earth_centre_dist (float height, double latitude, double *p_sin_o, double *p_cos_o);
* \param height Height above sea level in metres.
* \param latitude latitude in degrees.
* \param p_sin_o Pointer to hold p_sin_o
* \param p_cos_o Pointer to hold p_cos_o
* \ingroup earth
* 
* Calculate the quantaties "p sin o" and "p cos o" needed in calculations for
* diurnal parallaxes, eclipses and occultations given the observers height
* in metres above sea level and there latitude in degrees.
*/

void get_earth_centre_dist (float height, double latitude, double *p_sin_o, double *p_cos_o)
{
     double a,b,f,u;
     
     a = 6378.14;
     
     f = 1 / 298.257;
     
     b = a * (1 - f);
     
     u = b / a * tan(deg_to_rad(latitude));
     u = atan(u);
     
     *p_sin_o = b / a * sin (u) + (height / 6378140 ) * sin (latitude);
     *p_cos_o = cos (u) + (height / 6378140) * cos (latitude);
}
     
     
