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

Copyright 2002 Liam Girdwood
 
*/

#include "libnova.h"
#include <math.h>

/* number of steps in calculation, 3.32 steps for each significant 
digit required */
#define KEPLER_STEPS	53

/* the BASIC SGN() function  for doubles */
static inline double sgn (double x)
{
	if (x == 0.0)
		return (x);
	else
		if (x < 0.0)
			return (-1.0);
		else
			return (1.0);
}

/*! \fn double solve_kepler (double E, double M);
* \param E Orbital eccentricity
* \param M Mean anomaly
* \return Eccentric anomaly
*
* Calculate the eccentric anomaly. 
* This method was devised by Roger Sinnott. (Sky and Telescope, Vol 70, pg 159)
*/
double solve_kepler (double e, double M)
{
	double Eo = M_PI_2;
	double F, M1;
	double D = M_PI_4;
	int i;
	
	/* covert to radians */
	M = deg_to_rad (M);
	
	F = sgn (M); 
	M = fabs (M) / (2.0 * M_PI);
	M = (M - (int)M) * 2.0 * M_PI * F;
	
	if (M < 0)
		M = M + 2.0 * M_PI;
	F = 1.0;
	
	if (M > M_PI)
		F = -1.0;
	
	if (M > M_PI)
		M = 2.0 * M_PI - M;
	
	for (i = 0; i < KEPLER_STEPS; i++)
	{
		M1 = Eo - e * sin (Eo);
		Eo = Eo + D * sgn (M - M1);
		D /= 2.0;
	}
	Eo *= F;
	
	/* back to degrees */
	Eo = rad_to_deg (Eo);
	return (Eo);
}

/*! \fn double get_mean_anomaly (double n, double delta_JD);
* \param n Mean motion (degrees/day)
* \param delta_JD Time since perihelion
* \return Mean anomaly (degrees)
*
* Calculate the mean anomaly. LIAM add more 
*/
double get_mean_anomaly (double n, double delta_JD)
{
	return (delta_JD * n);
}

/*! \fn double get_true_anomaly (double n, double delta_JD);
* \param e Orbital eccentricity
* \param E Eccentric anomaly
* \return True anomaly (degrees)
*
* Calculate the true anomaly. LIAM add more 
*/
/* equ 30.1 */
double get_true_anomaly (double e, double E)
{
	double v;
	
	E = deg_to_rad (E);
	
	v = sqrt ((1.0 + e) / (1.0 - e)) * tan (E / 2.0);
	v = 2.0 * atan (v);
	v = rad_to_deg (v);
	return (v);
}

/*! \fn double get_radius_vector (double a, double e, double E);
* \param a Semi-Major axis in AU
* \param e Orbital eccentricity
* \param E Eccentric anomaly
* \return Radius vector AU
*
* Calculate the radius vector. LIAM add more 
*/
/* equ 30.2 */
double get_radius_vector (double a, double e, double E)
{
	double r;
	
	r = a * ( 1.0 - e * cos (deg_to_rad (E)));
	return (r);
}


/*! \fn double get_smajor_diam (double e, double q);
* \param e Eccentricity
* \param q Perihelion distance in AU
* \return Semi-major diameter in AU
*
* Calculate the semi major diameter. LIAM add more 
*/
double get_smajor_diam (double e, double q)
{
	return (q / (1.0 - e));
}

/*! \fn double get_sminor_diam (double e, double a);
* \param e Eccentricity
* \param a Semi-Major diameter in AU
* \return Semi-minor diameter in AU
*
* Calculate the semi minor diameter. LIAM add more 
*/
double get_sminor_diam (double e, double a)
{
	return (a * sqrt (1 - e * e));
}

/*! \fn double get_mean_motion (double a);
* \param a Semi major diameter in AU
* \return Mean daily motion (degrees/day)
*
* Calculate the mean daily motion (degrees/day). LIAM add more 
*/
double get_mean_motion (double a)
{
	double q = 0.9856076686; /* Gaussian gravitational constant (degrees)*/
	return (q / (a * sqrt(a)));
}

