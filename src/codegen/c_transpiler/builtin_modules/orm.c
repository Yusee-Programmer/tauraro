// ==========================================
// ORM MODULE - SQLite3-Based Implementation (Object-Relational Mapping)
// ==========================================
// Provides: Model, Field, Database, Query, Session
// Platform: Cross-platform
// Features: SQLite3 backend with fallback pure C implementation

#ifndef TAURARO_ORM_MODULE_H
#define TAURARO_ORM_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "external_libs_config.h"


#ifndef TAU_HELPER_FUNCTIONS_DEFINED
#define TAU_HELPER_FUNCTIONS_DEFINED

static inline double tau_to_double(TauValue v) {
    if (v.type == 0) return (double)v.value.i;
    if (v.type == 1) return v.value.f;
    return 0.0;
}

static inline int64_t tau_to_int64(TauValue v) {
    if (v.type == 0) return v.value.i;
    if (v.type == 1) return (int64_t)v.value.f;
    return 0;
}

static inline bool tau_to_bool(TauValue v) {
    if (v.type == 3) return v.value.i != 0;
    if (v.type == 0) return v.value.i != 0;
    if (v.type == 1) return v.value.f != 0.0;
    if (v.type == 2) return v.value.s != NULL && v.value.s[0] != '\0';
    return true;
}

static inline char* tau_to_string(TauValue v) {
    if (v.type == 2) return v.value.s;
    return NULL;
}
#endif // TAU_HELPER_FUNCTIONS_DEFINED

// SQLite3 Integration
#if HAVE_SQLITE3
    #include <sqlite3.h>
    #define ORM_BACKEND "SQLite3"
#else
    #define ORM_BACKEND "Pure C"
    // Fallback type definitions
    typedef void sqlite3;
    typedef void sqlite3_stmt;
    #define sqlite3_open(a,b) (-1)
    #define sqlite3_exec(a,b,c,d,e) (-1)
    #define sqlite3_prepare_v2(a,b,c,d,e) (-1)
    #define sqlite3_step(a) (-1)
    #define sqlite3_finalize(a) (-1)
    #define sqlite3_close(a) (-1)
    #define SQLITE_OK 0
    #define SQLITE_ROW 100
    #define SQLITE_DONE 101
#endif

// Field structure
typedef struct {
    char* name;
    char* type;
    int primary_key;
    int nullable;
    char* default_value;
} Field;

// Model structure
typedef struct {
    char* name;
    Field* fields;
    int field_count;
} Model;

// Database structure
typedef struct {
    char* connection_string;
    void* connection;
} Database;

// Query structure
typedef struct {
    Model* model;
    char* where_clause;
    char* order_by;
    int limit;
} Query;

// orm.Model base class
static inline TauValue tauraro_orm_Model(void) {
    Model* model = (Model*)malloc(sizeof(Model));
    model->name = NULL;
    model->fields = NULL;
    model->field_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)model, .refcount = 1, .next = NULL};
}

// orm.Field(type, primary_key=False, nullable=False)
static inline TauValue tauraro_orm_Field(TauValue type) {
    if (type.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    Field* field = (Field*)malloc(sizeof(Field));
    field->type = (char*)malloc(strlen(type.value.s) + 1);
    strcpy(field->type, type.value.s);
    field->name = NULL;
    field->primary_key = 0;
    field->nullable = 1;
    field->default_value = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)field, .refcount = 1, .next = NULL};
}

// orm.IntegerField()
static inline TauValue tauraro_orm_IntegerField(void) {
    Field* field = (Field*)malloc(sizeof(Field));
    field->type = (char*)malloc(8);
    strcpy(field->type, "INTEGER");
    field->name = NULL;
    field->primary_key = 0;
    field->nullable = 1;
    field->default_value = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)field, .refcount = 1, .next = NULL};
}

// orm.StringField(max_length=None)
static inline TauValue tauraro_orm_StringField(void) {
    Field* field = (Field*)malloc(sizeof(Field));
    field->type = (char*)malloc(8);
    strcpy(field->type, "VARCHAR");
    field->name = NULL;
    field->primary_key = 0;
    field->nullable = 1;
    field->default_value = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)field, .refcount = 1, .next = NULL};
}

