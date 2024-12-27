# ESP8266-expansion-board
/*
Code able to control twenty six output pins and acquire data from sixteen inputs using only three esp8266 pins.

This code was designed to a expansion board, designed by me, based in a CD74HC4067 16 channels mux and three 74HC595 shift registers.

It will automatically acquire data from the 16 mux channels, filter it, and storage. Contains also an moving average function so the working value of each variable gets more stable. The board itself features, also, a low-pass filter. Them both should be enough to stabilize the signal satisfactorily.

Using only three pins from the microcontroler it is possible to control the whole 24 shift register outputs, and, using the first four pins from the first shift register, we become able to control the input mux. Then, remains 20 shift register bits to control anything intended, besides four other output pins directly from the ESP, capable of PWM outputs. The d1 and d2 outputs are also free, totalizing 26 outputs possibilities. These last two are separeted from the rest because they are clock and data pins, respectively.

This code only acquires the data. Further functions must be implemented in order to control any kind of actual system.

It also features an OTA upload mechanism. Create your own Host name and password.

The led built in blink function was only inteded to reassure the correct functioning of the ESP since it will be placed in the field in a 24/7 operating regime.

Conections already estabilished by the board:

D0 (GPIO16) - SHIFT REGISTER 12 (LATCH)
D3 (GPIO5)  - SHIFT REGISTER 14 (DATA)
D4 (GPIO4)  - SHIFT REGISTER 11 (CLOCK)
SHIFT REGISTER 1 QO - S0 CD74HC4067
SHIFT REGISTER 1 Q1 - S1 CD74HC4067
SHIFT REGISTER 1 Q2 - S2 CD74HC4067
SHIFT REGISTER 1 Q3 - S3 CD74HC4067

The variables, functions, tab names and comments are in Brasilian. idc.
*/
