/* $Id: trig.h,v 1.1.1.1 2002-07-15 12:18:47 l_girdwood Exp $
**
* Copyright (C) 1999, 2000 Juan Carlos Remis
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  Module:                                                               */
/*                                                                        */
/*  Description:                                                          */
/*                                                                        */
/*                                                                        */
/*  "CAVEAT UTILITOR".                                                    */
/*                                                                        */
/*                   "Non sunt multiplicanda entia praeter necessitatem"  */
/*                                                   Guillermo de Occam.  */
/*------------------------------------------------------------------------*/
/*  Revision History:                                                     */
/*                                                                        */
/*------------------------------------------------------------------------*/

/**/

#ifndef LN_TRIG_H
#define LN_TRIG_H

	    
/* Conversion factors between degrees and radians */
#define D2R  (1.7453292519943295769e-2)  /* deg->radian */
#define R2D  (5.7295779513082320877e1)   /* radian->deg */
#define R2S  (2.0626480624709635516e5)   /* arc seconds per radian */
#define S2R  (4.8481368110953599359e-6)  /* radians per arc second */

#define DM_PI (2*M_PI)
#define RADIAN (180.0 / M_PI)


#endif
