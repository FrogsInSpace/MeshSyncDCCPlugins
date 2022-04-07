#pragma once

#include <string>

class msblenContextPathProvider {
public:
	virtual std::string get_path(const Object* obj) = 0;
	virtual std::string get_path(const Object* arm, const Bone* obj) = 0;
};