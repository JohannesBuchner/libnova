## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libnova.la

libnova_la_SOURCES = \
	julian_day.c \
	dynamical_time.c \
	earth_globe.c \
	sidereal_time.c \
	transform.c \
	nutation.c \
	libnova.c \
	trig.c   \
	aberration.c   \
	apparent_position.c \
	solar_coordinates.c \
	precession.c  \
	proper_motion.c \
	mercury.c \
	venus.c  \
	earth.c  \
	mars.c   \
	jupiter.c \
	saturn.c  \
	uranus.c  \
	neptune.c  \
	pluto.c \
	vsop87.c \
	lunar.c


include_HEADERS = libnova.h  

noinst_HEADERS = \
	julian_day.h \
	dynamical_time.h \
	earth_globe.h \
	sidereal_time.h \
	transform.h \
	nutation.h \
	trig.h \
	aberration.h \
	apparent_position.h \
	solar_coordinates.h \
	precession.h  \
	proper_motion.h \
	vsop87.h \
	pluto.h \
	lunar.h
  

libnova_la_LDFLAGS = \
	-version-info $(LT_VERSION) \
	-release $(LT_RELEASE) 
	-export-dynamic

