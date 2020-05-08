#include "lipo.h"

#include "lpc40xx.h"

#include "adc.h"

/**
 *
 */
static void lipo__enable_burst_mode();
static void lipo__pin_configure();
static uint16_t adc__get_channel_reading_with_burst_mode(adc_channel_e channel);

// 4095 -> 3.3V
// max_value -> 2.143
static const float MAX_LIPO_ADC = (2.1 * 4095) / 3.3;
static const float MAX_LIPO_BATTERY = 7.54;

void lipo__init() {
  adc__initialize();
  lipo__enable_burst_mode();
  lipo__pin_configure();
}

float lipo__get_voltage() {
  // TODO, Change the channel later
  uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_5);

  // max_value -> 7.5
  // adc_value -> x
  float rval = (adc_value * MAX_LIPO_BATTERY) / MAX_LIPO_ADC;
  return rval;
}

/**
 *
 */
static void lipo__enable_burst_mode() {
  // Set the Burst Mode and Channel here
  LPC_ADC->CR |= (1 << 16);

  // Make sure Start bit is 0
  LPC_ADC->CR &= ~(0xFF << 24);
}

static void lipo__pin_configure() {
  // Set alternate function register here
  LPC_IOCON->P1_31 &= 0xF;
  LPC_IOCON->P1_31 |= 0x3;

  // Set to Analog function
  LPC_IOCON->P1_31 &= ~(1 << 7);

  // Set CR Value here
  LPC_ADC->CR |= (1 << 5);
}

static uint16_t adc__get_channel_reading_with_burst_mode(adc_channel_e channel) {
  uint16_t ret = (LPC_ADC->DR[channel] >> 4) & 0xFFF;
  return ret;
}
