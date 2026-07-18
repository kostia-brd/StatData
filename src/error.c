#include "error.h"

const char *SDResultString(SDResult err)
{
    switch (err)
    {
        case SD_OK:
            return "Success";

        case SD_ERR_ARGUMENT:
            return "Invalid argument";

        case SD_ERR_IO:
            return "I/O error";

        case SD_ERR_MEMORY:
            return "Out of memory";

        default:
            return "Unknown error";
    }
}