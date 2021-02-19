# Batterytator

A simple C script to monitor your laptop battery without too much cpu & memory footprint.

## Requirements

- `notify-send`
- `mplayer`

## Compile

`gcc -o batterytator ./batterytator.c`

## Usage

Execute the binary with the arguments:

`./batterytator <Battery ID>`

To get the value of `<Battery ID>`, you can list the folders in the `/sys/class/power_supply/`.

## Systemd usage

