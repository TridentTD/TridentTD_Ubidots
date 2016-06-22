/*

 [TridentTD] : MANABU's Esp8266 IoT Library
 www.facebook.com/miniNodeMCU
 
 TridentTD_Ubidots.h - A simple client for UBIDOTS

 20/06/2559 Buddism Era  (2016)
 

Copyright (c) 2016 TridentTD

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#ifndef _TRIDENTTD_UBIDOTS_H_
#define _TRIDENTTD_UBIDOTS_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

//#define  UBIDOTS_DEBUG_MODE

#ifdef UBIDOTS_DEBUG_MODE
    #define DEBUG_PRINTER Serial
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...) { }
    #define DEBUG_PRINTLN(...) { }
#endif


typedef struct Value {
  String variable_id;
  float  value_id;
} Value;


typedef struct Variable {
  String variable_id;
  String variable_name;
  time_t timestamp;
  float  last_value;
} Variable;

class TridentTD_Ubidots {
   public:
      TridentTD_Ubidots(char* token);
      bool  wificonnect(char *ssid, char *pass);
      bool  loadAll();
      float getValue(String variable_name);
      void  setValue(String variable_name, double value);
      bool  sendAll();
	  String getVersion();
   private:
	  String _version="[Trident_Ubidots] Version 1.0.0";
      char*   _token;
      uint8_t maxValues;        // maxValues for send
      uint8_t currentValue;
      Value * val;
      
      uint8_t _variable_count;
      Variable * ubidotsVar;

      HTTPClient       _http;
};
#endif /* _TRIDENTTD_UBIDOTS_H_ */
