#ifndef MQTT_H
#define MQTT_H

#include "config.h"
#include <iostream>

class MQTT {
	private:
		config::mqtt	config;
	public:
		MQTT(config::mqtt& config) : config(config) {
			std::cout << config.broker << std::endl;
		}
};

#endif
