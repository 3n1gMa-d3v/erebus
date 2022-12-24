#!/bin/bash
# ===========================================================================
#
# SPDX-FileCopyrightText: © 2020 Phantom Developers
# SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
# SPDX-License-Identifier: MIT
#
# Created: 2019-01-22 HLXEasy
#
# Helper script to create checksums for given file (1st parameter)
# and write them into given another file (2nd parameter)
#
# ===========================================================================

releaseDescription=$1
workspace=$2
jobURL=$3
accessToken=$4

if test -e "${releaseDescription}" ; then
    cp "${releaseDescription}" "${workspace}"/releaseNotesToDeploy.txt
else
    echo "### ${releaseDescription}" > "${workspace}"/releaseNotesToDeploy.txt
fi
for currentChecksumfile in \
    Checksum-Phantom-Android-APK.txt \
    Checksum-Phantom-Android-AAB.txt \
    Checksum-Phantom-CentOS-8.txt \
    Checksum-Phantom-Debian-Buster.txt \
    Checksum-Phantom-Debian-Stretch.txt \
    Checksum-Phantom-Fedora.txt \
    Checksum-Phantom-OpenSUSE-Tumbleweed.txt \
    Checksum-Phantom-Mac.txt \
    Checksum-Phantom-Mac-OBFS4.txt \
    Checksum-Phantom-RaspberryPi-Buster.txt \
    Checksum-Phantom-RaspberryPi-Buster-aarch64.txt \
    Checksum-Phantom-RaspberryPi-Stretch.txt \
    Checksum-Phantom-Ubuntu-18-04.txt \
    Checksum-Phantom-Ubuntu-20-04.txt \
    Checksum-Phantom-Win64.txt \
    Checksum-Phantom-Win64-OBFS4.txt \
    Checksum-Phantom-Win64-Qt5.12.txt \
    Checksum-Phantom-Win64-Qt5.12-OBFS4.txt \
    Checksum-Phantom-Win64-Qt5.9.6.txt \
    Checksum-Phantom-Win64-Qt5.9.6-OBFS4.txt ; do
    curl -X POST -L --user "${accessToken}" "${jobURL}"/artifact/${currentChecksumfile} --output ${currentChecksumfile} || true
    if [[ -e "${currentChecksumfile}" ]] && [[ $(wc -l < "${currentChecksumfile}") -eq 1 ]] ; then
        archiveFilename=$(cut -d ' ' -f1 ${currentChecksumfile})
        checksum=$(cut -d ' ' -f2 ${currentChecksumfile})
        echo "**${archiveFilename}:** \`${checksum}\`" >> "${workspace}"/releaseNotesToDeploy.txt
        echo '' >> "${workspace}"/releaseNotesToDeploy.txt
    fi
done
