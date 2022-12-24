:: SPDX-FileCopyrightText: © 2020 Phantom Developers
:: SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
::
:: SPDX-License-Identifier: MIT
::
:: Wrapper script to define all requirements

set PHMWALLET_VERSION=4.2.0

set QTDIR=C:\Qt\5.15.0\msvc2019_64

call scripts\win-genbuild.bat
call scripts\win-build.bat
