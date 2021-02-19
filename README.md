# Batterytator

A simple C script to monitor your laptop battery without too much cpu & memory footprint.

## Requirements

- `notify-send`
- `mplayer`

## Installation

- Git clone this repository
- Use the command `make && sudo make install` to install the utility on your system

## Usage

Execute the binary with the arguments:

`./batterytator <Battery ID>`

To get the value of `<Battery ID>`, you can list the folders in the `/sys/class/power_supply/`.

## Systemd usage

