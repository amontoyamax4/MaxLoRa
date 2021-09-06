// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _MAXLORA_H_
#define _MAXLORA_H_

#include <Arduino.h>
#include <Stream.h>
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAMD)
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#define DEFAULT_SF 7
#define DEFAULT_FSB 2  //frequency sub band
#define RETX "7"

#define PWRIDX_EU868 "1"
#define PWRIDX_US915 "5"
#define PWRIDX_AU915 "5"
#define PWRIDX_AS923_925 "1" // TODO: should be 0
#define PWRIDX_KR920_923 "1" // TODO: should be 0
#define PWRIDX_IN865_867 "1" // TODO: should be 0

#define BUFFER_SIZE 300//?

typedef uint8_t port_t;

enum response_t
{
  ERROR_SEND_COMMAND_FAILED = (-1),
  ERROR_UNEXPECTED_RESPONSE = (-10),
  SUCCESSFUL_TRANSMISSION = 1,
  SUCCESSFUL_RECEIVE = 2
};

enum fp_t
{
  FP_EU868,
  FP_US915,
  FP_AU915,
  FP_AS923_925,
  FP_KR920_923,
  FP_IN865_867,
  FP_US915AB
};

class MaxLoRa
{
public:
  bool needsHardReset = false;

  MaxLoRa(Stream &modemStream, Stream &debugStream, int resetPin, fp_t fp = FP_US915AB, bool talkative = true, uint8_t sf = DEFAULT_SF, uint8_t fsb = DEFAULT_FSB);
  void autoBaud();
  void reset(bool adr = true);
  void showStatus(void);
  size_t getHardwareEui(char *buffer, size_t size);
  size_t getAppEui(char *buffer, size_t size);
  uint16_t getVDD();
  void onMessage(void (*cb)(const uint8_t *payload, size_t size, port_t port));
  bool provision(const char *appEui, const char *appKey);
  bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000);
  bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
  bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
  bool personalize();
  response_t sendBytes(const uint8_t *payload, size_t length, port_t port = 1, bool confirm = false, uint8_t sf = 0);
  response_t poll(port_t port = 1, bool confirm = false);
  void sleep(uint32_t mseconds);
  void wake(void);
  void saveState(void);
  void linkCheck(uint16_t seconds);
  uint8_t getLinkCheckGateways(void);
  uint8_t getLinkCheckMargin(void);

private:
  Stream *modemStream;
  Stream *debugStream;
  bool talkative;
  int resetPin;
  fp_t fp;
  uint8_t sf;
  uint8_t fsb;
  bool adr;
  char buffer[512];
  bool baudDetermined = false;
  void (*messageCallback)(const uint8_t *payload, size_t size, port_t port);

  void clearReadBuffer();
  size_t readLine(char *buffer, size_t size, uint8_t attempts = 3);
  size_t readResponse(uint8_t prefixTable, uint8_t indexTable, uint8_t index, char *buffer, size_t size);
  size_t readResponse(uint8_t table, uint8_t index, char *buffer, size_t size);

  void debugPrintIndex(uint8_t index, const char *value = NULL);
  void debugPrintMessage(uint8_t type, uint8_t index, const char *value = NULL);

  void configureEU868();
  void configureUS915(uint8_t fsb);
  void configureAU915(uint8_t fsb);
  void configureAS923_925();
  void configureKR920_923();
  void configureIN865_867();
  void configureUS915AB();
  void configureChannels(uint8_t fsb);
  bool setSF(uint8_t sf);
  bool waitForOk();

  void sendCommand(uint8_t table, uint8_t index, bool appendSpace, bool print = true);
  bool sendMacSet(uint8_t index, const char *value);
  bool sendChSet(uint8_t index, uint8_t channel, const char *value);
  bool sendJoinSet(uint8_t type);
  bool sendPayload(uint8_t mode, uint8_t port, uint8_t *payload, size_t len);
  void sendGetValue(uint8_t table, uint8_t prefix, uint8_t index);

};

#endif