/*! \fn void get_geo_rect_posn (struct ln_orbit* orbit, double JD, struct ln_rect_posn* posn);
* \param orbit Orbital parameters of object.
* \param JD Julian day
* \param posn Position pointer to store objects position
*
* Calculate the objects rectangular geocentric position given it's orbital
* elements for the given julian day. LIAM add more 
*/
void get_geo_rect_posn (struct ln_orbit* orbit, double JD, struct ln_rect_posn* posn)
{
	double A,B,C;
	double F,G,H;
	double P,Q,R;
	double sin_e, cos_e;
	double a,b,c;
	double sin_omega, sin_i, cos_omega, cos_i;
	double M,v,E,r;
	
	/* J2000 obliquity of the ecliptic */
	sin_e = 0.397777156;
	cos_e = 0.917482062;
	
	/* equ 33.7 */
	sin_omega = sin (deg_to_rad (orbit->omega));
	cos_omega = cos (deg_to_rad (orbit->omega));
	sin_i = sin (deg_to_rad  (orbit->i));
	cos_i = cos (deg_to_rad  (orbit->i));
	F = cos_omega;
	G = sin_omega * cos_e;
	H = sin_omega * sin_e;
	P = -sin_omega * cos_i;
	Q = cos_omega * cos_i * cos_e - sin_i * sin_e;
	R = cos_omega * cos_i * sin_e + sin_i * cos_e;
	
	/* equ 33.8 */
	A = atan2 (F,P);
	B = atan2 (G,Q);
	C = atan2 (H,R);
	a = sqrt (F * F + P * P);
	b = sqrt (G * G + Q * Q);
	c = sqrt (H * H + R * R);

	/* get mean anomaly */
	if (orbit->n == 0)
		orbit->n = get_mean_motion (orbit->a);
	M = get_mean_anomaly (orbit->n, JD - orbit->JD);

	/* get eccentric anomaly */
	E = solve_kepler (orbit->e, M);
	
	/* get true anomaly */
	v = get_true_anomaly (orbit->e, E);
	
	/* get radius vector */
	r = get_radius_vector (orbit->a, orbit->e, E);

	/* equ 33.9 */
	posn->X = r * a * sin (A + deg_to_rad(orbit->w + v));
	posn->Y = r * b * sin (B + deg_to_rad(orbit->w + v));
	posn->Z = r * c * sin (C + deg_to_rad(orbit->w + v));
}

/*! \fn void get_helio_rect_posn (struct ln_orbit* orbit, double JD, struct ln_rect_posn* posn);
* \param orbit Orbital parameters of object.
* \param JD Julian day
* \param posn Position pointer to store objects position
*
* Calculate the objects rectangular heliocentric position given it's orbital
* elements for the given julian day. LIAM add more 
*/
void get_helio_rect_posn (struct ln_orbit* orbit, double JD, struct ln_rect_posn* posn)
{
	double M,v,E,r,u;
	double sin_omega, sin_i, cos_omega, cos_i, cos_u, sin_u;

	sin_omega = sin (deg_to_rad (orbit->omega));
	cos_omega = cos (deg_to_rad (orbit->omega));
	sin_i = sin (deg_to_rad  (orbit->i));
	cos_i = cos (deg_to_rad  (orbit->i));
	
	/* get mean anomaly */
	if (orbit->n == 0)
		orbit->n = get_mean_motion (orbit->a);
	M = get_mean_anomaly (orbit->n, JD - orbit->JD);

	/* get eccentric anomaly */
	E = solve_kepler (orbit->e, M);
	
	/* get true anomaly */
	v = get_true_anomaly (orbit->e, E);
	u = orbit->w + v;
	sin_u = sin (deg_to_rad (u));
	cos_u = cos (deg_to_rad (u));
	
	/* get radius vector */
	r = get_radius_vector (orbit->a, orbit->e, E);

	posn->X = r * (cos_omega * cos_u - sin_omega * sin_u * cos_i);
	posn->Y = r * (sin_omega * cos_u + cos_omega * sin_u * cos_i);
	posn->Z = r * sin_i * sin_u;
}


/*!
* \fn void get_body_equ_coords (double JD, struct ln_orbit * orbit, struct ln_equ_posn * posn)
* \param JD Julian Day.
* \param orbit Orbital parameters.
* \param posn Pointer to hold asteroid position.
*
* Calculate a bodies equatorial coordinates for the given julian day.
*/
void get_body_equ_coords (double JD, struct ln_orbit * orbit, struct ln_equ_posn * posn)
{
	struct ln_rect_posn body_rect_posn, sol_rect_posn;
	double dist, t;
	double x,y,z;
	
	/* get solar and body rect coords */
	get_geo_rect_posn (orbit, JD, &body_rect_posn);
	get_geo_solar_coords (JD, &sol_rect_posn);
	
	/* calc distance and light time */
	dist = get_rect_distance (&body_rect_posn, &sol_rect_posn);
	t = get_light_time (dist);
	
	/* repeat calculation with new time (i.e. JD - t) */
	get_geo_rect_posn (orbit, JD - t, &body_rect_posn);
	
	/* calc equ coords equ 33.10 */
	x = sol_rect_posn.X + body_rect_posn.X;
	y = sol_rect_posn.Y + body_rect_posn.Y;
	z = sol_rect_posn.Z + body_rect_posn.Z;

	posn->ra = rad_to_deg(atan2 (y,x));
	posn->dec = rad_to_deg(atan2 (z,sqrt (x * x + y * y)));
}


