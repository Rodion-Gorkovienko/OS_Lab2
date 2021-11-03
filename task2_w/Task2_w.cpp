#include "Menu.h"
#include "MappingWriting.h"


auto form_menu() {
	using namespace std;
	auto main_menu = std::make_shared<Menu>("Mapping a file: writing");
	main_menu->add_item("Create", mapping_crate);
	main_menu->add_item("Write", mapping_write);
	main_menu->add_item("Delete", mapping_delete);

	main_menu->add_property(make_shared<string>("File name"), make_shared<string>("C:\\example4\\test"));
	main_menu->add_property(make_shared<string>("Maping name"), make_shared<string>("MyMap"));
	main_menu->add_property(make_shared<string>("Data to write"), make_shared<string>("Cake is a lie"));
	return main_menu;
}

int main()
{
	form_menu()->execute();
	mapping_delete(nullptr);
}
