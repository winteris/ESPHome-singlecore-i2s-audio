#pragma once

#include "esphome/components/media_player/media_player.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/helpers.h"

#include <Arduino.h>
#include "AudioGeneratorMP3.h"
#include "AudioGeneratorWAV.h"
#include "AudioFileSourceBuffer.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceHTTPStream.h"
#include "AudioOutputI2S.h"

#include "esphome/core/log.h"

namespace esphome {
namespace i2s_audio {

class I2SAudioMediaPlayer : public Component, public media_player::MediaPlayer {
 public:
  void setup() override;
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

  void loop() override;

  void dump_config() override;

  void set_dout_pin(uint8_t pin) { this->dout_pin_ = pin; }
  void set_bclk_pin(uint8_t pin) { this->bclk_pin_ = pin; }
  void set_lrclk_pin(uint8_t pin) { this->lrclk_pin_ = pin; }
  void set_mclk_pin(uint8_t pin) { this->mclk_pin_ = pin; }
  void set_mute_pin(GPIOPin *mute_pin) { this->mute_pin_ = mute_pin; }
#if SOC_I2S_SUPPORTS_DAC
  void set_internal_dac_mode(i2s_dac_mode_t mode) { this->internal_dac_mode_ = mode; }
#endif
  void set_external_dac_channels(uint8_t channels) { this->external_dac_channels_ = channels; }

  media_player::MediaPlayerTraits get_traits() override;

  bool is_muted() const override { return this->muted_; }

 protected:
  void control(const media_player::MediaPlayerCall &call) override;

  void mute_();
  void unmute_();
  void set_volume_(float volume, bool publish = true);
  void stop_();
  
  char* playing_status = new char[10];
  float volume_level   = 0.8;
  const int preallocateBufferSize = 2048;
  void *preallocateBuffer = NULL;
  
  void broadcastStatus(const char* msg);
  void updateLEDBrightness(int brightness_percentage);
  void stopPlaying();
  void playaudio(const char* source);

  AudioOutputI2S *out = NULL;
  AudioGeneratorMP3 *mp3 = NULL;
  AudioGeneratorWAV *wav = NULL;
  AudioFileSourceHTTPStream *file_http = NULL;
  AudioFileSourcePROGMEM *file_progmem = NULL;
  AudioFileSourceBuffer *buff = NULL;

  uint8_t dout_pin_;
  // uint8_t din_pin_;
  uint8_t bclk_pin_;
  uint8_t lrclk_pin_;
  uint8_t mclk_pin_;

  GPIOPin *mute_pin_{nullptr};
  bool muted_{false};
  float unmuted_volume_{0};


#if SOC_I2S_SUPPORTS_DAC
  i2s_dac_mode_t internal_dac_mode_{I2S_DAC_CHANNEL_DISABLE};
#endif
  uint8_t external_dac_channels_;

  HighFrequencyLoopRequester high_freq_;
};

}  // namespace i2s_audio
}  // namespace esphome

