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

Copyright (C) 2000 Liam Girdwood <liam@gnova.org>

*/

#ifndef LN_DYNAMICAL_TIME_H
#define LN_DYNAMICAL_TIME_H

#include "libnova.h"

struct year_TD
{
    int year;
	double TD;
};


/* Stephenson and Houlden  for years prior to 948 A.D.*/
static double get_dynamical_diff_sh1 (double JD);

/* Stephenson and Houlden  for years between 948 A.D. and 1600 A.D.*/
static double get_dynamical_diff_sh2 (double JD);

/* Table 9.a pg 72 for years 1620..1992.*/
static double get_dynamical_diff_table (double JD);

/* get the dynamical time diff in the near past / future 1992 .. 2010 */
static double get_dynamical_diff_near (double JD);

/* uses equation 9.1 pg 73 to calc JDE for othe JD values */          
static double get_dynamical_diff_other (double JD);


#endif
