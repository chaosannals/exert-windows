#include "topview.h"

struct topview_state_t {
	std::string tag;
};

topview_t::topview_t() {

}

topview_t::~topview_t() {

}

const std::string& topview_t::get_tag() const {
	return state->tag;
}
std::string topview_t::get_attr(std::string& name) {

}
void topview_t::set_attr(std::string& name, std::string& value) {

}

topview_t topview_t::get_child(std::string& tag) {

}

std::vector<topview_t::baseview_t> topview_t::get_children() {

}

void topview_t::paint(HDC parent) {

}

void topview_t::show() {

}