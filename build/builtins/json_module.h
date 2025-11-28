// Tauraro json Module Header
// Auto-generated - DO NOT EDIT

#ifndef TAURARO_JSON_MODULE_H
#define TAURARO_JSON_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations from main tauraro runtime
typedef struct tauraro_value tauraro_value_t;
extern tauraro_value_t* tauraro_value_new(void);
extern tauraro_value_t* tauraro_none(void);
extern tauraro_value_t* tauraro_int(long long val);
extern tauraro_value_t* tauraro_float(double val);
extern tauraro_value_t* tauraro_bool(int val);
extern tauraro_value_t* tauraro_string(const char* val);

// Module initialization
void tauraro_json_module_init(void);

// Module attribute access
tauraro_value_t* tauraro_json_get_attr(const char* name);

// Module function declarations

tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_dump(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_load(int argc, tauraro_value_t** argv);


#endif // TAURARO_JSON_MODULE_H
