dnl Process this file with autoconf to produce a configure script

dnl -----------------------------------------------------
dnl This is the main version definition area.
dnl wpu parse start /^define\(MY_([^,]+),\[([^\]]+)\]\)/
define(MY_PACKAGE,[lmpc])
define(MY_TITLE,[The Little Movie Processing Centre])
define(MY_VERSION,[3.4.0])
define(MY_DATE,[8/1/2004])
define(MY_COMMENT,[alpha test release])
define(MY_QUALITY,[alpha])
define(MY_EMAIL,[uwe@planetquake.com])
dnl wpu parse end
dnl Here is the end of the main version definition area.
dnl -----------------------------------------------------

dnl This defines PACKAGE_BUGREPORT, PACKAGE_NAME, AC_PACKAGE_TARNAME, AC_PACKAGE_VERSION
AC_INIT(MY_TITLE,MY_VERSION,MY_EMAIL,MY_PACKAGE)
dnl get system type
AC_CANONICAL_SYSTEM

USE_CPLUSPLUS=no

dnl I know it. The better way is to bring the package and version
dnl only here but wpu can only parse the upper variant (on two lines).
dnl this defines PACKAGE and VERSION
AM_INIT_AUTOMAKE(AC_PACKAGE_TARNAME,AC_PACKAGE_VERSION)

AM_CONFIG_HEADER(config.h)

dnl automake needs this
dnl AC_ARG_PROGRAM

dnl subst own variables 
DATE="MY_DATE"
COMMENT="MY_COMMENT"
QUALITY="MY_QUALITY"
AC_SUBST(QUALITY)dnl

dnl subst Autoconf variables
AC_SUBST(NETLIBS)dnl
EMPTY=""
AC_SUBST(EMPTY)dnl

dnl insert in config.h.in
AC_DEFINE_UNQUOTED(DATE,"${DATE}",[Define the build date.])
AC_DEFINE_UNQUOTED(COMMENT,"${COMMENT}",[Define a release note.])
AC_DEFINE_UNQUOTED(QUALITY,"${QUALITY}",[Define the release code quality.])
AC_DEFINE_UNQUOTED(HOST,"${host}",[Define the host, where the program should run.])

dnl Checks for programs.
AC_PATH_PROG([BYACC], [byacc], [true], [$PATH:/usr/self/bin])dnl
AC_PROG_CC
# we need CXX in every case, because some stupid tests afterwards use it
# if test $USE_CPLUSPLUS = yes; then
  AC_PROG_CXX
# fi
dnl AC_PATH_PROG(GZIP, gzip, gzip)dnl
AC_PROG_INSTALL
INSTALL_SCRIPT='${INSTALL} -m 755'
AC_SUBST(INSTALL_SCRIPT)dnl
AC_PATH_PROG(JAR, jar)dnl
AC_PATH_PROG(JAVA, java)dnl
AC_PATH_PROG(JAVAC, javac)dnl
AC_PATH_PROG([JFLEX], [jflex], [true], [$PATH:/usr/self/bin])dnl
AM_PROG_LEX
AC_PROG_MAKE_SET
AC_PATH_PROG(PERL, perl)dnl
AC_PROG_RANLIB
AC_PATH_PROG(RM, rm, rm)dnl
AC_PATH_PROG(SED, sed, sed)dnl
AC_PATH_PROG(SGML2HTML, sgml2html, sgml2html)dnl
AC_PATH_PROG(SGML2LATEX, sgml2latex, sgml2latex)dnl
AC_PATH_PROG(SGML2TXT, sgml2txt, sgml2txt)dnl
dnl get the full directory name of the configure stuff
olddir=`pwd`
cd $ac_aux_dir/bin
fulldir=`pwd`
cd $olddir 
AC_PATH_PROG(WPU, wpu, wpu, $fulldir:$PATH)dnl
unset fulldir
AC_PROG_YACC
AC_DEFINE(YY_NO_UNPUT,1,[Define, if unput should not be used, the lex code is much in this way.])
AC_PATH_PROG(ZIP, zip, zip)dnl

# this is specific to my own computer, this should really be checked somehow
FLAVOR_HOME=/usr/self/src/flavor/flavor
AC_MSG_WARN(I fixed FLAVOR_HOME to $FLAVOR_HOME. Please change it.)
AC_SUBST(FLAVOR_HOME)

