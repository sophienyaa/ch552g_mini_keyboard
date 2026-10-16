FQBN = CH55xDuino:mcs51:ch552:usb_settings=user148,upload_method=usb,clock=16internal,bootloader_pin=p36

.PHONY: all compile upload clean configurator

all: compile

compile:
	arduino-cli compile -b $(FQBN) .

upload:
	arduino-cli upload -b $(FQBN) .

configurator:
	open -a "Google Chrome" tools/index.html 2>/dev/null || open tools/index.html

clean:
	rm -rf build
