dnl
dnl AC_LIB_LIBNOVA (MINIMUM-VERSION)
dnl
dnl Check for availability of libnova.
dnl Abort if not found or if current version is not up to par.
dnl

AC_DEFUN([AC_LIB_LIBNOVA],[
	AC_PATH_PROG(LIBNOVA, libnovaconfig, no)
	if test "$LIBNOVA" = no; then
		AC_MSG_ERROR(Could not find libnovaconfig. Is libnova installed ?)
	fi
	min_libnova_version=ifelse([$1], ,0.1,$1)
	AC_MSG_CHECKING(for libnova - version >= $min_libnova_version)
	libnova_major_version=`$LIBNOVA --version | \
		sed 's/libnova \([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
	libnova_minor_version=`$LIBNOVA --version | \
		sed 's/libnova \([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
	libnova_micro_version=`$LIBNOVA --version | \
		sed 's/libnova \([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
	no_libnova=""
dnl
dnl Now check if the installed libnova is sufficiently new.
dnl
	AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libnova.h>

static char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char*) malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int 
main ()
{
  char  *tmp_version;
  
  int    major;
  int    minor;
  int    micro;

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_libnova_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
    printf ("%s, bad version string\n", "$min_libnova_version");
    exit (1);
  }

  if (($libnova_major_version > major) ||
      (($libnova_major_version == major) && ($libnova_minor_version > minor)) ||
      (($libnova_major_version == major) && ($libnova_minor_version == minor)) && ($libnova_micro_version >= micro)) {
    return 0;
  } else {
    printf ("\n");
    printf ("*** An old version of libnova ($libnova_major_version.$libnova_minor_version.$libnova_micro_version) was found.\n");
    printf ("*** You need a version of libnova newer than or equal to %d.%d.%d.  The latest version of\n",
	       major, minor, micro);
    printf ("*** libnova is always available from http://gnova.org.\n");
    printf ("***\n");
    return 1;
  }
}
],,no_libnova=yes,[/bin/true])
	if test "x$no_libnova" = x ; then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([libnova is required to compile Nova. Please check config.log.])
	fi

])