AC_PATH_PROG(FLAVORC, flavorc, $FLAVOR_HOME/bin/flavorc)dnl

dnl try to add some options on the already calculated GCC CFLAGS
AC_MSG_CHECKING(to see if we can add '-W -Wall' to CFLAGS)
if test $GCC = yes; then
  CFLAGS="$CFLAGS -D_U_=\"__attribute__((unused))\" -W -Wall -pedantic"
  AC_MSG_RESULT(yes)
else
  CFLAGS="-D_U_=\"\" $CFLAGS"
  AC_MSG_RESULT(no)
fi

if test $USE_CPLUSPLUS = yes; then
  dnl try to add some options on the already calculated G++ CFLAGS
  AC_MSG_CHECKING(to see if we can add '-W -Wall' to CXXFLAGS)
  if test $GXX = yes; then
    CXXFLAGS="$CXXFLAGS -D_U_=\"__attribute__((unused))\" -W -Wall -pedantic"
    AC_MSG_RESULT(yes)
  else
    CXXFLAGS="-D_U_=\"\" $CXXFLAGS"
    AC_MSG_RESULT(no)
  fi

  dnl we need the C++ compiler for the following tests
  AC_LANG_CPLUSPLUS
fi

dnl Checks for libraries.
AC_CHECK_LIB(m, main)
AC_CHECK_LIB(nsl,gethostbyname,NETLIBS="$NETLIBS -lnsl",,)
AC_CHECK_LIB(socket,socket,NETLIBS="$NETLIBS -lsocket",,)

dnl Check for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(memory.h sys/ipc.h sys/shm.h sys/time.h string.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
dnl AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MEMCMP
dnl we need a running program to check this. It can't be detected on
dnl cross-compilers
dnl AC_FUNC_SETVBUF_REVERSED


AC_ARG_ENABLE(dbs,
[  --enable-dbs            build dbs.  [default=yes]],enable_dbs=$enableval,enable_dbs=yes)

if test "x$enable_dbs" = "xyes" ; then
	AC_CHECK_FUNC(shmget, dbs_bin="dbs\$(EXEEXT)", dbs_bin="")
else
	dbs_bin=""
fi
AC_SUBST(dbs_bin)


AC_ARG_ENABLE(demcut,
[  --enable-demcut         build demcut.  [default=yes]],enable_demcut=$enableval,enable_demcut=yes)

if test "x$enable_demcut" = "xyes" ; then
	demcut_bin="demcut\$(EXEEXT)"
else
	demcut_bin=""
fi
AC_SUBST(demcut_bin)


AC_ARG_ENABLE(lmpc,
[  --enable-lmpc           build lmpc.  [default=yes]],enable_lmpc=$enableval,enable_lmpc=yes)

if test "x$enable_lmpc" = "xyes" ; then
	lmpc_bin="lmpc\$(EXEEXT)"
else
	lmpc_bin=""
fi
AC_SUBST(lmpc_bin)


AC_CHECK_FUNCS(mkfifo gettimeofday strdup)
dnl AC_CHECK_FUNCS(gethostname select socket strerror strtol)
AC_CHECK_FUNC(getopt_long,,[AC_LIBOBJ(getopt) AC_LIBOBJ(getopt1)])
AC_MSG_CHECKING(for a memmem prototype)
AC_EGREP_HEADER(memmem,string.h,ac_memmem_proto=yes,ac_memmem_proto=no)
if test $ac_memmem_proto = yes; then
  AC_DEFINE_UNQUOTED(HAVE_MEMMEM_PROTOTYPE,1,"Define if you have the memmem prototype.")
fi
AC_MSG_RESULT($ac_memmem_proto)
AC_REPLACE_FUNCS(memmem)
AC_REPLACE_FUNCS(strsep)

dnl enable options, no option do work actually, they must all be enabled
AC_ARG_ENABLE(lmp,
    [  --enable-lmp            enables LMP support (must be enabled)],
    ENABLE_LMP=$enableval
)
: ${ENABLE_LMP=yes}
case "$ENABLE_LMP" in
    n | no) : ;;
    y | ye | yes) 
        AC_DEFINE(ENABLE_LMP,1,"Define if LMP support should be compiled in.")
    ;;
esac

AC_ARG_ENABLE(dmo,
    [  --enable-dmo            enables DMO support (must be enabled)],
    ENABLE_DMO=$enableval
)
: ${ENABLE_DMO=yes}
case "$ENABLE_DMO" in
    n | no) : ;;
    y | ye | yes) 
        AC_DEFINE(ENABLE_DMO,1,"Define if DMO support should be compiled in.")
    ;;
