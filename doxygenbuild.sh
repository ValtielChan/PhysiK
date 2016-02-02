#!/bin/sh
doxygen docoxygen.config
echo "<html><head><meta http-equiv="refresh" content='0; URL=./html/index.html'></head><body></body></html>" > index.html