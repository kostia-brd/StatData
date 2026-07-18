#ifndef ERROR_H
#define ERROR_H

typedef enum
{
    SD_OK = 0,
    SD_ERR_ARGUMENT,
    SD_ERR_IO,
    SD_ERR_MEMORY
	
} SDResult;

const char *SDResultString(SDResult err);

#endif