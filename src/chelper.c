#include <string.h>
#include "chelper.h"

void set_default_value_string(const char *eval_str, const char *default_value, char *out_str) {
    if (eval_str == NULL) {
        // Default value is too big for the output buffer
        if (strlen(default_value) + 1 > sizeof(out_str)) {
            return;
        }
        strcpy(out_str, default_value);
    } else {
        // Otherwise, copy eval_str to out_str
        if (strlen(eval_str) + 1 >sizeof(out_str)) {
            return;
        }
        strcpy(out_str, eval_str);
    }
}
