## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_builddir)/src/TFELConfig    \
	      -I$(top_srcdir)/include -I$(srcdir)
if TFEL_WIN
AM_LDFLAGS   = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELConfig.la 
libTFELConfig_la_SOURCES  = GetInstallPath.cxx
libTFELConfig_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELConfig_EXPORTS

EXTRA_DIST = CMakeLists.txt
