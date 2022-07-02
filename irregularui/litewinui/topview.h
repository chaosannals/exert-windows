#pragma once

#include "baseview.h"

struct topview_state_t;

class topview_t : public baseview_t{
	topview_state_t* state;
public:
	topview_t();
	virtual ~topview_t();
	virtual const std::string& get_type()const { return "topview"; }
	virtual const std::string& get_tag()const;
	virtual std::string get_attr(std::string& name);
	virtual void set_attr(std::string& name, std::string& value);
	virtual baseview_t *get_child(std::string& tag);
	virtual std::vector<baseview_t*> get_children();
	virtual void paint(HDC parent);
	void show();
};