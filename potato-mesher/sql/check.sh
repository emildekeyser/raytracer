#!/bin/bash

meshfile="$1"
dbfile="$(basename --suffix=.mesh $meshfile).db"

echo mesh:
grep '^[[:graph:]]*$' $meshfile

echo db:
sqlite3 $dbfile <<< "select count(*) from vertex;select count(*) from triangle;" | grep '[[:digit:]]'
