/**
 * ==========================================================================
 * CanLogger configuration
 * ==========================================================================
 */
#ifndef _CanLogger_config_h
#define _CanLogger_config_h

// Set your CAN bus speed here:
#define CANLOGGER_CAN_BUS_SPEED       CAN_500KBPS

// Set your CAN MCP clock frequency here:
#define CANLOGGER_CAN_MCP_FREQ        MCP_16MHZ

// Set your CAN CS pin number here:
#define CANLOGGER_CAN_CS_PIN          10

// Set your CAN IRQ pin here (0 = no IRQ, use polling):
#define CANLOGGER_CAN_IRQ_PIN         2

// Set your logic inputs here:
const int logic_input[] = {
  3, 4, 5, 6, 7, A0, A1, A2, A3, A4, A5
};

#endif // _CanLogger_config_h

