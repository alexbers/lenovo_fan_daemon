# Lenovo Fan Daemon
A custom fan daemon for Lenovo Slim 9 and IdeaPad 710s

An idea behind it is to look at the temperature, and if it is low enough, disable fans to keep a laptop quiet.

When the temperature is high, the daemon returns fan control back to EFI firmware.

# Build, Install and Launch

Build:

    make

Install:
    
    make install

Launch:

    ./lenovo_fan_daemon

Launch on boot:

    systemctl enable lenovo_fan_daemon

# Supported Hardware

Only Lenovo Slim 9 and IdeaPad 710s are supported. If you have IdeaPad you should put zero to IS_LENOVO_SLIM9 (fans.c).

It should be relatively easy to add other models. Check https://github.com/hirschmann/nbfc/tree/master/Configs.
