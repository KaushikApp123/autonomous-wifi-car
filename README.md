# Autonomous WiFi-Controlled Car

MSP430FR2355 car that follows a line course on its own and can also be driven over WiFi. Written in bare-metal C (no OS). An ESP8266 bridges TCP/IP commands from a laptop to the MSP430 over UART.

Built for ECE 306 (Project 10) at NC State.

![Car top view](docs/car_top.jpg)
![Car held showing chassis](docs/car_side.jpg)

## How it works

- **Finite state machine.** `main.c` runs the event loop. The car moves between states (forward, reverse, left, right, arcs, line-detect, line-follow, stop) based on sensor readings and incoming commands.
- **Line following.** Two IR reflectance sensors are sampled by the ADC on a Timer_A interrupt. A bang-bang controller adjusts the motor PWM depending on which sensor sees the black line, which keeps the car on course.
- **WiFi control.** The ESP8266 receives commands over TCP/IP and passes them to the MSP430 over UART at 115200 baud, using interrupt-driven ring buffers. Every command packet has to start with a set start character and carry a valid PIN before it's executed. Anything else is rejected with "BAD START" or "PIN FAIL" (`Parse_Drive_Command()` in `movemnt.c`).
- **Motor supply voltage (DAC).** The MSP430's DAC sets the control input of the car's motor-supply regulator. On startup it steps the voltage up gradually on a timer interrupt, then holds it at a set level (`dac.h`).

## Debugging story: unstable turns

The car's turns were inconsistent: the same command turned it a different amount from run to run. The code looked correct, so I put an oscilloscope on the motor supply and saw the battery voltage dropping under load. The motors were getting less power as the battery drained, so the same PWM gave a different turn.

The fix was to drive the motors from a regulated voltage set by the DAC instead of straight off the battery, so the motor supply stays steady as the battery drops. The tested set points are kept as alternates in `dac.h`.

## Hardware

- MSP430FR2355 LaunchPad
- ESP8266-01 WiFi module
- 2x IR reflectance sensors
- DC motors with H-bridge driver
- 16x2 LCD (SPI)

## Wiring

```
ESP8266 TX -> MSP430 UART RX (A1)
ESP8266 RX -> MSP430 UART TX (A1)
IR sensors -> MSP430 ADC (A2, A3)
Motors     -> H-bridge -> GPIO
LCD        -> SPI (UCB0)
```

## Build

Open in Code Composer Studio, select the MSP430FR2355 target, then build and flash over USB.

Default baud is 115200. IR thresholds are in `macros.h`. Tune the BLACK and WHITE values for your floor.

## File overview

| File | What it does |
|---|---|
| `main.c` | Entry point, FSM event loop, command execution |
| `movemnt.c` | Navigation states, motor control, IR logic |
| `serial.c` | UART drivers, ESP8266 WiFi bridge, ring buffers |
| `adc.c` | IR sensor ADC sampling |
| `dac.c` / `dac.h` | DAC setup and motor-supply voltage set points |
| `timers.c` | Timer config, PWM, DAC ramp on timer overflow |
| `ports.c` | GPIO init |
| `macros.h` | Tunable constants |

## Performance

- Command-to-actuation latency over WiFi: under 100 ms
- IR sampling: interrupt-driven via Timer_A

## Known limitations

- The ESP8266 sometimes needs a manual reset after power-on if it doesn't join the network. `Init_IOT()` in `serial.c` handles most cases.
- At high speed, the bang-bang controller wobbles slightly on tight turns. A proportional controller would smooth this out. For now, lowering `SPEED_FAST` in `macros.h` helps.
