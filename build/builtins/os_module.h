// Tauraro os Module Header
// Auto-generated - DO NOT EDIT

#ifndef TAURARO_OS_MODULE_H
#define TAURARO_OS_MODULE_H

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
void tauraro_os_module_init(void);

// Module attribute access
tauraro_value_t* tauraro_os_get_attr(const char* name);

// Module function declarations

tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_chdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_listdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_mkdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rmdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_remove(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rename(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_getenv(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_putenv(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_system(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_exists(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isfile(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_join(int argc, tauraro_value_t** argv);


#endif // TAURARO_OS_MODULE_H