esac

AC_ARG_ENABLE(dem,
    [  --enable-dem            enables DEM support (must be enabled)],
    ENABLE_DEM=$enableval
)
: ${ENABLE_DEM=yes}
case "$ENABLE_DEM" in
    n | no) : ;;
    y | ye | yes) 
        AC_DEFINE(ENABLE_DEM,1,"Define if DEM support should be compiled in.")
    ;;
esac

AC_ARG_ENABLE(qwd,
    [  --enable-qwd            enables QWD support (must be enabled)],
    ENABLE_QWD=$enableval
)
: ${ENABLE_QWD=yes}
case "$ENABLE_QWD" in
    n | no) : ;;
    y | ye | yes) 
        AC_DEFINE(ENABLE_QWD,1,"Define if QWD support should be compiled in.")
    ;;
esac

AC_ARG_ENABLE(dm2,
    [  --enable-dm2            enables DM2 support (must be enabled)],
    ENABLE_DM2=$enableval
)
: ${ENABLE_DM2=yes}
case "$ENABLE_DM2" in
    n | no) : ;;
    y | ye | yes) 
        AC_DEFINE(ENABLE_DM2,1,"Define if DM2 support should be compiled in.")
    ;;
esac

AC_ARG_ENABLE(dm3,
    [  --enable-dm3            enables DM3 support (must not be enabled)],
    ENABLE_DM3=$enableval
)
: ${ENABLE_DM3=no}
case "$ENABLE_DM3" in
    n | no) : ;;
    y | ye | yes) 
        AC_DEFINE(ENABLE_DM3,1,"Define if DM3 support should be compiled in.")
    ;;
esac

dnl set some vars in src/Makefile to switch DM3 support on or off
if test ${ENABLE_DM3} = "yes" ; then
	UDM3_H=udm3.h
	UDM3_C=udm3.c
	UDM3_O=udm3.o
	AC_SUBST(UDM3_H)
	AC_SUBST(UDM3_C)
	AC_SUBST(UDM3_O)
fi

dnl output
AC_OUTPUT([ \
	Makefile bin/Makefile \
	spec/Makefile \
	spec/dem-q1/Makefile \
	spec/dm2-q2/Makefile \
	spec/dmo-dn/Makefile \
	spec/faq/Makefile \
	spec/icons/Makefile \
	spec/lmp-d1/Makefile \
	spec/main/Makefile \
	spec/misc/Makefile \
	spec/qwd-qw/Makefile \
	src/Makefile \
	doc/Makefile \
	bin/dema \
	bin/dm2cat \
	src/java/Makefile \
	src/java/lmpcj \
	src/java/com/Makefile \
	src/java/com/machinima/Makefile \
	src/java/com/machinima/lmpc/Makefile \
	src/java/com/machinima/lmpc/io/Makefile \
	src/java/com/machinima/lmpc/io/base/Makefile \
	src/java/com/machinima/lmpc/io/node/Makefile \
	src/java/com/machinima/lmpc/io/text/Makefile \
	src/java/com/machinima/lmpc/io/bin/Makefile \
	],
[echo timestamp > stamp-h ; chmod +x bin/dema bin/dm2cat src/java/lmpcj])

dnl check warnings
dnl AC_MSG_WARN(build=$build host=$host target=$target)
dnl AC_MSG_WARN('host_cpu=$host_cpu host_vendor=$host_vendor host_os=$host_os')

dnl configure warnings
if test "$ac_cv_header_sys_ipc_h" != yes; then
  AC_MSG_WARN(Your systems lacks IPC. DBS will not be compiled.)
fi

AH_BOTTOM([
/*
 *  With the following definitions I can use always the non-reversed order.
 *  The check to get the SETVBUF_REVERSED value needs a real program run,
 *  which is impossible with cross-compiling. So you have to manually
 *  uncomment the next line, if you suspect something bad with setvbuf.
 */
/* #define SETVBUF_REVERSED 1 */
#if defined(SETVBUF_REVERSED)
#define SETVBUF(stream, buf, mode , size) setvbuf(stream, mode, buf, size)
#else
#define SETVBUF(stream, buf, mode , size) setvbuf(stream, buf, mode, size)
#endif
])
