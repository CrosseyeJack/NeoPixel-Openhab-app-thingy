#!/bin/sh
gcc lights.c -o bin/ledlights
sudo chown root:root bin/ledlights
sudo chmod 4755 bin/ledlights
