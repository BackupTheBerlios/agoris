# Configure paths for agoris
# Erik Andersen	30 May 1998
# Modified by Tero Pulkkinen (added the compiler checks... I hope they work..)
# Modified by Thomas Langen 16 Jan 2000 (corrected CXXFLAGS)
# Adapted to the gtkextra-- library by Cedric Gustin 11 Feb 2000
# Modified by Murray Cumming for agoris example library.

dnl Test for AGORIS, and define AGORIS_CFLAGS and AGORIS_LIBS
dnl   to be used as follows:
dnl AM_PATH_AGORIS([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
AC_DEFUN(AM_PATH_AGORIS,
[dnl 
dnl Get the cflags and libraries from the agoris-config script
dnl
AC_ARG_WITH(AGORIS-prefix,[  --with-agoris-prefix=PREFIX
                          Prefix where agoris is installed (optional)],
            agoris_config_prefix="$withval", agoris_config_prefix="")
AC_ARG_WITH(agoris-exec-prefix,[  --with-agoris-exec-prefix=PREFIX
                          Exec prefix where agoris is installed (optional)],
            agoris_config_exec_prefix="$withval", agoris_config_exec_prefix="")
AC_ARG_ENABLE(agoristest, [  --disable-agoristest     Do not try to compile and run a test agoris program],
		    , enable_agoristest=yes)

  if test x$agoris_config_exec_prefix != x ; then
     agoris_config_args="$agoris_config_args --exec-prefix=$agoris_config_exec_prefix"
     if test x${AGORIS_CONFIG+set} != xset ; then
        AGORIS_CONFIG=$agoris_config_exec_prefix/bin/agoris-config
     fi
  fi
  if test x$agoris_config_prefix != x ; then
     agoris_config_args="$agoris_config_args --prefix=$agoris_config_prefix"
     if test x${AGORIS_CONFIG+set} != xset ; then
        AGORIS_CONFIG=$agoris_config_prefix/bin/agoris-config
     fi
  fi

  AC_PATH_PROG(AGORIS_CONFIG, agoris-config, no)
  min_agoris_version=ifelse([$1], ,0.1.1,$1)

  AC_MSG_CHECKING(for agoris - version >= $min_agoris_version)
  no_agoris=""
  if test "$AGORIS_CONFIG" = "no" ; then
    no_agoris=yes
  else
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

    AGORIS_CFLAGS=`$AGORIS_CONFIG $agoris_config_args --cflags`
    AGORIS_LIBS=`$AGORIS_CONFIG $agoris_config_args --libs`
    agoris_config_major_version=`$AGORIS_CONFIG $agoris_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    agoris_config_minor_version=`$AGORIS_CONFIG $agoris_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    agoris_config_micro_version=`$AGORIS_CONFIG $agoris_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  fi
  AC_SUBST(AGORIS_CFLAGS)
  AC_SUBST(AGORIS_LIBS)
])

