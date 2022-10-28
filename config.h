
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <nlohmann/json.hpp>

namespace config {
	class mqtt {
		public:
		std::string	broker;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(mqtt, broker)
	};
	class bus {
		public:
		int	id;
		std::string	device;
		std::string	baudrate;
		std::string	mode;
		std::string	address;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(bus, id, device, baudrate, mode, address)
	};
};

#endif
