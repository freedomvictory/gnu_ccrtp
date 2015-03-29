# Copyright (C) 2000-2005 Open Source Telecom Corporation.
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

ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = autogen.sh ccrtp.spec ccrtp.spec.in COPYING.addendum \
    ccrtp.list ccrtp.list.in libccrtp.pc libccrtp.pc.in autoconf/* \
    ccrtp-config.h directive.in PKGBUILD.in CMakeLists.txt \
    cmake/Modules/*.cmake *.cmake cmake-abi.sh

SUBDIRS = src doc
DIST_SUBDIRS = m4 src phone doc demo w32
if BUILD_DEMOS
SUBDIRS += demo
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libccrtp.pc

srpm:	dist
	rm -f *.rpm
	rpmbuild  -bs --nodeps --define "_sourcedir ." --define "_srcrpmdir ." --sign ccrtp.spec

deb:	dist
	rm -f *.deb *.debian.tar.gz *.dsc *.changes
	cape-source --sign ccrtp-${VERSION}.tar.gz .

dist-hook:  cleandist

cleandist:
	@rm -f ${PACKAGE}-[0-9]*.tar.gz
	@rm -f ${PACKAGE}-[0-9]*.tar.bz2
	@rm -f ${PACKAGE}-[0-9]*.shar.gz
	@rm -f ${PACKAGE}-[0-9]*.zip
	@rm -f *${PACKAGE}*.rpm
	@rm -f *${PACKAGE}*.deb
	@rm -f ${PACKAGE}_*.gz
	@rm -f ${PACKAGE}_*.dsc
	@rm -f *.changes

