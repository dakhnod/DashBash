#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define WIFI_SSID "XXXXXX"
#define WIFI_PASS "xxxxxx"
#define HTTP_ENDPOINT "http://example.com/"

extern "C" {
#include "user_interface.h"
  typedef void (*freedom_outside_cb_t)(uint8 status);
  int  wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
  void wifi_unregister_send_pkt_freedom_cb(void);
  int  wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}

unsigned int channel = 1;

const uint8_t targets[][6] = { // collection of target macs, find the mac address by e.g. airodump-ng
  {0x6c, 0x56, 0x97, 0x0a, 0xd4, 0x17},
  //{0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
  //...
};
uint16_t seqNumbers[sizeof(targets) / sizeof(targets[0])]; // last packet sequence number store
int pressedButton = -1; // flag indicating the last pressed button

void receiveCallback(uint8_t*, uint16_t);
void onButtonPress(uint8_t);

// this method gets called when a butotn press is detected and the ESP is connected to a WiFi
// after the execution of this function the ESP goes back to promiscous mode
void onWifiConnected() {
  HTTPClient http;
  http.begin(HTTP_ENDPOINT);
  http.GET();
  http.end();
}

void setupPromiscous() {
  WiFi.disconnect();
  yield();
  wifi_set_opmode(STATION_MODE); //promiscous mode setup
  wifi_set_channel(channel);
  wifi_promiscuous_enable(false);
  wifi_set_promiscuous_rx_cb(receiveCallback);
  wifi_promiscuous_enable(true);
  digitalWrite(LED_BUILTIN, HIGH);
}

void setupWifi() {
  wifi_promiscuous_enable(false);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, millis() / 100 & 1);
    yield();
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void onButtonPress(uint8_t buttonIndex) { // callback for received button press
  setupWifi();
  onWifiConnected();
  setupPromiscous();
}

int targetLookUp(uint8_t* mac) { // find the index of a stored button mac address
  for (int i = 0; i < (sizeof(targets) / sizeof(targets[0])); i++) { // iterate through all stored macs
    if (memcmp(targets[i], mac, 6) == 0) return i; // hit!
  }
  return -1;
}

void setup() {
  memset(seqNumbers, ~0, sizeof(seqNumbers)); // set all lastSequence-entries to MAX
  pinMode(LED_BUILTIN, OUTPUT);

  setupPromiscous();
}

void loop() {
  if (pressedButton > -1) { // check if buttonPressed-flag is set
    onButtonPress(pressedButton);
    pressedButton = -1; // unset flag
  }
}

void receiveCallback(uint8_t *buf, uint16_t len)
{
  if (pressedButton == -1 && len == 128) {
    uint8_t* const macAddress = buf + 22;
    int buttonIndex = targetLookUp(macAddress);

    if (buttonIndex < 0) return; // mac not found

    /*int seqHigh = buf[35];
    int seqLow  = buf[34];
    int sequenceNumber = (seqHigh << 4) | (seqLow >> 4);
    int lastSeq = seqNumbers[buttonIndex];
    seqNumbers[buttonIndex] = sequenceNumber;*/
    //if (sequenceNumber < lastSeq) { // check if sequence numbering started new
      pressedButton = buttonIndex;
    //}
  }
}
