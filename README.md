# DashBash
Amazon Dash button press sniffer using an ESP8266

this is a simple sketch for the ESP8266 using the ESP library version 2.4.2

All the code does is sniff WiFi probe requests and alarm when it recognizes 
a mac address and the sequence number for those packets start from new,
since the sequencing for the packets starts fresh with every Dash button reboot.


When settings up such a dash button, just connect it to a hotspot that you close right after that.
I haven't tested otherwise, but ensure that the button can properly connect to the Internet after the initialization and the first press.

That way, if the button is not able to connect to a hotspot, it will shut down after about two seconds,
saving quite some energy compared to it connecting and trying to reach Amazon servers a few times,
yet sending some probe request packets before shutting down.