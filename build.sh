#!/bin/sh

cmake --build ./build -j12
cp ./build/compile_commands.json ./compile_commands.json
