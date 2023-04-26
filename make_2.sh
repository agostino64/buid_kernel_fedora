#!/bin/bash

CC=clang LLVM=1 ARCH=x86_64 make -j$(nproc) KCFLAGS="-O2 -pipe" KCPPFLAGS="-O2 -pipe" INSTALL_MOD_STRIP=1 binrpm-pkg
