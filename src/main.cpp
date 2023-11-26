#include <Arduino.h>
#include "BluetoothA2DPSink.h"
#include <ezButton.h>

/// @brief Field for the Bluetooth Stack.
BluetoothA2DPSink a2dp_sink;

// Area for Define Constants
#define SKIPPIN 6
#define PREVIOUSPIN 9
#define DEADTIMEBTN 100

ezButton skipPin(SKIPPIN);         // create ezButton object that attach to pin SKIPPIN;
ezButton previousPin(PREVIOUSPIN); // create ezButton object that attach to pin PREVIOUSPIN;

/// @brief Setup for the Different IO-Ports and Communication.
void setup()
{
    // Config for Adafuit / UDA1334A Breakout Board
    static const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 41000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
    };
    a2dp_sink.set_i2s_config(i2s_config);

    // Sets the Name of the Bluetooth-Adapter
    a2dp_sink.start("My-Music-Player");

    // Sets the Buttons debounce Time.
    skipPin.setDebounceTime(DEADTIMEBTN);
    previousPin.setDebounceTime(DEADTIMEBTN);
}

/*
    DAC 	ESP32
    VIN 	5V
    GND 	GND
    WSEL 	WS (GPIO25)
    DIN 	OUT (GPIO22)
    BCLK 	BCK (GPIO26)
*/

/// @brief Main-Loop to do the Magic.
void loop()
{
    //Changes the Buttons Values and checks for a press.
    ButtonLoops();

    //Handles the Actions of the Buttons.
    ButtonHandler();
}

/// @brief Checks all the Buttons, if they got pressed and so.
void ButtonLoops()
{
    // MUST call these loop() functions for every button first
    skipPin.loop();
    previousPin.loop();
}

/// @brief Checks, if a Buttons are pressed and starts the corresponding Action.
void ButtonHandler()
{
    // Checks if the Pause-Button got pressed.
    if (skipPin.isReleased())
        a2dp_sink.next();

    // Checks if the Pause-Button got pressed.
    if (previousPin.isReleased())
        a2dp_sink.previous();
}