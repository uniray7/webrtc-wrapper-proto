#ifndef _SY_CLI_STRUCT_H_
#define _SY_CLI_STRUCT_H_

#include "../common.h"

struct sy_session_t{
    char *session_id;
    char *URI_code;
    struct libwebsocket *wsi;
    struct libwebsocket_context *context;
    char *local_repo_path;
    char repo_name[REPO_NAME_SIZE];
};


#endif //_SY_CLI_STRUCT_H_
