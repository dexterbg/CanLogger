# CanLogger

This is a very simple CAN and logic logging tool.

It's meant to analyse correlations between logic signals and CAN messages (i.e. for protocol reverse engineering).

It's not optimized for high time resolution & accuracy. It's currently sufficient to analyze correlations between
CAN messages and logic signals within a resolution of ~ 1-2 ms.

Multiple input ports can be monitored at once.

Output format is three columns separated by space:

  - Time (ms)
  - ID ("I…" for an input port, else CAN ID)
  - Data (0/1 for an input port, else CAN data hexadecimal)

