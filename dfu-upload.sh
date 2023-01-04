#!/bin/bash

if [[ $# != 1 ]] ; then
	echo "Usage: $0 <firmware>.hex"
	exit 1
fi

set -exuo pipefail
MCU=atmega16u2
TARGET=$1
dfu-programmer $MCU erase || true
dfu-programmer $MCU flash $TARGET
dfu-programmer $MCU reset
