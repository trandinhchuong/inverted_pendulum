CFLAGS		+= -I./sources/driver/led
#CFLAGS		+= -I./sources/driver/rtc
#CFLAGS		+= -I./sources/driver/button
#CFLAGS		+= -I./sources/driver/ir
CFLAGS		+= -I./sources/driver/flash
#CFLAGS		+= -I./sources/driver/fuzzy_logic
#CFLAGS		+= -I./sources/driver/hs1101
#CFLAGS		+= -I./sources/driver/kalman

#CPPFLAGS	+= -I./sources/driver/SHT1x
#CPPFLAGS	+= -I./sources/driver/thermistor
#CPPFLAGS	+= -I./sources/driver/Adafruit_ssd1306syp
#CPPFLAGS	+= -I./sources/driver/EmonLib
#CPPFLAGS	+= -I./sources/driver/ds1302
CPPFLAGS	+= -I./sources/driver/epprom

VPATH += sources/driver/led
#VPATH += sources/driver/button
#VPATH += sources/driver/rtc
#VPATH += sources/driver/ir
#VPATH += sources/driver/SHT1x
#VPATH += sources/driver/thermistor
#VPATH += sources/driver/Adafruit_ssd1306syp
#VPATH += sources/driver/EmonLib
#VPATH += sources/driver/ds1302
VPATH += sources/driver/eeprom
VPATH += sources/driver/flash
#VPATH += sources/driver/fuzzy_logic
#VPATH += sources/driver/kalman
#VPATH += sources/driver/hs1101

SOURCES += sources/driver/led/led.c
#SOURCES += sources/driver/rtc/rtc.c
#SOURCES += sources/driver/button/button.c
#SOURCES += sources/driver/ir/ir.c
SOURCES += sources/driver/flash/flash.c
#SOURCES += sources/driver/fuzzy_logic/fuzzy_logic.c
#SOURCES += sources/driver/hs1101/hs1101.c
#SOURCES += sources/driver/kalman/kalman.c

#SOURCES_CPP += sources/driver/SHT1x/SHT1x.cpp
#SOURCES_CPP += sources/driver/thermistor/thermistor.cpp
#SOURCES_CPP += sources/driver/Adafruit_ssd1306syp/Adafruit_GFX.cpp
#SOURCES_CPP += sources/driver/Adafruit_ssd1306syp/Adafruit_ssd1306syp.cpp
#SOURCES_CPP += sources/driver/Adafruit_ssd1306syp/glcdfont.cpp
#SOURCES_CPP += sources/driver/EmonLib/EmonLib.cpp
#SOURCES_CPP += sources/driver/ds1302/DS1302.cpp
SOURCES_CPP += sources/driver/eeprom/eeprom.cpp
