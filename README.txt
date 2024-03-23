0. In:
    1. In GND 220V - Left pin, on the side of the board
    2. In Phase 220V - middle
    3. Out Phase 220V - Right pin, in the middle of the board
1. J4/1 1 -> J4 1 -> P0.6
2. Second contact pins:
  1. GND (right pin, in the middle of the board) - preasure sensor black wire
  2. +5v (middle pin) - preasure sensor red wire
  3. Preasure Signal (left pin, in the side of the board) - preasure sensor yellow wire
3.1 J3/1 1 - +5V
3.2 J3/1 2 and 3 - UART (disconnect to flash ESP via USB)
4. J4/1 2 - GND
5. J4/1 1 - Preasure Signal
6. J4/1 3 - ESP reset (bistable relay ON Signal, pin 2.0). Added manual switch as it impacts ESP connected to USB

Voltage transformation:
    258.8 / 1348 = 0.192
    518.1 / 2699 = 0.192