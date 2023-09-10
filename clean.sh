#!/bin/sh

rm -rf ./build/*
rm -rf ./bin/*

cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
