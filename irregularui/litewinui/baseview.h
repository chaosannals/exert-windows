#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <string>

struct attr_t {
	char type;
	union {
		bool bv;
		int iv;
		double dv;
		char* sv;
	} data;
};

struct baseview_t {
	virtual ~baseview_t() = 0;
	virtual const std::string& get_type()const = 0;
	virtual const std::string& get_tag()const = 0;
	virtual std::string get_attr(std::string& name) = 0;
	virtual void set_attr(std::string& name, std::string& value) = 0;
	virtual baseview_t* get_child(std::string& tag) = 0;
	virtual std::vector<baseview_t*> get_children() = 0;
	virtual void paint(HDC parent) = 0;
};