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

Copyright 2004 Liam Girdwood
 
*/

#ifndef _LN_RISE_SET_H
#define _LN_RISE_SET_H

#include <libnova/ln_types.h>

#define LN_STAR_STANDART_HORIZONT		-0.5667

#ifdef __cplusplus
extern "C" {
#endif


/*! \defgroup rst Rise, Set, Transit
*
* Functions relating to an objects rise, set and transit
*
* All angles are expressed in degrees.
*/

/*! \fn double ln_get_object_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object,struct ln_rst_time * rst);
* \brief Calculate the time of rise, set and transit for an object not orbiting the Sun.
* \ingroup rst
*/
int ln_get_object_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst);

/*! \fn double ln_get_object_next_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object,struct ln_rst_time * rst);
* \brief Calculate the time of next rise, set and transit for an object not orbiting the Sun.
* E.g. it's sure, that rise, set and transit will be in <JD, JD+1> range.
* \ingroup rst
*/
int ln_get_object_next_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst);

int ln_get_body_rst_horizont (double JD, struct ln_lnlat_posn * observer, void (*get_equ_body_coords) (double, struct ln_equ_posn *), double horizont, struct ln_rst_time * rst);

#ifdef __cplusplus
};
#endif

#endif
