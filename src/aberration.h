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

Copyright 2000 Liam Girdwood

*/

#ifndef LN_ABERRATION_H
#define LN_ABERRATION_H

#include "libnova.h"

/* data structures to hold arguments and coefficients of Ron-Vondrak theory */
struct arg
{
	double a_L2;
	double a_L3;
	double a_L4;
	double a_L5;
	double a_L6;
	double a_L7;
	double a_L8;
	double a_LL;
	double a_D;
	double a_MM;
	double a_F;
};

struct XYZ
{
	double sin1;
	double sin2;
	double cos1;
	double cos2;
};

#endif
