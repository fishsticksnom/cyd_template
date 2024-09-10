#define LGFX_USE_V1

#include <LovyanGFX.hpp>

#define LCD_MOSI 13
#define LCD_MISO 12
#define LCD_SCK 14
#define LCD_CS 15
#define LCD_RST -1
#define LCD_DC 2
#define LCD_BL 21

#define TOUCH_MOSI 32
#define TOUCH_MISO 39
#define TOUCH_SCK 25
#define TOUCH_CS 33
#define TOUCH_IRQ 36

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Touch_XPT2046 _touch_instance;
    lgfx::Light_PWM _light_instance;

public:
    LGFX(void)
    {
        {                                      // Configure bus control settings.
            auto cfg = _bus_instance.config(); // Obtain the configuration structure for the bus.

            // SPI bus configuration
            cfg.spi_host = HSPI_HOST;  // Select the SPI to use (VSPI_HOST or HSPI_HOST)
            cfg.spi_mode = 0;          // Set the SPI communication mode (0 ~ 3)
            cfg.freq_write = 40000000; // SPI clock speed for writing (maximum 80MHz, rounded to an integer value)
            cfg.freq_read = 16000000;  // SPI clock speed for reading
            cfg.spi_3wire = true;      // Set to true if using MOSI pin for receiving
            cfg.use_lock = true;       // Set to true if using transaction lock
            cfg.dma_channel = 1;       // Set the DMA channel (1 or 2. 0=disable)
            cfg.pin_sclk = LCD_SCK;    // Set the SCLK pin number for SPI
            cfg.pin_mosi = LCD_MOSI;   // Set the MOSI pin number for SPI
            cfg.pin_miso = LCD_MISO;   // Set the MISO pin number for SPI (-1 = disable)
            cfg.pin_dc = LCD_DC;       // Set the D/C pin number for SPI (-1 = disable)
                                       // If sharing the SPI bus with an SD card, do not omit MISO; always set it.

            _bus_instance.config(cfg);              // Apply the configuration values to the bus.
            _panel_instance.setBus(&_bus_instance); // Set the bus to the panel.
        }

        {                                        // Configure display panel control settings.
            auto cfg = _panel_instance.config(); // Obtain the configuration structure for the display panel.

            cfg.pin_cs = LCD_CS;   // Pin number where CS is connected (-1 = disable)
            cfg.pin_rst = LCD_RST; // Pin number where RST is connected (-1 = disable)
            cfg.pin_busy = -1;     // Pin number where BUSY is connected (-1 = disable)

            // * The following settings have general default values for the panel, so try commenting out unknown items if necessary.

            cfg.memory_width = 240;   // Maximum width supported by the driver IC
            cfg.memory_height = 320;  // Maximum height supported by the driver IC
            cfg.panel_width = 240;    // Actual displayable width
            cfg.panel_height = 320;   // Actual displayable height
            cfg.offset_x = 0;         // X-direction offset of the panel
            cfg.offset_y = 0;         // Y-direction offset of the panel
            cfg.offset_rotation = 0;  // Rotation direction offset value 0~7 (4~7 for vertical inversion)
            cfg.dummy_read_pixel = 8; // Number of dummy read bits before pixel read
            cfg.dummy_read_bits = 1;  // Number of dummy read bits before non-pixel data read
            cfg.readable = true;      // Set to true if data reading is possible
            cfg.invert = false;       // Set to true if the panel's light/darkness is inverted
            cfg.rgb_order = false;    // Set to true if the panel's red and blue are swapped
            cfg.dlen_16bit = false;   // Set to true if the panel sends data in 16-bit units
            cfg.bus_shared = true;    // Set to true if sharing the bus with an SD card (e.g., for bus control in drawJpgFile)

            _panel_instance.config(cfg);
        }

        {                                        // Configure backlight control settings (remove if not needed).
            auto cfg = _light_instance.config(); // Obtain the configuration structure for the backlight.

            cfg.pin_bl = LCD_BL; // Pin number where the backlight is connected
            cfg.invert = false;  // Set to true to invert the backlight brightness
            cfg.freq = 44100;    // PWM frequency for the backlight
            cfg.pwm_channel = 7; // PWM channel number to use

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // Set the backlight to the panel.
        }

        { // Configure touchscreen control settings (remove if not needed).
            auto cfg = _touch_instance.config();

            cfg.x_min = 0;           // Minimum X value obtained from the touchscreen (raw value)
            cfg.x_max = 239;         // Maximum X value obtained from the touchscreen (raw value)
            cfg.y_min = 0;           // Minimum Y value obtained from the touchscreen (raw value)
            cfg.y_max = 319;         // Maximum Y value obtained from the touchscreen (raw value)
            cfg.pin_int = TOUCH_IRQ; // Pin number where INT is connected
            cfg.bus_shared = true;   // Set to true if using the same bus as the screen
            cfg.offset_rotation = 0; // Adjustment for mismatch between display and touch orientation, set with values 0~7

            // For SPI connection
            cfg.spi_host = VSPI_HOST;  // Select the SPI to use (HSPI_HOST or VSPI_HOST)
            cfg.freq = 1000000;        // Set SPI clock speed
            cfg.pin_sclk = TOUCH_SCK;  // Pin number where SCLK is connected
            cfg.pin_mosi = TOUCH_MOSI; // Pin number where MOSI is connected
            cfg.pin_miso = TOUCH_MISO; // Pin number where MISO is connected
            cfg.pin_cs = TOUCH_CS;     // Pin number where CS is connected

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
        }

        setPanel(&_panel_instance); // Set the panel to use.
    }
};
;
