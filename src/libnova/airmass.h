/*
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 *  Copyright (C) 2000 - 2005 Petr Kubanek
 *  Copyright (C) 2011 Petr Kubanek, Institute of Physics <kubanek@fzu.cz>
 */

#ifndef _LN_AIRMASS_H
#define _LN_AIRMASS_H

#include <libnova/ln_types.h> 

#ifdef __cplusplus
extern "C" {
#endif

/*! \fn double ln_get_airmass (double alt, double airmass_scale);
 * \brief Calculate airmass in given altitude.
 * \ingroup airmass
 */
double LIBNOVA_EXPORT ln_get_airmass (double alt, double airmass_scale);

/*! \fn double ln_get_alt_from_airmass (double X, double airmass_scale)
 * \brief Calculate altitude for given airmass.
 * \ingroup airmass
 */
double LIBNOVA_EXPORT ln_get_alt_from_airmass (double X, double airmass_scale);

#ifdef __cplusplus
};
#endif

#endif
