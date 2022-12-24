# SPDX-FileCopyrightText: © 2020 Phantom Developers
# SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
#
# SPDX-License-Identifier: MIT

ACLOCAL_AMFLAGS = -I m4

if OS_WINDOWS_CROSSCOMPILE
# for leveldb
AM_MAKEFLAGS = TARGET_OS=OS_WINDOWS_CROSSCOMPILE
endif

export CC CXX AR LD
SUBDIRS = db4.8/build_unix leveldb src

Phantomwallet.app: all-am
	mkdir -p Phantomwallet.app/Contents/{MacOS,Resources}
	cp Info.plist Phantomwallet.app/Contents
	cp phantomwallet.icns Phantomwallet.app/Contents/Resources
	cp src/phantomwallet Phantomwallet.app/Contents/MacOS/Phantomwallet
	echo "APPL????" > Phantomwallet.app/Contents/PkgInfo
	@MACDEPLOYQT@ Phantomwallet.app @MACDEPLOYQT_LIBPATH@

.PHONY: Phantomwallet.app
