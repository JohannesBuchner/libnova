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

Copyright (C) 2002 Liam Girdwood <liam@nova-ioe.org>

*/

#ifndef LN_LUNAR_H
#define LN_LUNAR_H
#include "libnova.h"

/* used for elp1 - 3 */
struct main_problem
{
	int i1;
	int i2;
	int i3;
	int i4;
	double A;
	double B1;
	double B2;
	double B3;
	double B4;
	double B5;
	double B6;
};

/* used for elp 4 - 9 */
struct earth_pert
{
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	double O;
	double A;
	double P;
}; 

/* used for elp 10 - 21 */
struct planet_pert
{
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	int i6;
	int i7;
	int i8;
	int i9;
	int i10;
	int i11;
	double theta;
	double O;
	double P;
};

typedef struct earth_pert tidal_effects;
typedef struct earth_pert moon_pert;
typedef struct earth_pert rel_pert;
typedef struct earth_pert plan_sol_pert;
 

/* sum ELP series */
double sum_series_elp1 ();
double sum_series_elp2 ();
double sum_series_elp3 ();
double sum_series_elp4 (double t);
double sum_series_elp5 (double t);
double sum_series_elp6 (double t);
double sum_series_elp7 (double t);
double sum_series_elp8 (double t);
double sum_series_elp9 (double t);

/* initialise lunar constants */
void init_lunar_constants (double t);

#endif
