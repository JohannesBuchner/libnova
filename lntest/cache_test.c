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

Copyright 2000 Liam Girdwood  */


/* libnova test suite -- pretty scrappy at the moment, needs some proper tests */

#include "libnova.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void nutation_test (void)
{
	struct ln_date date;
	double JDE, JD, nutation_longitude, nutation_obliquity, nutation_ecliptic;
	int i;

	printf("\n\nNutation test.....\n");
	printf("Processor Time %ld mS\n",clock()/1000);        
	for (i=0;i<100000;i++)
	{
		date.years = 2028;
		date.months = 11;
		date.days = 13;
		date.hours = 0;
		date.minutes = 0;
		date.seconds = 0;

		JD = get_julian_day (&date);
		/*         printf("JD is %f\n",JD); */

		JDE = get_jde (JD);
		/*        printf("JDE is %f\n",JDE); */

		get_nutation (JD, &nutation_longitude, &nutation_obliquity, &nutation_ecliptic);
		/*      printf("Nutation in obliquity %f ",nutation_obliquity); */
		/*      printf("Nutation in ecliptic %f ",nutation_ecliptic); */
	}
	printf("100000 iterations at the same epoch \n");
	printf("Nutation in longitude %f,obliquity %f,ecliptic %f \n",nutation_longitude,nutation_obliquity,nutation_ecliptic); 
	printf("Processor Time %ld mS\n",clock()/1000);        
    
	for (i=0;i<100000;i++)
	{
		date.years = 2028;
		date.months = 11;
		date.days = i;
		date.hours = 0;
		date.minutes = 0;
		date.seconds = 0;

		JD = get_julian_day (&date);
		/*         printf("JD is %f\n",JD); */

		JDE = get_jde (JD);
		/*        printf("JDE is %f\n",JDE); */

		get_nutation (JD, &nutation_longitude, &nutation_obliquity, &nutation_ecliptic);
		/*      printf("Nutation in obliquity %f ",nutation_obliquity); */
		/*      printf("Nutation in ecliptic %f ",nutation_ecliptic); */
	}
	printf("100000 iterations incrementing the epoch by 1 day \n");
	printf("Nutation in longitude %f,obliquity %f,ecliptic %f \n",nutation_longitude,nutation_obliquity,nutation_ecliptic); 
	printf("Processor Time %ld mS\n",clock()/1000);        
}


int main ()
{
  nutation_test();
  return (0);
}
