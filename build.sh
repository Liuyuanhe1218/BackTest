#!/bin/bash

cd build/
cmake ..
cmake --build .
cp backtest ..
cd ..