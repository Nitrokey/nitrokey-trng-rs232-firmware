
# Nitrokey TRNG RS232 Firmware

[Infnoise](https://github.com/waywardgeek/infnoise) inspired serial TRNG implementation for NetHSM

## Production

- Download the firmware from the release page
- Download the dfu-upload.sh script and place it in the same folder
- Connect a device that you want to flash via USB
- Run ```./dfu-upload.sh firmware```

## Getting Started

- [Install Docker](https://docs.docker.com/engine/install/)

- Install DFU-Programmer:
  - Linux: `$ apt-get install -y dfu-programmer`
  - MacOS: `$ brew install dfu-programmer`

## Build & Upload

Go to src directory:

`$ cd src`

### Build:

`$ make docker-build`

### Flash:

1. Get the Atmega16u2 into DFU-mode
2. `$ make dfu`

### Build & Flash:

1.	Get the Atmega16u2 into DFU-mode
2.	`$ make`

On some systems the `make dfu` command needs `sudo` privilege.

### DFU mode:

New (previously unflashed) Atmega16u2 always boot directly in DFU mode.

On development boards:
1. permanently close JP1
2. connect board over USB
3. shortly close JP2

## Monitor random data output

Linux:

```
$ stty 115200 raw cs8 -cstopb -parenb -F /dev/[serial device]
$ xxd /dev/[serial device]
```

MacOS:

```
$ (stty 115200 raw cs8 -cstopb -parenb; xxd) </dev/[serial device]
```

Serial device would be something like  `ttyS0` or `cu.usbserial-XXX`.

### Check entropy

With the `ent` tool the entropy can be estimated:

Linux:
```
$ dd if=/dev/[serial device] iflag=fullblock bs=1K count=100 | ent 
```

MacOS:
```
(stty 115200 raw cs8 -cstopb -parenb; dd iflag=fullblock bs=1K count=100) </dev/[serial device] | ent
```

This should result in output similar to:

```
100+0 records in
100+0 records out
102400 bytes transferred in 9.188599 secs (11144 bytes/sec)
Entropy = 7.998317 bits per byte.

Optimum compression would reduce the size
of this 102400 byte file by 0 percent.

Chi square distribution for 102400 samples is 238.57, and randomly
would exceed this value 76.25 percent of the times.

Arithmetic mean value of data bytes is 127.5779 (127.5 = random).
Monte Carlo value for Pi is 3.140982070 (error 0.02 percent).
Serial correlation coefficient is -0.000348 (totally uncorrelated = 0.0).
```

## Hardware

The [schematic](https://github.com/Nitrokey/nitrokey-trng-rs232-hardware/blob/main/trng-serial.pdf) can be found insed the [hardware repository](https://github.com/Nitrokey/nitrokey-trng-rs232-hardware).

![Pinout top view](.images/NK_TRNG-Serial_Pinout.png)

![NK TRNG-Serial Hardware](.images/NK_TRNG-Serial.jpg)

## [License](./LICENSE)
