#!/bin/bash

# Start sever
# ./bin/sv &

# Commands execution
./bin/cl -e "ls"
ls > gol

[ "$(diff log gol)" ] && echo "False" || echo "True"
