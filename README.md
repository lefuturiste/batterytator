# Batterytator

A simple C script to monitor your laptop battery without too much cpu & memory footprint.

## Requirements

- `notify-send`, to send desktop notifications
- `mplayer`, to play music file

## Installation

- Git clone this repository
- Get the battery ID for your laptop:
  - List the folders in the `/sys/class/power_supply/`.
  - Usually the right ID is `BAT1` or something like that
- Use the command `env BAT_ID=YOUR_BAT_ID_HERE make && sudo make install` to install the utility on your system

**I recommand to not use systemd but instead to just start batterytator from .xinitrc, so it gets access to notify-send**

To do that, you have to append this line to your `~/.xinitrc :

```
# Start batterytator
/usr/bin/batterytator {YOUR_USER} {YOUR_BAT_ID} &
```

## Uninstallation

Use the command `sudo make uninstall` to wipe out batterytator from your system

## Usage

### Start the service

Once you installed batterytator you can still start/stop the service:

- Start: `sudo systemctl start batterytator`
- Stop: `sudo systemctl start batterytator`

By default batterytator will run at startup you can still change this behavious:

- Enable launch at startup: `sudo systemctl enable batterytator`
- Remove launch at startup: `sudo systemctl disable batterytator`

## Standalone usage

Execute the binary with the arguments:

`./batterytator <User> <Battery ID>` or `/usr/bin/batterytator <User> <Battery ID>`

## Logs

For now, logs are stored under `/home/{USER}/batterytator.log`, as you can tell not the best location.
