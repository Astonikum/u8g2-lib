#include <MadDisplay.h>
#include <MadU8g2.h>
// Hardware I2C pins follow the selected Arduino board.
U8G2_SH1106_128X64_NONAME_F_HW_I2C driver(U8G2_R0,U8X8_PIN_NONE);
mad::U8g2Adapter<128,64> adapter(driver,mad::monoPanelSpec(128,64,mad::PanelBus::I2C,mad::Color::Blue));
mad::Display<decltype(adapter)> display(adapter);
mad::VerticalLayout root;
mad::Page page(root,mad::Color::Black);
mad::Label title("MAD DISPLAY",mad::LabelStyle(mad::Color::White,1));
void setup() {
  if (!display.begin()) while (true) { delay(100); }
  root.add(title); display.setFps(20);
  display.openPage(display.registerPage(page));
}
void loop() { display.render(micros()); }
