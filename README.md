The **NeoICSerial** class is intended as a drop-in replacement for Paul Stoffregen's class [AltSoftSerial](https://github.com/PaulStoffregen/AltSoftSerial).  It adds the capability to register a function to be called when a new character is received.

This class can only use one predefined Input Capture pin.  Each MCU and board has a pre-determined pin:

<table><tr><td> <b>Board</b> </td><td align=center> <b>Transmit</b> </td><td align=center> <b>Receive</b> </td><td align=center> <b>PWM Unusable</b></td></tr>
<tr><td> Teensy 3.0 & 3.1 </td><td align=center> 21 </td><td align=center> 20 </td><td align=center> 22</td></tr>
<tr><td> Teensy 2.0 </td><td align=center> 9  </td><td align=center> 10 </td><td align=center> (none)</td></tr>
<tr><td> Teensy++ 2.0 </td><td align=center> 25 </td><td align=center> 4 </td><td align=center> 26, 27</td></tr>
<tr><td> Arduino Uno </td><td align=center> 9  </td><td align=center> 8 </td><td align=center> 10</td></tr>
<tr><td> Arduino Leonardo </td><td align=center> 5 </td><td align=center> 13 </td><td align=center> (none)</td></tr>
<tr><td> Arduino Mega </td><td align=center> 46 </td><td align=center> 48 </td><td align=center> 44, 45</td></tr>
<tr><td> Wiring-S </td><td align=center> 5 </td><td align=center> 6 </td><td align=center> 4</td></tr>
<tr><td> Sanguino </td><td align=center> 13 </td><td align=center> 14 </td><td align=center> 12</td></tr>
</table>

If the Input Capture pin is not available, you may want to consider [NeoHWSerial](https://github.com/SlashDevin/NeoHWSerial) or [NeoSWSerial](https://github.com/SlashDevin/NeoSWSerial).

To handle received characters with your procedure, you must register it with the `NeoICSerial` class or your instance:

```
  NeoICSerial serial_port;
  NeoICSerial::attachInterrupt( handleRxChar );
     //  OR
  serial_port.attachInterrupt( handleRxChar );
```

Remember that the registered procedure is called from an interrupt context, and it should return as quickly as possible.  Taking too much time in the procedure will cause many unpredictable behaviors, including loss of received data.  See the similar warnings for the built-in [`attachInterrupt`](https://www.arduino.cc/en/Reference/AttachInterrupt) for digital pins.

The registered procedure will be called from the ISR whenever a character is received.  The received character **will not** be stored in the `rx_buffer`, and it **will not** be returned from `read()`.  Any characters that were received and buffered before `attachInterrupt` was called remain in `rx_buffer`, and could be retrieved by calling `read()`.

If `attachInterrupt` is never called, or it is passed a `NULL` procedure, the normal buffering occurs, and all received characters must be obtained by calling `read()`.

The original `AltSoftSerial` files were modified to include two new methods, `attachInterrupt` and `detachInterrupt`:

```
    typedef void (* isr_t)( uint8_t );
    static void attachInterrupt( isr_t fn );
    static void detachInterrupt() { attachInterrupt( (isr_t) NULL ); };
```

The following compatibilty constructor was removed:
```
	  // for drop-in compatibility with NewSoftSerial, rxPin & txPin ignored
	  AltSoftSerial(uint8_t rxPin, uint8_t txPin, bool inverse = false) { }
```
