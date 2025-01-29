#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "FileHelper.h"
#include "General.h"
#include "SuperFile.h"


int	 writeStringToFile(const char* str, FILE* fp, const char* msg)
{
	int length = (int)strlen(str);
	length++;
	if (!writeIntToFile(length, fp, msg))
		return 0;

	if (!writeCharsToFile(str, length, fp, msg))
		return 0;


	return 1;
}

int	 writeCharsToFile(const char* arr, int size, FILE* fp, const char* msg)
{
	if (fwrite(arr, sizeof(char), size, fp) != size)
	{
		puts(msg);
		return 0;
	}
	return 1;

}

int	 writeIntToFile(int val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int writeFloatToFile(float val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(float), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int writeProductToCompressedFile(const Product* pP, FILE* fp, const char* msg)
{
    BYTE data[9] = { 0 };
    data[0] = (pP->barcode[2] - '0') << 4 | (pP->barcode[3] - '0') & 0xF;
    data[1] = (pP->barcode[4] - '0') << 4 | (pP->barcode[5] - '0') & 0xF;
    data[2] = (pP->barcode[6] - '0') << 4 | (pP->type & 0x3) << 2 | strlen(pP->name) >> 2;
    data[3] = strlen(pP->name) << 6;

    data[4] = pP->count;
    data[5] = ((int)ceilf(pP->price * 100) % 100) << 1 | (int)pP->price >> 8;
    data[6] = (int)pP->price;

    data[7] = pP->expiryDate.day << 3 | pP->expiryDate.month >> 1;
    data[8] = pP->expiryDate.month << 7 | (pP->expiryDate.year - 2024) << 4;

    if (fwrite(data, sizeof(BYTE), 4, fp) != 4 ||
        fwrite(pP->name, sizeof(char), strlen(pP->name), fp) != strlen(pP->name) ||
        fwrite(data + 4, sizeof(BYTE), 5, fp) != 5)
    {
        puts(msg);
        return 0;
    }
    return 1;
}

char* readStringFromFile(FILE* fp, const char* msg)
{
	char* str;
	int length;
	if (!readIntFromFile(&length, fp, msg))
		return NULL;
	str = (char*)malloc((length) * sizeof(char));
	if (!str)
	{
		puts(msg);
		return NULL;
	}
	if (fread(str, sizeof(char), length, fp) != length)
	{
		free(str);
		puts(msg);
		return NULL;
	}
	return str;
}

int readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg)
{
	int len;
	if (!readIntFromFile(&len, fp, msg))
		return 0;

	if (!readCharsFromFile(arr, len, fp, msg))
		return 0;

	return 1;
}

int	 readCharsFromFile(char* arr, int length, FILE* fp, const char* msg)
{
	if (fread(arr, sizeof(char), length, fp) != length)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int	  readIntFromFile(int* pVal, FILE* fp, const char* msg)
{
	if (fread(pVal, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;

}

int readFloatFromFile(float* val, FILE* fp, const char* msg)
{
	if (fread(val, sizeof(float), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}


char* readDynStringFromTextFile(FILE* fp)
{
	char temp[MAX_STR_LEN];
	myGets(temp, sizeof(temp), fp);
	return getDynStr(temp);
}

char* readSupermarketNameFromCompressedFile(int len, FILE* fp)
{
    char* name = (char*)malloc(sizeof(char) * (len + 1));
    if (!name)
        return NULL;

    if (fread(name, sizeof(char), len, fp) != len)
    {
        printf("Error reading supermarket name\n");
        free(name);
        return NULL;
    }
    name[len] = '\0';
    return name;
}

int readProductFromCompressedFile(Product* pP, FILE* fp, const char* msg)
{
    BYTE data[9];
    if (fread(data, sizeof(BYTE), 4, fp) != 4)
    {
        puts(msg);
        return 0;
    }

    int len = (data[2] & 0x3) << 2 | data[3] >> 6;
    if (fread(pP->name, sizeof(char), len, fp) != len ||
        fread(data + 4, sizeof(BYTE), 5, fp) != 5)
    {
        puts(msg);
        return 0;
    }
    pP->name[len] = '\0';

    initProductFromCompressedData(pP, data);
    return 1;
}

int  initSupermarketFromCompressedData(SuperMarket* pMarket, BYTE* data, FILE* fp)
{
    pMarket->productCount = data[0] << 2 | data[1] >> 6;
    int len = data[1] & 0x3F;

    pMarket->name = readSupermarketNameFromCompressedFile(len, fp);
    if (!pMarket->name)
    {
        fclose(fp);
        return 0;
    }

    pMarket->productArr = createProductArrFromCompressedFile(pMarket->productCount, fp);
    if (!pMarket->productArr)
    {
        free(pMarket->name);
        fclose(fp);
        return 0;
    }
}

void initProductFromCompressedData(Product * pP, BYTE* data)
{
    pP->type = (data[2] >> 2) & 0x3;
    strcpy(pP->barcode, getProductTypePrefix(pP->type));
    pP->barcode[2] = (data[0] >> 4) + '0';
    pP->barcode[3] = (data[0] & 0xF) + '0';
    pP->barcode[4] = (data[1] >> 4) + '0';
    pP->barcode[5] = (data[1] & 0xF) + '0';
    pP->barcode[6] = (data[2] >> 4) + '0';
    pP->barcode[7] = '\0';

    pP->count = data[4];
    pP->price = (float)(data[5] >> 1) / 100 + ((data[5] & 0x1) << 8 | data[6]);

    pP->expiryDate.day = data[7] >> 3;
    pP->expiryDate.month = (data[7] & 0x7) << 1 | data[8] >> 7;
    pP->expiryDate.year = ((data[8] >> 4) & 0x7) + 2024;
}

Product** createProductArrFromCompressedFile(int size, FILE* fp)
{
    Product** arr = (Product**)malloc(sizeof(Product*) * size);
    if (!arr)
        return NULL;

    for (int i = 0; i < size; i++)
    {
        arr[i] = (Product*)malloc(sizeof(Product));
        if (!arr[i] || !readProductFromCompressedFile(arr[i], fp, "Error reading product\n"))
        {
            for(int j = 0; j <= i; j++)
                if (arr[i])
                {
                    freeProduct(arr[i]);
                    free(arr[i]);
                }
            return NULL;
        }
    }
    
    return arr;
}