/*! \fn double get_orbit_len (struct ln_orbit * orbit);
* \param orbit Orbital parameters
* \return Orbital length in AU
*
* Calculate the orbital length in AU. 
* Accuracy: 0.001% for e < 0.88
*			0.01% for e < 0.95
*			1% for e = 0.9997
*			3% for e = 1
*/
double get_orbit_len (struct ln_orbit * orbit)
{
	double len;
	double A,G,H;
	double b;
	
	b = get_sminor_diam (orbit->e, orbit->a);
	
	A = (orbit->a + b) / 2.0;
	G = sqrt (orbit->a * b);
	H = (2.0 * orbit->a * b) / (orbit->a + b);
	
	len = M_PI * (( 21.0 * A - 2.0 * G - 3.0 * G) / 8.0);
	return (len);
}

/*! \fn double get_orbit_vel (double JD, struct ln_orbit * orbit);
* \param JD Julian day.
* \param orbit Orbital parameters
* \return Orbital velocity in km/s.
*
* Calculate orbital velocity in km/s for the given julian day. 
*/
double get_orbit_vel (double JD, struct ln_orbit * orbit)
{
	double V;
	double r;
	
	r = get_body_solar_dist (JD, orbit);
	V = 1.0 / r - 1.0 / (2.0 * orbit->a);
	V = 42.1219 * sqrt (V);
	return (V);
}

/*! \fn double get_orbit_pvel (struct ln_orbit * orbit);
* \param orbit Orbital parameters
* \return Orbital velocity in km/s.
*
* Calculate orbital velocity at perihelion in km/s. 
*/
double get_orbit_pvel (struct ln_orbit * orbit)
{
	double V;
	
	V = 29.7847 / sqrt (orbit->a);
	V *= sqrt ((1.0 + orbit->e) / (1.0 - orbit->e));
	return (V);
}

/*! \fn double get_orbit_avel (struct ln_orbit * orbit);
* \param orbit Orbital parameters
* \return Orbital velocity in km/s.
*
* Calculate the orbital velocity at aphelion in km/s. 
*/
double get_orbit_avel (struct ln_orbit * orbit)
{
	double V;
	
	V = 29.7847 / sqrt (orbit->a);
	V *= sqrt ((1.0 - orbit->e) / (1.0 + orbit->e));
	return (V);
}

/*!
* \fn double get_body_solar_dist (double JD, struct ln_orbit * orbit)
* \param JD Julian Day.
* \param orbit Orbital parameters
* \return The distance in AU between the Sun and the body. 
*
* Calculate the distance between a body and the Sun.
*/
double get_body_solar_dist (double JD, struct ln_orbit * orbit)
{
	struct ln_rect_posn body_rect_posn, sol_rect_posn;
	double dist;
	
	/* get solar and body rect coords */
	get_helio_rect_posn (orbit, JD, &body_rect_posn);
	get_geo_solar_coords (JD, &sol_rect_posn);
	
	/* calc distance and light time */
	dist = get_rect_distance (&body_rect_posn, &sol_rect_posn);
	return (dist);
}

/*!
* \fn double get_asteroid_earth_dist (double JD, struct ln_orbit * orbit)
* \param JD Julian day.
* \param orbit Orbital parameters
* \returns Distance in AU
*
* Calculate the distance between an body and the Earth
* for the given julian day.
*/
double get_body_earth_dist (double JD, struct ln_orbit * orbit)
{
	struct ln_rect_posn body_rect_posn, earth_rect_posn;
	double dist;
			
	/* get solar and body rect coords */
	get_geo_rect_posn (orbit, JD, &body_rect_posn);
	earth_rect_posn.X = 0;
	earth_rect_posn.Y = 0;
	earth_rect_posn.Z = 0;
	
	/* calc distance and light time */
	dist = get_rect_distance (&body_rect_posn, &earth_rect_posn);
	return (dist);
}


/*! \fn double get_body_phase_angle (double JD, struct ln_orbit * orbit);
* \param JD Julian day
* \param orbit Orbital parameters
* \return Phase angle.
*
* Calculate the pase angle of the body. The angle Sun - body - Earth. 
*/
double get_body_phase_angle (double JD, struct ln_orbit * orbit)
{
	double r,R,d;
	double E,M;
	double phase;
	
	/* get mean anomaly */
	if (orbit->n == 0)
		orbit->n = get_mean_motion (orbit->a);
	M = get_mean_anomaly (orbit->n, JD - orbit->JD);
	
	/* get eccentric anomaly */
	E = solve_kepler (orbit->e, M);
	
	/* get radius vector */
	r = get_radius_vector (orbit->a, orbit->e, E);
	
	/* get solar and Earth distances */
	R = get_body_earth_dist (JD, orbit);
	d = get_body_solar_dist (JD, orbit);
	
	phase = (r * r + d * d + R * R) / ( 2.0 * r * d );
	phase = acos (deg_to_rad (phase));
	
	return (phase);
}
