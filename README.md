# MadU8g2

Packed monochrome MadDisplay adapter for U8g2 full-buffer OLED drivers:
SSD1306 (128x64, 128x32, 64x48, legacy 96x16), SH1106, SSD1309 and SSD1305.
White/blue/yellow-blue describe physical emitters, not programmable RGB colors.

Use `U8g2Adapter<Width,Height>(driver,profileId)` with a matching `_F_` constructor
and **U8G2_R0**. The adapter rejects paged buffers and other rotations. The original
MadSSD1306 library remains available for optimized differential I2C transfers on
128-wide SSD1306 panels. MadU8g2 sends a full frame; the Studio models that full
transfer for SPI OLEDs, SH1106, SSD1309, SSD1305 and the 64x48 OLED.

```cpp
U8G2_SH1106_128X64_NONAME_F_HW_I2C driver(U8G2_R0,U8X8_PIN_NONE);
mad::U8g2Adapter<> panel(driver,mad::monoPanelSpec(128,64,mad::PanelBus::I2C,mad::Color::Blue));
mad::Display<decltype(panel)> display(panel);
```

`createOLEDDriver(profileId,cs,dc,reset)` provides the corresponding full-buffer
U8g2 driver for every catalog mono profile. SPI needs CS and DC; I2C uses board
pins. Returned driver lifetime belongs to the caller. For another I2C address use
`driver.setI2CAddress(address << 1)` before begin. `begin(frequency=0)` sets the
profile's conservative bus clock. Two packed buffers are used: canonical MadDisplay
bits plus U8g2 native pages. `emittedColor(x,y)` reports physical panel emission.

On AVR, use the direct `monoPanelSpec` overload shown above to avoid linking the
whole searchable catalog into scarce data RAM. String IDs remain convenient on
larger boards.

Build `examples/mono_ui/mono_ui.ino` with `pio run` (Mega 2560). The standard
SSD1306 128x64 demo needs more RAM than an Uno can spare for buffers and a scene.

Upstream: https://github.com/olikraus/u8g2

## Installation

Repository: https://github.com/Astonikum/u8g2-lib

PlatformIO (`platformio.ini`):

```ini
lib_deps =
  https://github.com/Astonikum/u8g2-lib.git#v0.2.1
```

PlatformIO resolves the core, adapter support and vendor dependencies automatically.

Arduino IDE: download **Code → Download ZIP**, then use **Sketch → Include Library → Add .ZIP Library**.
Install these GitHub ZIP libraries first:

- [MadDisplay](https://github.com/Astonikum/display-lib)
- [MadDisplayAdapters](https://github.com/Astonikum/adapter-common-lib)

Install from Arduino Library Manager: U8g2.

These packages are distributed through GitHub; they are not yet indexed in the Arduino or PlatformIO registries. Arduino IDE does not install a library directly from a Git URL.

Requires C++14 on the host. Embedded examples select their board and its Arduino toolchain in `platformio.ini`. RAM requirements are documented above; a matching controller does not guarantee a board has enough memory.
