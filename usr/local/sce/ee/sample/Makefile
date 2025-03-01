#
# Copyright (C) 1999 Sony Computer Entertainment Inc., All Rights Reserved.
#

SHELL = /bin/sh

all run clean:
	@for dir in `find . -type d -print`; do\
		if [ $$dir != "." ]; then \
			if [ -f $$dir/[Mm]akefile ]; then $(MAKE) -C $$dir $@; fi \
		fi \
	done
