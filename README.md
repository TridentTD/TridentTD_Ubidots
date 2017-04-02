##[TridentTD] Manabu's Esp8266 IoT Library
#TridentTD_Ubidots

TridentTD_Ubidots is a library for
http://ubidots.com/ to  receive or send data in the simplest method.

You can use ubidots client like this

- TridentTD_Ubidots ubidots;
- ubidots.wificonnect( ssid,password );
- ubidots.loadAll();
- ubidots.setDevice( ubidots\_device_name )
- ubidots.getLastValue( ubidots\_varaible_name );
- ubidots.getLastTimeStamp( ubidots\_varaible_name );
- ubidots.setValue( ubidots\_variable_name, value);
- ubidots.sendAll();
- ubidots.getVersion();

##Version
1.0.3  for Ubidots 2017