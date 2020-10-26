#!/bin/bash

meshfile="$1"
dbfile="$(basename --suffix=.mesh $meshfile).db"
meshhead=$(head -1 $meshfile)

(
    echo "create table vertex (id integer, x real, y real, z real);"
    echo "insert into vertex values"
    sed -nE "2,$(( $meshhead + 1 )) p" $meshfile \
	| nl --starting-line-number=0 --number-separator=' ' --number-width=1 \
        | sed -e 's/^/\(/' -e 's/ /, /g' -e 's/$/\),/' -e '$ s/,$/;/'


    echo "create table triangle (v1 integer, v2 integer, v3 integer);"
    echo "insert into triangle values"
    sed -n "$(( $meshhead + 3 )),$ p" $meshfile \
        | sed -e 's/^/\(/' -e 's/ /, /g' -e 's/$/\),/' -e '$ s/,$/;/'
) | sqlite3 $dbfile
