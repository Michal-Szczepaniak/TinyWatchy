# Getting started


## Prerequisites

There are some command line tools, we will need to be able to bootstrap. The following package names are for Ubuntu 22.04, you should be able to figure the corresponding out on any other system.

	apt install libfreetype-dev imagemagick

## Building your private defines file

There are some private settings, that you will need to define, these are store in a private C/C++ header. This file is excluded from version tracking to prevent accidental leaking of your credentials. You need to generate your own from the template.

	# in the repository root directory
	cp src/defines_private.h.template src/defines_private.h

## Building resources

Image resources have to be converted to hex representations so they can be used in the C/C++ code.

	# in the repository root directory
	cd resources
	bash generate.sh

## Building the binary image for the watch

	# in the repository root directory
	pio run

## Uploading the image to the watch

	# in the repository root directory
	pio run -t upload

