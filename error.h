#ifndef __ERROR_H__
#define __ERROR_H__


#define TRUE (1)
#define FALSE (0)

typedef enum {
    SUCCESS_STATUS,
    GENERIC_FAILED_STATUS
} error_status_t;


#define CHECK_ERR(OP, ERR_VAL) do { if (!(OP)) { ret_status = ERR_VAL; goto cleanup; }; } while(0);
#define CHECK(OP) do { if (!(OP)) { ret_status = GENERIC_FAILED_STATUS; goto cleanup; }; } while(0);

#endif //__ERROR_H__
