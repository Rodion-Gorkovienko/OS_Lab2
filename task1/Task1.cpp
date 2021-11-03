#include "Menu.h"
#include "Functions.h"


auto form_menu() {
	using namespace std;
	auto main_menu = std::make_shared<Menu>("Main menu");
	main_menu->add_item("Get system information", get_system_info);
	auto mem_menu = main_menu->add_item("Actions with memory");

	//Memory
	mem_menu->add_item("Get memory status", get_mem_status);
	mem_menu->add_item("Get region status", get_reg_status);
	auto reserv_menu = mem_menu->add_item("Region reservation");
	mem_menu->add_item("Free region", region_free);
	mem_menu->add_item("Read data from the region", region_read);
	mem_menu->add_item("Write data to the region", region_write);
	auto protect_menu = mem_menu->add_item("Set protection on region");

	//Reservation
	reserv_menu->add_item("Automatically reserve region", auto_reserv);
	reserv_menu->add_item("Automatically reserve region and transfer physical memory", auto_reserv_phys);
	reserv_menu->add_item("Reserve a current region", region_reserv);
	reserv_menu->add_item("Reserve a current region and transfer physical memory", region_reserv_phys);

	//Set protection
	auto Ex = protect_menu->add_item("Execute (PAGE_EXECUTE)", region_protection);
	auto ExR = protect_menu->add_item("Execute or read-only (PAGE_EXECUTE_READ)", region_protection);
	auto ExRW = protect_menu->add_item("Execute, read-only, or read/write (PAGE_EXECUTE_READWRITE)", region_protection);
	auto N = protect_menu->add_item("Close access (PAGE_NOACCESS)", region_protection);
	auto RO = protect_menu->add_item("Read-only (PAGE_READONLY)", region_protection);
	auto RW = protect_menu->add_item("Read-only or read/write (PAGE_READWRITE)", region_protection);
	
	//Property of items
	mem_menu->add_property(make_shared<string>("Pointer to the current region"), make_shared<string>("INDEFINITE"));
	Ex->add_property(make_shared<string>("Protect flag"), make_shared<string>("PAGE_EXECUTE"));
	ExR->add_property(make_shared<string>("Protect flag"), make_shared<string>("PAGE_EXECUTE_READ"));
	ExRW->add_property(make_shared<string>("Protect flag"), make_shared<string>("PAGE_EXECUTE_READWRITE"));
	N->add_property(make_shared<string>("Protect flag"), make_shared<string>("PAGE_NOACCESS"));
	RO->add_property(make_shared<string>("Protect flag"), make_shared<string>("PAGE_READONLY"));
	RW->add_property(make_shared<string>("Protect flag"), make_shared<string>("PAGE_READWRITE"));

	return main_menu;
}

int main()
{
	form_menu()->execute();
}
