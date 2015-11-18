#ifndef _SY_COMMON_H_
#define _SY_COMMON_H_

#ifndef WRITE_DATA_SIZE
#define WRITE_DATA_SIZE 2048
#endif 

#ifndef RTCDC_DATA_SIZE
#define RTCDC_DATA_SIZE 1024
#endif 

#ifndef REPO_NAME_SIZE
#define REPO_NAME_SIZE 32
#endif 

#ifndef ID_SIZE
#define ID_SIZE 32
#endif 

#ifndef FILE_NAME_SIZE
#define FILE_NAME_SIZE 32
#endif 

#ifndef LOCAL_PATH_SIZE
#define LOCAL_PATH_SIZE 64
#endif 

typedef enum {
    FS_REGISTER = 0,
    FS_REGISTER_OK,
    FS_REGISTER_FAIL,
    UPLOAD_REQUEST,
    UPLOAD_READY,
    UPLOAD_OK
} METADATAtype;


typedef enum {
    FILE_CLEAN = 0,
    FILE_CLIENT_LACK,
    FILE_FS_LACK,
    FILE_DIRTY,
} FILEstatus;


struct sy_diff_t{
    uint8_t num;
    struct diff_info_t *files_diff;
};


struct diff_info_t{
    char filename[FILE_NAME_SIZE];
    FILEstatus dirty;
    /* 1 is dirty, while 0 is not */
};


#endif // _SY_COMMON_H_
