# Time-stamp: <05/09/09 21:12:16 ptr>
# $Id: sys.mak 1802 2005-11-01 08:25:57Z complement $

INSTALL := $$HOME/bin/install-sh

INSTALL_SO := ${INSTALL} -c -m 0755
INSTALL_A := ${INSTALL} -c -m 0644
INSTALL_EXE := ${INSTALL} -c -m 0755

EXT_TEST := /usr/bin/test
