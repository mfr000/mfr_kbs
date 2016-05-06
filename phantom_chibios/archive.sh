#!/bin/bash

BDIR=t${1}

if [[ -z $1 ]]; then
    echo "Supply a string to use in directory creation"
    exit 1
fi

if [ -d "$BDIR" ]; then
    echo "${BDIR} already exists. Choose again."
    exit 1
fi

mkdir ${BDIR}
cp -p matrix.c ${BDIR}
cp -p *.h ${BDIR}
cp -p Makefile ${BDIR}


