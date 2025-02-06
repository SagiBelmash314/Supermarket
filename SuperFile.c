#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileHelper.h"
#include "SuperFile.h"

#include "myMacros.h"
#include "Product.h"


int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp,"Error open supermarket file to write\n");

	if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n"))
	{
		CLOSE_RETURN_0(fp);
	}

	if (!writeIntToFile(pMarket->productCount, fp, "Error write product count\n"))
	{
		CLOSE_RETURN_0(fp);
	}

	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (!saveProductToFile(pMarket->productArr[i], fp))
		{
		CLOSE_RETURN_0(fp);
		}
	}

	fclose(fp);

	saveCustomersToTextFile(pMarket->customerArr, pMarket->customerCount, customersFileName);

	return 1;
}

int	saveSuperMarketToCompressedFile(const SuperMarket* pMarket, const char* fileName, const char* customersFileName)
{
    FILE* fp;
    fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp,"Error open supermarket file to write\n");
    int len = strlen(pMarket->name);
    BYTE data[2] = { 0 };
    data[0] = pMarket->productCount >> 2;
    data[1] = pMarket->productCount << 6 | len;
    if (fwrite(data, sizeof(BYTE), 2, fp) != 2 ||
        fwrite(pMarket->name, sizeof(char), len, fp) != len) {
        printf("Error write supermarket\n");
        fclose(fp);
        return 0;
    }

    for (int i = 0; i < pMarket->productCount; i++)
        if (!writeProductToCompressedFile(pMarket->productArr[i], fp, "Error write product\n"))
        {
			CLOSE_RETURN_0(fp);
        }

    fclose(fp);
	saveCustomersToTextFile(pMarket->customerArr, pMarket->customerCount, customersFileName);
    return 1;
}

int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName, const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp,"Error opening company file\n");
	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n");
	if (!pMarket->name)
	{
		CLOSE_RETURN_0(fp);
	}

	int count;

	if (!readIntFromFile(&count, fp, "Error reading product count\n")) {
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
	}

	pMarket->productArr = (Product**)malloc(count * sizeof(Product*));
	if (!pMarket->productArr) {
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
	}

	pMarket->productCount = count;

	for (int i = 0; i < count; i++)
	{
		pMarket->productArr[i] = (Product*)malloc(sizeof(Product));
		if (!pMarket->productArr[i]) {
			FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
		}

		if (!loadProductFromFile(pMarket->productArr[i], fp))
		{
			free(pMarket->productArr[i]);
			FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
		}
	}


	fclose(fp);

	pMarket->customerArr = loadCustomersFromTextFile(customersFileName, &pMarket->customerCount);
	CHECK_RETURN_0(pMarket->customerArr);
	return	1;

}

int	loadSuperMarketFromCompressedFile(SuperMarket* pMarket, const char* fileName, const char* customersFileName)
{
    FILE* fp;
    fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp,"Error opening company file\n");

    BYTE data[2] = { 0 };

    if (fread(data, 1, 2, fp) != 2)
    {
        printf("Error reading supermarket\n");
        CLOSE_RETURN_0(fp);
    }

    if (!initSupermarketFromCompressedData(pMarket, data, fp)) {
    	CLOSE_RETURN_0(fp);
    }

    fclose(fp);

    pMarket->customerArr = loadCustomersFromTextFile(customersFileName, &pMarket->customerCount);
	CHECK_RETURN_0(pMarket->customerArr);
    return 1;
}

int	saveCustomersToTextFile(const Customer* customerArr, int customerCount, const char* customersFileName)
{
	FILE *fp = fopen(customersFileName, "w");
    CHECK_MSG_RETURN_0(fp,"Error opening customers file to write\n");
	fprintf(fp, "%d\n", customerCount);
	for (int i = 0; i < customerCount; i++)
		customerArr[i].vTable.saveToFile(&customerArr[i], fp);

	fclose(fp);
	return 1;
}

Customer* loadCustomersFromTextFile(const char* customersFileName, int* pCount)
{
	FILE* fp;

	fp = fopen(customersFileName, "r");
    CHECK_MSG_RETURN_0(fp,"Error opening customers file to write\n");

	Customer* customerArr = NULL;
	int customerCount;

	fscanf(fp, "%d\n", &customerCount);

	if (customerCount > 0)
	{
		customerArr = (Customer*)calloc(customerCount, sizeof(Customer)); //cart will be NULL!!!
		if (!customerArr) {
			CLOSE_RETURN_0(fp);
		}

		for (int i = 0; i < customerCount; i++)
		{
			if (!loadCustomerFromFile(&customerArr[i], fp))
			{
				freeCustomerCloseFile(customerArr, i, fp);
				return NULL;
			}
		}
	}

	fclose(fp);
	*pCount = customerCount;
	return customerArr;
}


void freeCustomerCloseFile(Customer* customerArr, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		free(customerArr[i].name);
		customerArr[i].name = NULL;
		if (customerArr[i].pDerivedObj)
		{
			free(customerArr[i].pDerivedObj);
			customerArr[i].pDerivedObj = NULL;
		}
	}
	free(customerArr);
	fclose(fp);
}