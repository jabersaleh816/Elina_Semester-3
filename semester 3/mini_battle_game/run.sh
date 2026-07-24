#!/bin/bash
cd "$(dirname "$0")"
mkdir -p output
g++ -Wall -Wextra -std=c++17 main.cpp -o output/main
./output/main
