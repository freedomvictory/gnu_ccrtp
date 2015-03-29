# Copyright (C) 2002-2005 Open Source Telecom Corporation.
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

AUTOMAKE_OPTIONS = no-dependencies dist-shar
ccxxincludedir=$(includedir)/ccrtp/crypto

noinst_HEADERS = SrtpSymCrypto.h hmac.h macSkein.h skein.h skein_iv.h skein_port.h skeinApi.h \
		brg_endian.h brg_types.h twofish.h

kdoc_headers = SrtpSymCrypto.h hmac.h

kdoc:
	kdoc -f html -d $(KDOC_DIR) -L$(KDOC_DIR) -n ccrtp $(kdoc_headers)
