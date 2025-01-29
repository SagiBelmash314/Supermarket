#pragma once

#include "Product.h"
#include "SuperFile.h"

int		        writeStringToFile(const char* str, FILE* fp, const char* msg);
int		        writeCharsToFile(const char* arr, int size, FILE* fp, const char* msg);
int		        writeIntToFile(int val, FILE* fp, const char* msg);
int		        writeFloatToFile(float val, FILE* fp, const char* msg);
int             writeProductToCompressedFile(const Product* pP, FILE* fp, const char* msg);


char*	        readStringFromFile(FILE* fp, const char* msg);
char*	        readDynStringFromTextFile(FILE* fp);
int		        readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg);
int		        readCharsFromFile(char* arr, int size, FILE* fp, const char* msg);
int		        readIntFromFile(int* val, FILE* fp, const char* msg);
int		        readFloatFromFile(float* val, FILE* fp, const char* msg);
int             readProductFromCompressedFile(Product* pP, FILE* fp, const char* msg);
char*           readSupermarketNameFromCompressedFile(int len, FILE* fp);

int             initSupermarketFromCompressedData(SuperMarket* pMarket, BYTE* data, FILE* fp);
void            initProductFromCompressedData(Product * pP, BYTE* data);
Product**       createProductArrFromCompressedFile(int size, FILE* fp);
