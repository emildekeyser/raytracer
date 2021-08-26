#!/bin/bash

meshfile="$1"
dbfile="$(basename --suffix=.mesh $meshfile).py"
meshhead=$(head -1 $meshfile)

(
    echo "vertices = ["
    sed -nE "2,$(( $meshhead + 1 )) p" $meshfile \
        | sed -e 's/^/\t\(/' -e 's/ /, /g' -e 's/$/\),/' -e '$ s/,$//'

    echo "]"

    echo "triangles = ["
    sed -n "$(( $meshhead + 3 )),$ p" $meshfile \
        | sed -e 's/^/\(/' -e 's/ /, /g' -e 's/$/\),/' -e '$ s/,$//'
    echo "]"
) > $dbfile
