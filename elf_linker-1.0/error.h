#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum {
    WRITE_OK,
    MISSING_HEADER,
    MISSING_SECTION_HEADER,
    MISSING_SECTION_DATA,
    INVALID_FILE
} WriteError;



#endif