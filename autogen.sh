#!/bin/sh

#
# Run this to generate all the initial makefiles.
#
# $Id$

aclocal || exit 1
autoheader || exit 1
automake --add-missing || exit 1
autoconf || exit 1
./configure "$@" || exit 1