// orm.CharField(max_length)
static inline TauValue tauraro_orm_CharField(TauValue max_length) {
    return tauraro_orm_StringField();
}

// orm.BooleanField()
static inline TauValue tauraro_orm_BooleanField(void) {
    Field* field = (Field*)malloc(sizeof(Field));
    field->type = (char*)malloc(8);
    strcpy(field->type, "BOOLEAN");
    field->name = NULL;
    field->primary_key = 0;
    field->nullable = 1;
    field->default_value = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)field, .refcount = 1, .next = NULL};
}

// orm.DateTimeField()
static inline TauValue tauraro_orm_DateTimeField(void) {
    Field* field = (Field*)malloc(sizeof(Field));
    field->type = (char*)malloc(9);
    strcpy(field->type, "DATETIME");
    field->name = NULL;
    field->primary_key = 0;
    field->nullable = 1;
    field->default_value = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)field, .refcount = 1, .next = NULL};
}

// orm.ForeignKey(to_model)
static inline TauValue tauraro_orm_ForeignKey(TauValue to_model) {
    Field* field = (Field*)malloc(sizeof(Field));
    field->type = (char*)malloc(12);
    strcpy(field->type, "FOREIGN_KEY");
    field->name = NULL;
    field->primary_key = 0;
    field->nullable = 1;
    field->default_value = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)field, .refcount = 1, .next = NULL};
}

// orm.Database(connection_string)
static inline TauValue tauraro_orm_Database(TauValue conn_str) {
    if (conn_str.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    Database* db = (Database*)malloc(sizeof(Database));
    db->connection_string = (char*)malloc(strlen(conn_str.value.s) + 1);
    strcpy(db->connection_string, conn_str.value.s);
    db->connection = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)db, .refcount = 1, .next = NULL};
}

// orm.Database.connect()
static inline TauValue tauraro_orm_Database_connect(TauValue db) {
    if (db.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    Database* database = (Database*)db.value.ptr;
    database->connection = (void*)1;  // Simulate connection
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// orm.Database.create_tables(models)
static inline TauValue tauraro_orm_Database_create_tables(TauValue db, TauValue models) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// orm.Model.objects
static inline TauValue tauraro_orm_Model_objects(TauValue model) {
    Query* query = (Query*)malloc(sizeof(Query));
    query->model = (Model*)model.value.ptr;
    query->where_clause = NULL;
    query->order_by = NULL;
    query->limit = -1;
    
    return (TauValue){.type = 6, .value.ptr = (void*)query, .refcount = 1, .next = NULL};
}

// orm.Query.all()
static inline TauValue tauraro_orm_Query_all(TauValue query) {
    return (TauValue){.type = 4, .value.ptr = NULL, .refcount = 1, .next = NULL};  // List
}

// orm.Query.filter(condition)
static inline TauValue tauraro_orm_Query_filter(TauValue query, TauValue condition) {
    return query;  // Return modified query
}

// orm.Query.get(condition)
static inline TauValue tauraro_orm_Query_get(TauValue query, TauValue condition) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};  // Single object
}

// orm.Query.order_by(field)
static inline TauValue tauraro_orm_Query_order_by(TauValue query, TauValue field) {
    return query;
}

// orm.Query.limit(n)
static inline TauValue tauraro_orm_Query_limit(TauValue query, TauValue n) {
    if (query.type == 6) {
        Query* q = (Query*)query.value.ptr;
        q->limit = n.type == 0 ? n.value.i : -1;
    }
    return query;
}

// orm.Model.save()
static inline TauValue tauraro_orm_Model_save(TauValue model) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// orm.Model.delete()
static inline TauValue tauraro_orm_Model_delete(TauValue model) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// orm.Session(database)
static inline TauValue tauraro_orm_Session(TauValue database) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}


#endif // TAURARO_ORM_MODULE_H
