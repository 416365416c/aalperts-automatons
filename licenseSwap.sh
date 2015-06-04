#!/bin/bash
#replaces license headers en-masse
#usage: licenceSwap.sh new-license files...
#note that new-license is a text filename containing the text, not the text itself

if (( $# < 2 )); then
    echo "Usage: licenceSwap.sh new-license files...";
fi;

NEW_TEXT=$1
AWK_PROG="BEGIN {top=1} /^[^\*]+$/ {top=0} !top {print} top && /\*\/$/ {top=0} top && /^\/?\*/ {}"
shift
while (( $# > 0 )); do
    TMP_FILE="$1.tmp"
    awk -e "$AWK_PROG" $1 > $TMP_FILE #Surgically remove old header
    cat $NEW_TEXT $TMP_FILE > $1 #Prepend new header and overwrite old file
    rm $TMP_FILE
    shift
done
