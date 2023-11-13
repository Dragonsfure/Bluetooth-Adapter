#include <Arduino.h>
#include "BluetoothA2DPSink.h"

/// @brief Field for the Bluetooth Stack.
BluetoothA2DPSink a2dp_sink;

//Area for Define Constants
#define SKIPPIN 0
#define PREVIOUSPIN 2
#define PAUSEPIN 4

/// @brief Setup for the Different IO-Ports and Communication.
void setup()
{   
    // Config for Adafuit / UDA1334A Breakout Board
    static const i2s_config_t i2s_config = {
         .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
         .sample_rate = 41000,
         .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
         .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
         .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
         .intr_alloc_flags = 0, // default interrupt priority
         .dma_buf_count = 8,
         .dma_buf_len = 64,
         .use_apll = false,
         .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
    };
    a2dp_sink.set_i2s_config(i2s_config);
    //Serial.begin(115200);
    a2dp_sink.start("BT-Sound-Adapter");

    // Pin Setup
    pinMode(SKIPPIN, INPUT);
    pinMode(PREVIOUSPIN, INPUT);
}

/// @brief Main-Loop to do the Magic.
void loop()
{
    if (digitalRead(SKIPPIN) == HIGH)
    {
        a2dp_sink.next();
    }
    if (digitalRead(PREVIOUSPIN) == HIGH){
        a2dp_sink.previous();
    }
    if (digitalRead(PAUSEPIN)== HIGH)
    {
        a2dp_sink.pause();
    }    
}