#pragma once
#include <U8g2lib.h>
#include <MadDisplay/buffer.hpp>
#include <MadDisplay/panels.hpp>

namespace mad {
template<int16_t Width=128,int16_t Height=64>
class U8g2Adapter final : public BareBuffer<Width,Height> {
 public:
  static_assert(Width>0 && Height>0 && Width%8==0 && Height%8==0,"OLED dimensions must fit native tiles");
  U8g2Adapter(U8G2& driver,const char* profile) : driver_(driver),spec_(panelSpec(profile)) {}
  U8g2Adapter(U8G2& driver,PanelSpec spec) : driver_(driver),spec_(spec) {}
  bool begin(uint32_t frequency=0) {
    if (!spec_.mono() || spec_.width!=Width || spec_.height!=Height) return false;
    if (!frequency) frequency=spec_.maxBusHz();
    if (frequency>spec_.maxBusHz() || !frequency) return false;
    driver_.setBusClock(frequency); if (!driver_.begin()) return false;
    // Only full-frame (_F_) constructors and U8G2_R0 use this native packing.
    ready_=driver_.getDisplayWidth()==Width && driver_.getDisplayHeight()==Height &&
        driver_.getBufferTileWidth()*8>=Width && driver_.getBufferTileHeight()*8>=Height &&
        driver_.getU8g2()->cb==U8G2_R0;
    return ready_;
  }
  Color emittedColor(int16_t x,int16_t y) const {
    if (!this->isOn(x,y)) return Color::Black;
    return spec_.kind==PanelKind::Zoned && y<16 ? Color::Yellow : spec_.ink;
  }
  void present() override {
    if (!ready_) return;
    driver_.clearBuffer();
    const size_t stride=driver_.getBufferTileWidth()*8;
    uint8_t* native=driver_.getBufferPtr();
    for (int16_t y=0;y<Height;++y) for (int16_t x=0;x<Width;++x)
      if (this->isOn(x,y)) native[size_t(y/8)*stride+x] |= 1u<<(y%8);
    driver_.sendBuffer();
  }
  U8G2& driver() { return driver_; }
 private:
  U8G2& driver_; const PanelSpec spec_; bool ready_=false;
};
// Driver lifetime belongs to the caller. Addresses in U8g2 use the 8-bit convention (0x3c << 1).
inline U8G2* createOLEDDriver(const char* profile,uint8_t cs=U8X8_PIN_NONE,uint8_t dc=U8X8_PIN_NONE,uint8_t reset=U8X8_PIN_NONE) {
  const auto& p=panelSpec(profile); if (!p.mono()) return nullptr;
  const bool i2c=p.bus==PanelBus::I2C;
  if (!i2c && (cs==U8X8_PIN_NONE || dc==U8X8_PIN_NONE)) return nullptr;
  if (!strcmp(p.controller,"SSD1306") && p.width==128 && p.height==64) {
    if (i2c) return new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  if (!strcmp(p.controller,"SSD1306") && p.width==128 && p.height==32) {
    if (i2c) return new U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SSD1306_128X32_UNIVISION_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  if (!strcmp(p.controller,"SSD1306") && p.width==64 && p.height==48) {
    if (i2c) return new U8G2_SSD1306_64X48_ER_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SSD1306_64X48_ER_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  if (!strcmp(p.controller,"SSD1306") && p.width==96 && p.height==16) {
    if (i2c) return new U8G2_SSD1306_96X16_ER_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SSD1306_96X16_ER_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  if (!strcmp(p.controller,"SH1106") && p.width==128 && p.height==64) {
    if (i2c) return new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  if (!strcmp(p.controller,"SSD1309") && p.width==128 && p.height==64) {
    if (i2c) return new U8G2_SSD1309_128X64_NONAME2_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  if (!strcmp(p.controller,"SSD1305") && p.width==128 && p.height==32) {
    if (i2c) return new U8G2_SSD1305_128X32_NONAME_F_HW_I2C(U8G2_R0,reset);
    return new U8G2_SSD1305_128X32_NONAME_F_4W_HW_SPI(U8G2_R0,cs,dc,reset);
  }
  return nullptr;
}
} // namespace mad
