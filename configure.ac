# Copyright (C) 1999-2005 Open Source Telecom Corporation.
# Copyright (C) 2006-2014 David Sugar, Tycho Softworks.
# Copyright (C) 2015 Cherokees of Idaho.
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([ccrtp], [2.1.2])
AC_CONFIG_SRCDIR([src/ccrtp/rtp.h])

LT_VERSION="3:1:0"
USES_UCOMMON_REQUIRED="6.2.2"

AC_CONFIG_AUX_DIR(autoconf)
AC_CANONICAL_SYSTEM
AC_PROG_CPP
AC_PROG_CXXCPP
AC_PROG_CXX
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AM_INIT_AUTOMAKE([dist-zip])
AM_CONFIG_HEADER(ccrtp-config.h)
AC_CONFIG_MACRO_DIR([m4])

AC_C_RESTRICT
AC_C_VOLATILE
AC_C_INLINE

# SRTP support
AC_ARG_ENABLE(srtp,
    AS_HELP_STRING([--enable-srtp],
        [enable SRTP support (default enabled)]))
if test -z "$enable_srtp"; then
   enable_srtp="yes"
   AC_DEFINE(SRTP_SUPPORT, [], [SRTP support])
fi

# demos (default-enabled)
AC_ARG_ENABLE([demos],
              AS_HELP_STRING([--disable-demos], [Disable compilation of demos]))

AS_IF([test "x$enable_demos" != "xno"],
      [AM_CONDITIONAL(BUILD_DEMOS, true)], [AM_CONDITIONAL(BUILD_DEMOS, false)])

# availability of gcrypt or openSSL crypto library if SRTP support is enabled
if test "${enable_srtp}" = "yes"
then
    openssl="false"
    gcrypt="false"
    AM_PATH_LIBGCRYPT_CCRTP([1:1.2.3],
                  [AC_CHECK_HEADERS([gcrypt.h],
                                    [], AC_MSG_ERROR([libgcrypt headers not found.]))
                   LIBS="$LIBGCRYPT_LIBS $LIBS"
                   gcrypt="true"
                  ],
                  [AC_CHECK_LIB([crypto],
                      [EVP_CipherInit_ex],
                      [AC_CHECK_HEADERS([openssl/bn.h openssl/aes.h openssl/sha.h],
                                        [], AC_MSG_ERROR([OpenSSL headers not found.]))
                       LIBS="-lcrypto $LIBS"
                       openssl="true"
                      ],
                      [AC_MSG_ERROR([Cannot find libgcrypt or OpenSSL crypto library.])]
                     )
                  ]
                 )
    AM_CONDITIONAL(SRTP_OPENSSL, test "$openssl" = "true")
    AM_CONDITIONAL(SRTP_GCRYPT, test "$gcrypt" = "true")

fi

OST_PROG_COMMON
OST_PROG_LIBVER
OST_PROG_LIBRARY(CCXX,[$LT_VERSION])
OST_AUTOMAKE_MODE
OST_MAINTAINER_MODE
PKG_CHECK_MODULES(COMMONCPP, commoncpp >= $USES_UCOMMON_REQUIRED)
CXXFLAGS="$CXXFLAGS $COMMONCPP_CFLAGS"
GNULIBS="$COMMONCPP_LIBS $LIBS"
AC_SUBST(GNULIBS)
# OST_CC_ENDIAN - now in Common C++
# Are we using the GNU compiler?
if test $GCC = yes ; then
    WARN_FLAGS="-fno-strict-aliasing -Wall -ansi -pedantic"
else
    WARN_FLAGS=""
fi
AC_SUBST(WARN_FLAGS)

OST_DEBUG

dnl --with-nana: check for nana, the GNU assertion checking and logging library
AC_ARG_WITH(nana,[  --with-nana             use GNU nana for asserts and logging],[
    AC_CHECK_HEADER(nana.h, [
        AC_CHECK_HEADERS(DL.h, [
            AC_CHECK_LIB(nana, main)
            ])
    ])
])

KDOC_DIR="\$(top_srcdir)/doc"
AC_SUBST(KDOC_DIR)
AC_SUBST(LT_VERSION)
AC_SUBST(USES_UCOMMON_REQUIRED)

AC_PATH_PROG(DOXYGEN, doxygen, no)
AC_SUBST(DOXYGEN)
AM_CONDITIONAL(DOXY, test "$DOXYGEN" != "no")

AC_OUTPUT(Makefile m4/Makefile src/Makefile src/ccrtp/Makefile PKGBUILD
doc/Makefile demo/Makefile phone/Makefile w32/Makefile ccrtp.spec 
ccrtp.list libccrtp.pc src/ccrtp/crypto/Makefile directive)
