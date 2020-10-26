#!/bin/bash

meshfile="$1"
dbfile="$(basename --suffix=.mesh $meshfile).clj"
meshhead=$(head -1 $meshfile)

(
    echo "(def vertices ["
    sed -nE "2,$(( $meshhead + 1 )) p" $meshfile \
        | sed -e 's/^/\t\[/' -e 's/$/\]/' 

    echo "])"

    echo "(def triangles ["
    sed -n "$(( $meshhead + 3 )),$ p" $meshfile \
        | sed -e 's/^/\t\[/' -e 's/$/\]/' 

    echo "])"
) | tee $dbfile
