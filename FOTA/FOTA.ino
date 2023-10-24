/**
   StreamHTTPClient.ino

    Created on: 24.05.2015

*/
/***********************************************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
/***********************************************************************************************************/
#define SPI_MSG_ID_RESET      0x00
#define SPI_MSG_ID_START      0x01
#define SPI_MSG_ID_END        0x02
#define SPI_MSG_ID_DATA       0x03

/***********************************************************************************************************/
class ESPSafeMaster {
private:
  uint8_t _ss_pin;
  void _pulseSS() {
    digitalWrite(_ss_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_ss_pin, LOW);
  }

public:
  ESPSafeMaster(uint8_t pin)
    : _ss_pin(pin) {}
  void begin() {
    pinMode(_ss_pin, OUTPUT);
    _pulseSS();
  }

  void sendToTiva(uint8_t msgID, uint8_t data)
  {
    _pulseSS();
    SPI.transfer(msgID);
    SPI.transfer(data);
    _pulseSS();
  }
};

/***********************************************************************************************************/
ESP8266WiFiMulti WiFiMulti;
ESPSafeMaster esp(SS);
uint8_t dataArray[6 * 4 * 1024] = { 0 };

/***********************************************************************************************************/
void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Ehab's PC", "1223334444");

  SPI.begin();
  esp.begin();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;  // must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

    Serial.print("\n[HTTP] begin...\n");

    // configure server and url
    http.begin(client, "http://fotaserver.freevar.com@fotaserver.freevar.com/firmware/0.0.1/test.0.0.1.bin");
    // http.begin(client, "jigsaw.w3.org", 80, "/HTTP/connection.html");

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
        // get length of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();
        // Serial.println("Len: " + String(len));

        // create buffer for read
        char buff[1024] = { 0 };
        int dataRecievedLength = 0;

        // get tcp stream
        WiFiClient* stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // read up to 128 byte
          int c = stream->readBytesUntil('\n', buff, std::min((size_t)len, sizeof(buff)));
          // Serial.printf("[INFO] readBytes: %d\n", c);
          if (!c) { Serial.println("[HTTP] read timeout"); }

          // write it to Serial
          // Serial.print("C = "+String(c)+'\t');
          // Serial.write(buff, c);

          dataArray[dataRecievedLength++] = strtol(buff, NULL, 16);
          // Serial.print("\ttest = "+String(test));


          // Serial.print('\n');
          if (len > 0) { len -= c; }
        }

        Serial.print("[HTTP] connection closed or file end.\n");
        digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off (LOW is the voltage level)

        // send reset request
        Serial.printf("[INFO] Sending RESET signal to Tiva-C with msgID = %#2X \n", SPI_MSG_ID_RESET);
        esp.sendToTiva(SPI_MSG_ID_RESET, 0);
        delay(5);
        
        // send start request
        Serial.printf("[INFO] Sending START signal to Tiva-C with msgID = %#2X \n", SPI_MSG_ID_START);
        esp.sendToTiva(SPI_MSG_ID_START, 0);
        delayMicroseconds(50);
        
        for (int c = 1; c < dataRecievedLength; c++) {
          // Serial.printf("[INFO] Sending array[%d] = %#2X \tto Tiva-C with msgID = %#2X \n", c, dataArray[c], SPI_MSG_ID_DATA);
          esp.sendToTiva(SPI_MSG_ID_DATA, dataArray[c]);
          delayMicroseconds(50);
        }

        // send end request
        Serial.printf("[INFO] Sending END signal to Tiva-C with msgID = %#2X \n", SPI_MSG_ID_END);
        esp.sendToTiva(SPI_MSG_ID_END, 0);
        delayMicroseconds(50);
        

        Serial.println("[INFO] size of array: " + String(dataRecievedLength - 1));
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(30000);
}


// char* token;

// void ConvertStrToArr(String hexString, unsigned char test[], unsigned int* length) {
//   const char dilm[] = "\n";
//   token = strtok((char*)hexString.c_str(), dilm);
//   token = strtok(NULL, dilm);
//   int index = 0;

//   while (token != NULL) {
//     // Convert the hexadecimal string to a uint8_t
//     test[index] = strtol(token, NULL, 16);
//     (*length)++;

//     // Move to the next token
//     token = strtok(NULL, dilm);
//     index++;
//   }

//   // Now, test[] contains the converted values.
//   // You can use them as needed.
//   // for (int i = 0; i < *length; i++)
//   // {
//   //   Serial.print("test[");
//   //   Serial.print(i);
//   //   Serial.print("] = 0x");
//   //   Serial.println(test[i], HEX);
//   // }
// }
