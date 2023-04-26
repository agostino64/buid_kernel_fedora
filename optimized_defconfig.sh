#!/bin/bash

CC=clang LLVM=1 ARCH=x86_64 make localmodconfig -j$(nproc)
