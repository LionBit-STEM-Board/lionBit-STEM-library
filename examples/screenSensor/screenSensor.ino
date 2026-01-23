#include "driver/i2s.h"
#include "lionbitsupport.h"   // already declares: Adafruit_ST7735 tft

#define I2S_NUM         (I2S_NUM_0)
#define I2S_BCK_PIN     17   // D10
#define I2S_WS_PIN      16   // D9
#define I2S_SD_PIN      18   // D11

int x = 0; // current column for waveform

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("LionBit INMP441 Audio Visualizer with VU Meter");

  // --- LCD Setup using Adafruit ST7735 ---
  tft.initR(INITR_BLACKTAB);   // correct init for LionBit LCD
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);

  // --- I2S Microphone Setup ---
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCK_PIN,
    .ws_io_num = I2S_WS_PIN,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD_PIN
  };

  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM);
}

void loop() {
  int32_t samples[64];
  size_t bytes_read;

  // --- Get 64 samples ---
  i2s_read(I2S_NUM, (void*)samples, sizeof(samples), &bytes_read, portMAX_DELAY);

  if (bytes_read > 0) {
    // Clear vertical strip for new waveform column
    tft.drawFastVLine(x, 0, 128, ST77XX_BLACK);

    int peak = 0;

    // --- Draw waveform ---
    for (int i = 0; i < 64; i++) {
      int32_t s = samples[i] >> 14; // reduce from 32-bit to ~11-bit
      s = constrain(s, -2048, 2048);
      int y = map(s, -2048, 2048, 0, 127);
      tft.drawPixel(x, y, ST77XX_GREEN);

      // Track peak level
      int abs_s = abs(s);
      if (abs_s > peak) peak = abs_s;
    }

    // --- VU meter ---
    int barHeight = map(peak, 0, 2048, 0, 127);
    if (barHeight > 127) barHeight = 127;

    uint16_t barColor;
    if (barHeight < 40) {
      barColor = ST77XX_GREEN;
    } else if (barHeight < 80) {
      barColor = ST77XX_YELLOW;
    } else {
      barColor = ST77XX_RED;
    }

    // Clear previous VU area (last 6 pixels of screen)
    tft.fillRect(122, 0, 6, 128, ST77XX_BLACK);

    // Draw VU bar (bottom aligned)
    tft.fillRect(122, 128 - barHeight, 6, barHeight, barColor);

    // --- Scroll waveform ---
    x++;
    if (x >= 120) x = 0; // Leave last 8px for VU meter
  }
}
