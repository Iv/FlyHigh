#!/bin/sh

# flyhigh man page creation script
# writes the flyhigh man page in troff format to stdout
# For viewing the man page, run these commands:
#   ./flyhigh-manpage.sh 9.9.9 "February 31, 2099" > flyhigh.1
#   gzip -9 flyhigh.1
#   man -l flyhigh.1.gz
#
# Arguments:
#   $1 version string
#   $2 date string

cat <<EOF
.TH flyhigh 1 "$2" "version $1" "USER COMMANDS"
.SH NAME
flyhigh \- flight preparation and analysis software
.SH SYNOPSIS
.B flyhigh
.SH DESCRIPTION
flyhigh is a unique software for Linux and Windows. It helps paraglider pilots to
.br
- configure gps-enabled flight instruments
.br
- manage and visualize flights, waypoints and routes
.br
- plan and optimize their next awesome FAI triangles!

See http://flyhigh.sourceforge.net for features.
.SH OPTIONS
flyhigh does not offer command-line options of its own;
As a Qt application it supports Qt options; please see:
.br
http://doc.qt.digia.com/qt/qapplication.html
.SH AUTHOR
Alex Graf (grafal (at) sourceforge.net)
.br
Juerg Allemann (ja_kern (at) sourceforge.net)
EOF
