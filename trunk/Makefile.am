## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

gpg_bodydocdir = ${prefix}/doc/gpg_body


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(gpg_bodydoc_DATA) \
	$(INTLTOOL_FILES)

data_DATA = MANUAL.odt \
	MANUAL.pdf

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gpg_bodydocdir)
