#include "chelper.h"

void set_default_value_string(const char *eval_str, const char *default_value, char *out_str) {
    if (eval_str == NULL) {
        // If eval_str is NULL, set out_str to the default_value
        strcpy(out_str, default_value);
    } else {
        // Otherwise, copy eval_str to out_str
        strcpy(out_str, eval_str);
    }
}
