# Lenovo Fan Daemon
The custom fan daemon for lenovo ideapad 710s

The idea behind it is to look at the temperature, and if it is low enough, disable fans to keep a laptop quiet.

When the temperature is high, daemon returns fan control back to EFI firmware.

# Build and launch

Build:

    make

Launch:

    ./lenovo_fan_daemon

# Supported Hardware

Only lenovo ideapad 710s is supported. It should be relatively easy to add other models. Check https://github.com/hirschmann/nbfc/tree/master/Configs.
