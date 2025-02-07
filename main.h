#pragma once

#define EXIT -1
#define NOT_ENOUGH_ARGS -1
#define ARGS_SCAN_FAIL -2
#define WRONG_COMPRESS_INPUT -3
#define MAX_FILE_NAME 256
#define CUSTOMER_FILE_NAME "Customers.txt"

typedef enum
{
	eShowSuperMarket, eAddProduct, eAddCustomer, eCustomerDoShopping, ePrintCart, eCustomerManageShoppingCart,
	eSortProducts, eSearchProduct, ePrintProductByType, eNofOptions
} eMenuOptions;

const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping","Print Shopping Cart","Customer Shopping Cart Managment",
								"Sort Products", "Search Product", "Print Product By Type" };

int menu();
void printUsage();