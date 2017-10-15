/**
 * ==========================================================================
 * CanLogger -- log CAN messages and logic input changes
 * ==========================================================================
 * 
 * Note: this is not optimized for high time resolution & accuracy.
 *  It's sufficient to analyze correlations between CAN messages and
 *  logic signals within a resolution of ~ 1-2 ms.
 * 
 * Author: Michael Balzer <dexter@dexters-web.de>
 * 
 * Libraries used:
 *  - MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib
 * 
 * License:
 *  This is free software under GNU Lesser General Public License (LGPL)
 *  https://www.gnu.org/licenses/lgpl.html
 *  
 */
#define CANLOGGER_VERSION "V1.0 (2017-10-15)"

#include <mcp_can.h>
#include <mcp_can_dfs.h>

#include "CanLogger_config.h"
#define INPUT_CNT (sizeof(logic_input) / sizeof(int))


// CAN interface:
MCP_CAN CAN(CANLOGGER_CAN_CS_PIN);

// CAN RX buffer:
unsigned long rxId;
byte rxLen;
byte rxBuf[8];

// Logic state:
byte logic_state[INPUT_CNT];


volatile bool canMsgReceived = false;

#if CANLOGGER_CAN_IRQ_PIN > 0
void canISR() {
  canMsgReceived = true;
}
#endif


void setup() {

  // init serial port:
  
  Serial.begin(1000000);
  while (!Serial);
  Serial.println(F("CanLogger " CANLOGGER_VERSION));

  // init CAN:
  
  #if CANLOGGER_CAN_IRQ_PIN > 0
    pinMode(CANLOGGER_CAN_IRQ_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(CANLOGGER_CAN_IRQ_PIN), canISR, FALLING);
  #endif
  
  while (CAN.begin(MCP_ANY, CANLOGGER_CAN_BUS_SPEED, CANLOGGER_CAN_MCP_FREQ) != CAN_OK) {
    Serial.println(F("setup: waiting for CAN connection..."));
    delay(3000);
  }
  
  CAN.setMode(MCP_NORMAL);

  // init logic input state:
  
  for (int i=0; i < INPUT_CNT; i++) {
    pinMode(logic_input[i], INPUT);
    logic_state[i] = digitalRead(logic_input[i]);
  }

  // print CSV header:
  
  Serial.println();
  Serial.println(F("Time ID Data"));
}


void loop() {

  unsigned long now = millis();
  byte input;

  // check and output logic inputs:
  
  for (byte i=0; i < INPUT_CNT; i++) {
    input = digitalRead(logic_input[i]);
    if (input != logic_state[i]) {
      Serial.print(now);
      Serial.print(' '); Serial.print('I'); Serial.print(logic_input[i]);
      Serial.print(' '); Serial.println(input);
      logic_state[i] = input;
    }
  }

  // check and output CAN data:
  
  #if CANLOGGER_CAN_IRQ_PIN == 0
    canMsgReceived = (CAN.checkReceive() == CAN_MSGAVAIL);
  #endif
  
  if (canMsgReceived) {
    canMsgReceived = false;
    while (CAN.readMsgBuf(&rxId, &rxLen, rxBuf) == CAN_OK) {
      Serial.print(now);
      Serial.print(' '); Serial.print(rxId, HEX);
      Serial.print(' ');
      for (byte i = 0; i<rxLen; i++) {
        if (rxBuf[i] < 16)
          Serial.print('0');
        Serial.print(rxBuf[i], HEX);
      }
      Serial.println();
    }
  }
  
}

