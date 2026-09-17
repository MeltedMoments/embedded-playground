# NoisyProject working notes

## Idea

Measure ambient noise in the alley and provide a simple visual indication
of sustained noise level.

Initial goal is not calibrated/legal dBA measurement. The first prototype
should detect relative sound level and show something like:

quiet -> acceptable -> getting loud -> too loud

No audio recording or storage.

## Why

Small experimental project for:
- learning ESP32 / embedded programming
- sensors and physical-world inputs
- LED/display outputs
- eventual preparation for watermill monitoring/automation

## Hardware

### ESP32-S3
- ESP32-S3 N16R8
- 16 MiB flash
- 8 MiB PSRAM
- PlatformIO / Arduino framework
- onboard NeoPixel: GPIO 48
- BOOT button: GPIO 0

### Microphone
- 2 x INMP441
- I2S
- not yet tested

### Display
- 2 x 8-pixel WS2812B sticks
- not yet tested

### Other bits
- AHT20 temperature/humidity sensor
- DHT22 module
- Trinket M0
- Grove LED bar

## 20260910

### ESP32 start up
- PlatformIO build/upload works
- USB serial connection is currently slow/unreliable
- temporary long startup delay needed to see setup() output
- TODO investigate USB/serial configuration

### Memory
- 16 MiB flash detected
- ~388 KiB internal heap
- ~8 MiB PSRAM
- tested 1 MiB PSRAM allocation/free successfully
- experiments/memory_test.cpp

### GPIO
- BOOT button read successfully using INPUT_PULLUP
- pressed = LOW

### Onboard NeoPixel
- Adafruit NeoPixel library
- GPIO 48
- colour changing works
- button cycles through colours
- experiments/onboard_pixel_test.cpp

### Timing
- experimented with millis()
- non-blocking periodic work instead of delay(): heartbeat
- looked at button edge detection and debounce

### I2C / AHT20
- SDA GPIO 8
- SCL GPIO 9
- I2C scanner detects device at 0x38
- experiments/i2c_scan.cpp

### next time
- Read AHT20 temperature/humidity.
- Explore what the AHT20 library is doing over I2C.
- Investigate ESP32 USB/serial startup behaviour.
- Try Wokwi simulation.
- When soldering supplies arrive:
   - solder 8-pixel NeoPixel stick
   - solder INMP441
   - start actual sound-level experiments.

## 20260911
- okay stop fussing about the lack of soldering equipment, plenty to do in the meantime.
- these workingnotes started, first part by chat, but we'll use the same format that I used in migtool from here on in. That seems to work well
- today: read the aht20 and install wokwi

### notes
- aht20 gave me hell, but it was because it wasn't getting power correctly. using F-F connectors finally got it working
- but I learnt a bit more about my multimeter.
- big clue: Finally the little green light on the aht20 lit up

    -  Is it powered?
    -  Is ground common?
    -  Are the signal lines electrically sensible?
    -  Can the bus see the device?
    -  Does the library/protocol work?
    -  Is the application logic correct?

- messing around with wokwi, seems like it will be pretty useful
    - needed to add these flags to .ini to make it work (but now hangs on real board)
```
build_flags = 
    -DBOARD_HAS_PSRAM
    -DARDUINO_USB_MODE=1
    -DARDUINO_USB_CDC_ON_BOOT=1    
```

### eod
- managed to read the AHT20 from the ESP when directly connected. Prob need new breadboards?
- got a rudimentary pixel display for some fake data in wokwi
- needs improving
    - show-pixels() is off-by one. need to think it through properly, not all 8 lights shown when level 8. 
    - better algorithm to figure out the level-divisor (needs max, min and divisble into 8)

### next time
- prob starting to need some project organisation
- start a git repo
- fix the current version of NoisyDisplay
- Clean up show_pixels() so it always clears the strip first and then lights exactly level pixels.
- Replace the big if/else threshold ladder with a calculation from 0–100 to 1–8.
- Then make the fake input more realistic — values bouncing around like 48, 52, 49, 55, 53... — and watch the LEDs flicker annoyingly.

