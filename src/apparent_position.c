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

Copyright 2000 Liam Girdwood
 
*/

#include "libnova.h"
#include "apparent_position.h"
#include <stdlib.h>

/*
** Apparent place of an Object 
*/

/*! \fn void get_apparent_position (struct ln_equ_position * mean_position, struct ln_equ_position * proper_motion, double JD, struct ln_equ_position * position)
* \param mean_position Mean position of object
* \param proper_motion Proper motion of object
* \param JD Julian Day
* \param position Pointer to store new object position
*
* Get the apparent position of an object from its mean position 
* Uses mean equatorial coordinates 
*/
void get_apparent_position 
	(struct ln_equ_position * mean_position, 
	struct ln_equ_position * proper_motion, 
	double JD,
	struct ln_equ_position * position)
{
	struct ln_equ_position proper_position;
	struct ln_equ_position aberration_position;
	
	get_proper_motion_equatorial (mean_position, proper_motion, JD, &proper_position);
	get_aberration_equatorial (&proper_position, JD, &aberration_position);
	get_precession_equatorial (&aberration_position, JD, position);
}

