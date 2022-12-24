# SPDX-FileCopyrightText: © 2020 Phantom Developers
# SPDX-FileCopyrightText: © 2019 SpectreCoin Developers
# SPDX-License-Identifier: MIT

target_sources(phantomwallet_lib
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/lz4.h

        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/lz4.c
        )
