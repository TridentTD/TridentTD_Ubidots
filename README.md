##[TridentTD] Manabu's Esp8266 IoT Library
#TridentTD_Ubidots

TridentTD_Ubidots is a library for
http://ubidots.com/ to  receive or send data in the simplest method.

You can use ubidots client like this

- TridentTD_Ubidots ubidots;
- ubidots.wificonnect(ssid,password);
- ubidots.loadAll();
- ubidots.getValue( ubidots_varaible_name);
- ubidots.setValue( ubidots_variable_name, value);
- ubidots.sendAll();
- ubidots.getVersion();

##Version
1.0.0
