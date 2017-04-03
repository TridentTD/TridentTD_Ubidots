TridentTD_Ubidots
=================

[TridentTD] Manabu's Esp8266 IoT Library
----------------------------------------

TridentTD_Ubidots is a library for
http://ubidots.com/ to  receive or send data in the simplest method.

You can use ubidots client like this

- TridentTD_Ubidots ubidots;
- ubidots.wificonnect( ssid,password );
- ubidots.loadAll();
- ubidots.setDevice( ubidots\_device_name );
- ubidots.getLastValue( ubidots\_varaible_name );
- ubidots.getLastValue( ubidots\_device_name, ubidots\_varaible_name );
- ubidots.getLastTimeStamp( ubidots\_varaible_name );
- ubidots.getLastTimeStamp( ubidots\_device_name, ubidots\_varaible_name );
- ubidots.setValue( ubidots\_variable_name, value);
- ubidots.setValue( ubidots\_device_name, ubidots\_variable_name, value);
- ubidots.sendAll();
- ubidots.getVersion();

Example
=======
Method 1
--------

* ubidots.wificonnect( ssid,password );
* ubidots.loadAll();
* ubidots.getLastValue( ubidots\_device_name, ubidots\_varaible_name );
* ubidots.setValue( ubidots\_device_name, ubidots\_variable_name, value);
* ubidots.sendAll();

Method 2
--------

* ubidots.wificonnect( ssid,password );
* ubidots.setDevice( ubidots\_device_name );
* ubidots.getLastValue( ubidots\_varaible_name );
* ubidots.setValue( ubidots\_variable_name, value);
* ubidots.sendAll();


Version
=======
1.0.3  for Ubidots 2017
