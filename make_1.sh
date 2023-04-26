#!/bin/bash

CC=clang LLVM=1 ARCH=x86_64 make -j$(nproc) KCFLAGS="-march=native -O2 -pipe" KCPPFLAGS="-march=native -O2 -pipe" INSTALL_MOD_STRIP=1 binrpm-pkg
