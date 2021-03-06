# DashBash
Amazon Dash button hack using an ESP8266.

This is a simple Arduino sketch for the ESP8266 using the ESP library version 2.4.2.

All the code does is sniff WiFi probe requests and alarm when it recognizes 
a mac address and the sequence number for those packets starts new,
since the sequencing for the packets starts fresh with every Dash button reboot.
The button index is then printed out as a byte through serial.

When settings up such a dash button, just connect it to a hotspot that you close right after that.
I haven't tested otherwise, but ensure that the button can properly connect to the Internet after the initialization and the first press.

That way, if the button is not able to connect to a hotspot, it will send those precious probe requests
and shut down after about two seconds,
saving quite some energy compared to it connecting and trying to reach Amazon servers a few times.

If you simply want to test whether the ESP is receiving the Probe Frames, check out the [test-led](https://github.com/dakhnod/DashBash/tree/test-led) branch
