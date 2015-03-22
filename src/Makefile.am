include_HEADERS=vitamtp.h

# ---------------------------------------------------------------------------
# Advanced information about versioning:
#   * "Writing shared libraries" by Mike Hearn
#      http://plan99.net/~mike/writing-shared-libraries.html
#   * libtool.info chapter "Versioning"
#   * libtool.info chapter "Updating library version information"
# ---------------------------------------------------------------------------
# Versioning:
#  - CURRENT (Major):  Increment if the interface has changes. AGE is always
#                      *changed* at the same time.
#  - AGE (Micro):      Increment if any interfaces have been added; set to 0
#		         if any interfaces have been removed. Removal has
#                      precedence over adding, so set to 0 if both happened.
#                      It denotes upward compatibility.
#  - REVISION (Minor): Increment any time the source changes; set to
#			 0 if you incremented CURRENT.
#
#  To summarize. Any interface *change* increment CURRENT. If that interface
#  change does not break upward compatibility (ie it is an addition),
#  increment AGE, Otherwise AGE is reset to 0. If CURRENT has changed,
#  REVISION is set to 0, otherwise REVISION is incremented.
# ---------------------------------------------------------------------------
CURRENT=4
AGE=1
REVISION=0
SOVERSION=$(CURRENT):$(REVISION):$(AGE)
LT_CURRENT_MINUS_AGE=`expr $(CURRENT) - $(AGE)`

if COMPILE_MINGW32
W32_CFLAGS=-mno-ms-bitfields
W32_LDFLAGS=-lws2_32
endif

# shared library for installing
lib_LTLIBRARIES=libvitamtp.la
libvitamtp_la_SOURCES=vitamtp.h datautils.c device.c ptp.c usb.c vitamtp.c wireless.c
if COMPILE_MINGW32
libvitamtp_la_SOURCES += asprintf.c socketpair.c
endif
libvitamtp_la_CFLAGS=-std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-parentheses $(XML_CFLAGS) $(LIBUSB_CFLAGS) $(DEVICE_CFLAGS) $(PTHREAD_CFLAGS) $(W32_CFLAGS)
libvitamtp_la_LDFLAGS=$(XML_LIBS) $(LIBUSB_LIBS) $(PTHREAD_LIBS) -no-undefined -export-symbols-regex "VitaMTP_[0-9A-Za-z_]+" -version-info $(SOVERSION) $(W32_LDFLAGS)
libvitamtp_la_LIBADD=$(LTLIBICONV)

EXTRA_DIST=ptp-pack.c device-flags.h ptp.h
DISTCLEANFILES=_stdint.h gphoto2-endian.h
