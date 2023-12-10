#!/bin/bash

echo "Starting installation."

echo "Created dirs."
mkdir out
mkdir obj

echo "Running make."
make

echo "Done."

echo "Removing object files."
rm -r obj

echo "Done."

