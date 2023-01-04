# Run "make help" for target help.

MCU          = atmega16u2
ARCH         = AVR8
BOARD        = NONE
F_CPU        = 16000000
OPTIMIZATION = 2
TARGET       = TRNGSerial
SRC          = $(TARGET).c
DMBS_PATH    = ./DMBS/DMBS
CC_FLAGS     =
LD_FLAGS     =

default: docker-build dfu

# Include common DMBS build system modules
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk

.docker_image: Dockerfile
	docker build -t avr_gcc .
	docker image ls -q avr_gcc > .docker_image

docker-build: .docker_image
	docker run -ti --rm -v $(shell pwd):/build avr_gcc make all
