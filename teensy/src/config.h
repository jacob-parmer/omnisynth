// This should eventually be replaced by a config file using a LittleFS filesystem. 
// Author: Jacob Parmer
// Date: Feb 7, 2021

const int NUM_PATTERNS = 1;

class Config {

	public:
		Config();
		void initialize();
		const int get_num_leds();

	private:
		int LED_PIN;
		const int NUM_LEDS = 20;
		int BRIGHTNESS;
		char const *LED_TYPE;
		char const *COLOR_ORDER;
		int UPDATES_PER_SECOND;
		char const *PATTERNS[NUM_PATTERNS] = {
			"blink"
		};
};


Config::Config() {
	
	this->LED_PIN = 2;
	//this->NUM_LEDS = 20;
	this->BRIGHTNESS = 64;
	this->LED_TYPE = "WS2812B";
	this->COLOR_ORDER = "GRB";
	this->UPDATES_PER_SECOND = 100;

};

const int Config::get_num_leds() { return this->NUM_LEDS; };