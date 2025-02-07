#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

#include <string.h>

#include "General.h"
#include "myMacros.h"
#include "Supermarket.h"
#include "SuperFile.h"

int main(int argc, char*  argv[])
{
	SuperMarket	market;

    srand(time(NULL));

    int compressed;
	char* superFileName[256];
	int numOfArgs = 0;

	if (argc != 3)
		ON_EXIT(NOT_ENOUGH_ARGS);

	numOfArgs = sscanf(argv[1], "%d", &compressed) + sscanf(argv[2], "%s", superFileName);



	if (numOfArgs != 2)
		ON_EXIT(ARGS_SCAN_FAIL);

	if (compressed != 0 && compressed != 1)
		ON_EXIT(WRONG_COMPRESS_INPUT);


	if (!compressed && strcmp(superFileName, SUPER_FILE_NAME) || compressed && strcmp(superFileName, COMPRESSED_SUPER_FILE_NAME))
		ON_EXIT(WRONG_FILE_NAME);

	if (!initSuperMarket(&market, superFileName, compressed, CUSTOMER_FILE_NAME))
	{
		printf("Error init Super Market");
		return 0;
	}

	int stop = 0;
	
	do
	{
		int option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;

		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if(!doShopping(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerManageShoppingCart:
			if(!manageShoppingCart(&market))
				printf("Error in shopping cart management\n");
			break;

		case eSortProducts:
			sortProducts(&market);
			break;

		case eSearchProduct:
			findProduct(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case EXIT:
			printMessage("Thank", "You", "For", "Shopping", "With", "Us", NULL);
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	handleCustomerStillShoppingAtExit(&market);

	if (compressed ? !saveSuperMarketToCompressedFile(&market, superFileName, CUSTOMER_FILE_NAME) :
                    !saveSuperMarketToFile(&market, superFileName, CUSTOMER_FILE_NAME))
		printf("Error saving supermarket to file\n");

	freeMarket(&market);
	
	return 1;
}

int menu()
{
	int option;
	printf("\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

void printUsage()
{
	printf("To use this program you need to enter:\n"
		"1) 0 for regular file, 1 for compressed file\n"
		"2) The file name of the matched file\n");
}
