

noinst_PROGRAMS = \
	lntest ln_cache_test
	
bin_PROGRAMS = \
	libnovaconfig
	
lntest_SOURCES = \
	test.c 
	
libnovaconfig_SOURCES = \
	libnova_config.c 
	
ln_cache_test_SOURCES = \
	cache_test.c 
		
INCLUDES = \
	 -Wall -I../src

lntest_LDADD = \
	../src/libnova.la
	
ln_cache_test_LDADD = \
	../src/libnova.la
	
libnovaconfig_LDADD = \
	../src/libnova.la


