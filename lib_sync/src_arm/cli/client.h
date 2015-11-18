#include <stdint.h>
#include "cli_struct.h"

/* sy_init return the URI_code, which is the repository location */ 
struct sy_session_t *sy_default_session();

/* TODO use uint8_t or METADATAtype */
uint8_t sy_upload(struct sy_session_t *sy_session, struct sy_diff_t *sy_session_diff);
