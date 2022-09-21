
#include <string>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include <boost/program_options.hpp>

#include "bus.h"
#include "message.h"
#include "vtype.h"
#include "AddressMap.h"

namespace po = boost::program_options;

using json = nlohmann::json;

static std::map<std::string,VType*>	vtypemap;

void messageformat_value_type_parse(json& messageformat) {
	for(auto& [key, tdef] : messageformat["types"].items()) {
		std::string	keys=key;
		if (!tdef.contains("type")) {
			std::cerr << "Type definition needs internal type name" << std::endl;
			exit(-1);
		}

		std::string	type=tdef["type"].get<std::string>();
		int		size=tdef["size"].get<int>();
 
		if (type == "float") {
			int	divisor=tdef["divisor"].get<int>();
			std::string	unit=tdef["unit"].get<std::string>();
			VType	*p=new  VTypeFloat(size, unit, divisor);
			vtypemap.insert(std::make_pair(keys, p));
		} else if (type == "datetime") {
			VType *p=new VTypeDateTime(size);
			vtypemap.insert(std::make_pair(keys,p));
		} else if (type == "unknown") {
			VType *p=new VTypeUnknown();
			vtypemap.insert(std::make_pair(keys,p));
		} else if (type == "bit") {
			std::string	set=tdef["set"].get<std::string>();
			std::string	unset=tdef["unset"].get<std::string>();
			VType *p=new VTypeBit(size, set, unset);
			vtypemap.insert(std::make_pair(keys,p));
		}
	}
	
	for(auto& [name, vtype] : vtypemap) {
		std::cout << name << std::endl;
	}
}

void messageformat_parse(json& messageformat) {
	for(auto& mtype : messageformat["messages"]) {

		if (!mtype.contains("type")) {
			std::cerr << "Need type" << std::endl;
			exit(-1);
		}

		std::string	type=mtype["type"].get<std::string>();
		std::string name=mtype["name"].get<std::string>();
		uint32_t	address=std::stoul(mtype["address"].get<std::string>(), nullptr, 16);
		
		VType	*vt;

		try {
				vt=vtypemap.at(type);
		} catch (std::out_of_range& e) {
			std::cerr << "Unknown public value type " << type << std::endl;
			exit(-1);
		}
	
		Address *aptr=new Address(address, vt, name);
		addressmap.add(aptr);	
	}
}

void messageformat_parse(std::string filename) {
	std::ifstream	file(filename);
	json		messageformat=json::parse(file);

	messageformat_value_type_parse(messageformat);
	messageformat_parse(messageformat);
}

int main(int argc, char **argv) {
	po::options_description	desc("Allowed options");

	desc.add_options()
		("help,h",	"produce help message")
		("config,i",	po::value<std::string>()->required(), "Config file")
	;

	po::variables_map	vm;

	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	} catch(const boost::program_options::error& e) {
		std::cerr << "Error: " << e.what() << "\n";
		std::cout << desc << std::endl;
		exit(-1);
	}


	std::ifstream configstream(vm["config"].as<std::string>());	
	json config=json::parse(configstream);

	messageformat_parse(config["messageformat"].get<std::string>());
	std::vector<Bus>	busses;

	for(auto &busconfig : config["busses"]) {
		std::cout << busconfig.dump() << std::endl;
		Bus	newbus(busconfig);
	}

	std::cout << config.dump() << std::endl;
}