## 20260912
- fairly irritated attempt at making headpins with silver-filled wire. Hopefully cleaning the oxycons will solve my problems
- cleaned up neopixel_test 

### eod
- played around with smoothing the data
- simple moving average, 4 readings neopixel-sma-test.cpp
- current one using exponential moving average with Rise and Fall factors 

### next time
- prob starting to need some project organisation
- start a git repo
- figure out breadboard: how to measure
- shopping list
    - krimpkous
    - breadboard?

## 20260913
- a few hours spare
- implement hysteresis 
- project org
- then grove 16x2 display

## 20260914
- yesterday got the aht20 and 16x2 display working together on the same I2C bus
- needed to remember how to power and use a breadboard!
- esp doesn't need to live on a BB, but use a BB as a bridge
- grove's is connected by duponts, connections are a bit wobbly

- started using arduino String, and also std::min and max()
- heartbeat now displays on the lcd :)

### notes
- oled 
```
OLED signal   ESP32-S3       Purpose        (wire)
---------------------------------------------
VCC           3V3            power          red
GND           GND            ground         black
DIN           GPIO 11        SPI MOSI/data  orange
CLK           GPIO 12        SPI clock      green
CS            GPIO 10        chip select    grey 
DC            GPIO 9         data/command   blue
RST           GPIO 8         reset          purple
```

### eod
- okay nice, oled hooked in as well, now updates temps on oled and lcd
- tiny 8x8 bitmap to actually display a heart as the heartbeat on the oled :)

### next time
- Add history to eg temperature  and display small graph 

## 20260915
- stuff from reichelt should arrive today, and Herman, and BAM too
- anyway, try to implement a "rolling history graph".

### eod
- decided to split the code instead, much saner
- soldering stuff arrived so will hopefully wire stuff up next time

### next time
- move experiments to src/
- try runnable neopixel wokwi experiment, sep from main working code
- get neopixel and microphone working 
- 

## 20260916
- finally managed to change vscode project colours so I know where I am (though colours still need improvement)
- right first thing is to reorg stuff so that I can run the neopixel wokwi demo
- then solder a neopixel
- and try it for real :)

### notes
- moved heartbeat code to heartbeat.cpp, that completes the refactoring
    - hmm, one more improvement: return bool from setup-<display>, and only try to display if we've got one, but maybe next time. 
- moved things around in platformio.ini
    - main project is 'esp32'
    - can run (wokwi) sub-projects neopixel-test (strip_fill_test) and neopixel-wokwi (hysteresis-test)
    ```
    # in project root
    ./wokwi-build neopixel-wokwi  (then open wokwi simulator)
    ```

    - program in the esp32 env is set as default in PIO, so can build and upload as normal
- Start soldering! 

- microphone
```
INMP441       ESP32-S3   (Colour)
VDD    ─────> 3V3         red      
GND    ─────> GND         black       
SCK    ─────> GPIO 17     blue    I²S bit clock
WS     ─────> GPIO 18     yellow  I²S word-select / LR clock
SD     ─────> GPIO 16     green   microphone data -> ESP
L/R    ─────> GND
```

### eod
- neopixel soldered and tested
- microphone soldered and tested
- now need to hook the two together

## 20260917
- Neat new magnetic third hand :)
- okay first thing is to sort out what the min,max numbers mean
    - 16ms samples of the peak/troughs, deltas are what we're interested in

- Apply Root Mean Squared: calculated as the square root of the mean of the squares of a set of values
    - Squaring places a heavier weight on larger spikes or errors, making RMS very sensitive to extreme values


# ===> I AM HERE MARKER HERE AM I <===
# ===> I AM HERE MARKER HERE AM I <===
# ===> I AM HERE MARKER HERE AM I <===


## Things learned / reminders

- setup() runs once after boot/reset; loop() then repeats.
- Serial monitor may miss early output even though setup() ran.
- millis() is useful for scheduling without blocking.
- I2C devices share SDA/SCL and have addresses.
- NeoPixel setPixelColor() updates the buffer; show() sends it.
- Store experimental programs outside src/ if they contain their own
  setup()/loop(), because PlatformIO compiles all .cpp files in src/.

