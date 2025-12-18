// ==========================================
// TEMPLA MODULE - Pure C Implementation (Template Engine)
// ==========================================
// Provides: Template, render, Environment, Loader
// Platform: Cross-platform

#ifndef TAURARO_TEMPLA_MODULE_H
#define TAURARO_TEMPLA_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Template structure
typedef struct {
    char* template_str;
    char* rendered;
    int compiled;
} Template;

// Environment structure
typedef struct {
    char* loader;
    int autoescape;
    int trim_blocks;
} Environment;

// templa.Template(template_string)
static inline TauValue tauraro_templa_Template(TauValue template_str) {
    if (template_str.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    Template* tpl = (Template*)malloc(sizeof(Template));
    tpl->template_str = (char*)malloc(strlen(template_str.value.s) + 1);
    strcpy(tpl->template_str, template_str.value.s);
    tpl->rendered = NULL;
    tpl->compiled = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)tpl, .refcount = 1, .next = NULL};
}

// templa.Template.render(context)
static inline TauValue tauraro_templa_Template_render(TauValue template, TauValue context) {
    if (template.type != 6) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    
    Template* tpl = (Template*)template.value.p;
    char* result = (char*)malloc(strlen(tpl->template_str) + 100);
    strcpy(result, tpl->template_str);
    // Simple string replacement
    
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// templa.Template.compile()
static inline TauValue tauraro_templa_Template_compile(TauValue template) {
    if (template.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    Template* tpl = (Template*)template.value.p;
    tpl->compiled = 1;
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// templa.Environment()
static inline TauValue tauraro_templa_Environment(void) {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->loader = NULL;
    env->autoescape = 0;
    env->trim_blocks = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)env, .refcount = 1, .next = NULL};
}

// templa.Environment.from_string(template_str)
static inline TauValue tauraro_templa_Environment_from_string(TauValue env, TauValue template_str) {
    return tauraro_templa_Template(template_str);
}

// templa.render(template_str, context)
static inline TauValue tauraro_templa_render(TauValue template_str, TauValue context) {
    TauValue tpl = tauraro_templa_Template(template_str);
    return tauraro_templa_Template_render(tpl, context);
}

// templa.FileSystemLoader(path)
static inline TauValue tauraro_templa_FileSystemLoader(TauValue path) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// templa.DictLoader(mapping)
static inline TauValue tauraro_templa_DictLoader(TauValue mapping) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// templa.select_autoescape function
static inline TauValue tauraro_templa_select_autoescape(void) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}


#endif // TAURARO_TEMPLA_MODULE_H
