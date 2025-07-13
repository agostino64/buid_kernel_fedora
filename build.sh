#!/bin/bash

echo "build"
make -C ./build_fedora

echo "run"
./build_fedora.elf
