## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libnova.la

libnova_la_SOURCES = \
	julian_day.c \
	dynamical_time.c \
	sidereal_time.c \
	transform.c \
	nutation.c \
	libnova.c \
	aberration.c   \
	apparent_position.c \
	solar.c \
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
	lunar.c  \
	elliptic_motion.c \
	asteroid.c \
	comet.c \
	parabolic_motion.c \
	misc.c \
	refraction.c \
	rise_set.c


include_HEADERS = libnova.h  

noinst_HEADERS = \
	julian_day.h \
	dynamical_time.h \
	sidereal_time.h \
	transform.h \
	nutation.h \
	aberration.h \
	apparent_position.h \
	precession.h  \
	proper_motion.h \
	vsop87.h \
	pluto.h \
	lunar.h 
  

libnova_la_LDFLAGS = \
	-version-info $(LT_VERSION) \
	-release $(LT_RELEASE) 
	-export-dynamic

