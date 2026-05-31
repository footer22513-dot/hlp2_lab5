#pragma once

namespace GNA {

void load_users_from_files();
void save_users_to_files();
int menu_load_database();
int menu_unload_database();
int menu_show_all_workers();
int menu_show_all_suppliers();
int menu_add_worker();
int menu_add_supplier();
int menu_delete_worker();
int menu_delete_supplier();
int menu_edit_worker();
int menu_edit_supplier();
int menu_search_workers();
int menu_search_suppliers();
int menu_sort();
int menu_show_production();
int menu_show_sales();
int menu_add_production();
int menu_add_sale();
int menu_create_report();
int menu_show_product();
int menu_polymorphism_demo();
int menu_add_product_to_supplier();

}
