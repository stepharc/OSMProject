#pragma once

#include <string>


class Myreadosm_tag
{
public:
	Myreadosm_tag();
	~Myreadosm_tag();

	std::string getKey(void);
	void setKey(std::string);

	std::string getValue(void);
	void setValue(std::string);

private:
	std::string key; /**< the KEY */
	std::string value; /**< the VALUE */
};

