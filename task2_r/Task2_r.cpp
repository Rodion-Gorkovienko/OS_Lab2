#include "Menu.h"
#include "MappingReading.h"


auto form_menu() {
	using namespace std;
	auto main_menu = std::make_shared<Menu>("Mapping a file: reading");
	main_menu->add_item("Open", mapping_open);
	main_menu->add_item("Read", mapping_read);
	main_menu->add_item("Close", mapping_close);

	main_menu->add_property(make_shared<string>("Maping name"), make_shared<string>("MyMap"));
	
	return main_menu;
}

int main()
{
	form_menu()->execute();
}
