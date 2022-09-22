#ifndef VTYPE_H
#define VTYPE_H

#include <string>
#include <map>

namespace VT {
	class Default {
		public:
		Default(std::string name, VT::Default *vt);
		virtual std::string decode(std::string value) const;
	};
}

extern std::map<std::string,VT::Default*>	vtmap;
#endif
