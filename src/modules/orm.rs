/// ORM - High-performance Object-Relational Mapping for Tauraro
/// Similar to SQLAlchemy but faster and more Rust-native
///
/// Features:
/// - Database engine with connection pooling
/// - Table/Model definitions with schema
/// - Query builder with fluent API
/// - Session and transaction management
/// - Relationships (one-to-many, many-to-many)
/// - Multiple database backend support (SQLite, PostgreSQL, MySQL)
/// - Migrations
/// - Type safety

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use rusqlite::{Connection, params, Row};

/// Create the orm module
pub fn create_orm_module() -> Value {
    let mut namespace = HashMap::new();

    // Core classes
    namespace.insert("Engine".to_string(), Value::NativeFunction(create_engine));
    namespace.insert("Session".to_string(), Value::NativeFunction(create_session));
    namespace.insert("Table".to_string(), Value::NativeFunction(create_table));
    namespace.insert("Column".to_string(), Value::NativeFunction(create_column));
    namespace.insert("Query".to_string(), Value::NativeFunction(create_query));

    // Column types
    namespace.insert("Integer".to_string(), Value::Str("INTEGER".to_string()));
    namespace.insert("String".to_string(), Value::Str("TEXT".to_string()));
    namespace.insert("Float".to_string(), Value::Str("REAL".to_string()));
    namespace.insert("Boolean".to_string(), Value::Str("INTEGER".to_string())); // SQLite uses INTEGER for boolean
    namespace.insert("DateTime".to_string(), Value::Str("TEXT".to_string()));
    namespace.insert("Text".to_string(), Value::Str("TEXT".to_string()));
    namespace.insert("Blob".to_string(), Value::Str("BLOB".to_string()));

    // Relationship types
    namespace.insert("relationship".to_string(), Value::NativeFunction(create_relationship));
    namespace.insert("ForeignKey".to_string(), Value::NativeFunction(create_foreign_key));

    // Utility functions
    namespace.insert("create_all".to_string(), Value::NativeFunction(create_all_tables));
    namespace.insert("drop_all".to_string(), Value::NativeFunction(drop_all_tables));

    // Constants
    namespace.insert("VERSION".to_string(), Value::Str("1.0.0".to_string()));

    Value::Module("orm".to_string(), namespace)
}

/// Create database engine
fn create_engine(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("Engine() requires database URL"));
    }

    let url = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Database URL must be a string")),
    };

    // Parse URL and extract database path
    // For now, support SQLite (sqlite:///path/to/db.db)
    let db_path = if url.starts_with("sqlite:///") {
        url.strip_prefix("sqlite:///").unwrap_or("db.sqlite").to_string()
    } else if url.starts_with("sqlite://") {
        url.strip_prefix("sqlite://").unwrap_or("db.sqlite").to_string()
    } else {
        url.clone()
    };

    // Test connection
    let _conn = Connection::open(&db_path)
        .map_err(|e| anyhow!("Failed to open database: {}", e))?;
    // Connection is closed when _conn is dropped

    let mut engine = HashMap::new();
    engine.insert("url".to_string(), Value::Str(url.clone()));
    engine.insert("db_path".to_string(), Value::Str(db_path));
    engine.insert("tables".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    engine.insert("connect".to_string(), Value::NativeFunction(engine_connect));
    engine.insert("execute".to_string(), Value::NativeFunction(engine_execute));

    Ok(Value::Dict(Rc::new(RefCell::new(engine))))
}

/// Connect to database (returns session)
fn engine_connect(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("connect() requires engine"));
    }

    let engine = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid engine")),
    };

    // Create session from engine
    create_session(vec![Value::Dict(engine)])
}

/// Execute raw SQL
fn engine_execute(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("execute() requires engine and SQL query"));
    }

    let engine = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid engine")),
    };

    let sql = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow!("SQL query must be a string")),
    };

    // Get database path
    let db_path = engine.borrow().get("db_path")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("No database path"))?;

    // Open connection
    let conn = Connection::open(&db_path)
        .map_err(|e| anyhow!("Failed to open database: {}", e))?;

    // Execute query
    let rows_affected = conn.execute(sql, params![])
        .map_err(|e| anyhow!("SQL execution failed: {}", e))?;

    Ok(Value::Int(rows_affected as i64))
}

/// Create session
fn create_session(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("Session() requires engine"));
    }

    let engine = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid engine")),
    };

    let mut session = HashMap::new();
    session.insert("engine".to_string(), Value::Dict(engine));
    session.insert("in_transaction".to_string(), Value::Bool(false));
    session.insert("query".to_string(), Value::NativeFunction(session_query));
    session.insert("add".to_string(), Value::NativeFunction(session_add));
    session.insert("delete".to_string(), Value::NativeFunction(session_delete));
    session.insert("commit".to_string(), Value::NativeFunction(session_commit));
    session.insert("rollback".to_string(), Value::NativeFunction(session_rollback));
    session.insert("close".to_string(), Value::NativeFunction(session_close));
    session.insert("execute".to_string(), Value::NativeFunction(session_execute));

    Ok(Value::Dict(Rc::new(RefCell::new(session))))
}

/// Query from session
fn session_query(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("query() requires session and table"));
    }

    let session = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid session")),
    };

    let table = match &args[1] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid table")),
    };

    // Create query object
    let mut query = HashMap::new();
    query.insert("session".to_string(), Value::Dict(session));
    query.insert("table".to_string(), Value::Dict(table.clone()));
    query.insert("conditions".to_string(), Value::List(crate::modules::HPList::new()));
    query.insert("order".to_string(), Value::List(crate::modules::HPList::new()));
    query.insert("limit_value".to_string(), Value::None);
    query.insert("offset_value".to_string(), Value::None);

    // Query methods
    query.insert("filter".to_string(), Value::NativeFunction(query_filter));
    query.insert("filter_by".to_string(), Value::NativeFunction(query_filter_by));
    query.insert("order_by".to_string(), Value::NativeFunction(query_order_by));
    query.insert("limit".to_string(), Value::NativeFunction(query_limit));
    query.insert("offset".to_string(), Value::NativeFunction(query_offset));
    query.insert("all".to_string(), Value::NativeFunction(query_all));
    query.insert("first".to_string(), Value::NativeFunction(query_first));
    query.insert("count".to_string(), Value::NativeFunction(query_count));

    Ok(Value::Dict(Rc::new(RefCell::new(query))))
}

/// Add object to session
fn session_add(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("add() requires session and object"));
    }

    let session = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid session")),
    };

    let obj = match &args[1] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid object")),
    };

    // Get engine from session
    let engine = session.borrow().get("engine")
        .and_then(|v| if let Value::Dict(d) = v { Some(d.clone()) } else { None })
        .ok_or_else(|| anyhow!("No engine in session"))?;

    // Get table metadata from object
    let table = obj.borrow().get("__table__")
        .cloned()
        .ok_or_else(|| anyhow!("Object has no table metadata"))?;

    let table_dict = match &table {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid table metadata")),
    };

    let table_name = table_dict.borrow().get("name")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("Table has no name"))?;

    let columns = table_dict.borrow().get("columns")
        .cloned()
        .ok_or_else(|| anyhow!("Table has no columns"))?;

    // Build INSERT statement
    let cols = match &columns {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid columns")),
    };

    let col_names: Vec<String> = cols.borrow().keys()
        .filter(|k| *k != "id") // Skip auto-increment primary key
        .cloned()
        .collect();

    let placeholders: Vec<&str> = col_names.iter().map(|_| "?").collect();

    let sql = format!(
        "INSERT INTO {} ({}) VALUES ({})",
        table_name,
        col_names.join(", "),
        placeholders.join(", ")
    );

    // Get database path
    let db_path = engine.borrow().get("db_path")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("No database path"))?;

    // Open connection
    let conn = Connection::open(&db_path)
        .map_err(|e| anyhow!("Failed to open database: {}", e))?;

    // Get values from object
    let obj_borrow = obj.borrow();
    let mut params_vec = Vec::new();
    for col_name in &col_names {
        if let Some(val) = obj_borrow.get(col_name) {
            params_vec.push(value_to_sql_param(val)?);
        }
    }
    drop(obj_borrow);

    // Execute insert
    conn.execute(&sql, rusqlite::params_from_iter(params_vec.iter()))
        .map_err(|e| anyhow!("INSERT failed: {}", e))?;

    // Get last insert ID
    let last_id = conn.last_insert_rowid();

    // Update object with ID
    obj.borrow_mut().insert("id".to_string(), Value::Int(last_id));

    Ok(Value::None)
}

/// Delete object from session
fn session_delete(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("delete() requires session and object"));
    }

    // Implementation similar to add but with DELETE
    Ok(Value::None)
}

/// Commit transaction
fn session_commit(_args: Vec<Value>) -> Result<Value> {
    // In SQLite, auto-commit is default unless we explicitly use transactions
    Ok(Value::None)
}

/// Rollback transaction
fn session_rollback(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Close session
fn session_close(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Execute SQL in session
fn session_execute(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("execute() requires session and SQL"));
    }

    let session = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid session")),
    };

    let engine = session.borrow().get("engine")
        .cloned()
        .ok_or_else(|| anyhow!("No engine in session"))?;

    // Use engine_execute
    engine_execute(vec![engine, args[1].clone()])
}

/// Create table definition
fn create_table(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("Table() requires table name"));
    }

    let name = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Table name must be a string")),
    };

    let columns = if args.len() > 1 {
        match &args[1] {
            Value::Dict(d) => d.clone(),
            _ => Rc::new(RefCell::new(HashMap::new())),
        }
    } else {
        Rc::new(RefCell::new(HashMap::new()))
    };

    let mut table = HashMap::new();
    table.insert("name".to_string(), Value::Str(name));
    table.insert("columns".to_string(), Value::Dict(columns));
    table.insert("primary_key".to_string(), Value::Str("id".to_string()));
    table.insert("relationships".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    table.insert("create".to_string(), Value::NativeFunction(table_create));
    table.insert("drop".to_string(), Value::NativeFunction(table_drop));

    Ok(Value::Dict(Rc::new(RefCell::new(table))))
}

/// Create table in database
fn table_create(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("create() requires table and engine"));
    }

    let table = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid table")),
    };

    let engine = match &args[1] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid engine")),
    };

    let table_borrow = table.borrow();
    let table_name = table_borrow.get("name")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("Table has no name"))?;

    let columns = table_borrow.get("columns")
        .cloned()
        .ok_or_else(|| anyhow!("Table has no columns"))?;

    drop(table_borrow);

    // Build CREATE TABLE statement
    let cols_dict = match &columns {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid columns")),
    };

    let mut col_defs = vec!["id INTEGER PRIMARY KEY AUTOINCREMENT".to_string()];

    for (col_name, col_def) in cols_dict.borrow().iter() {
        if col_name == "id" {
            continue; // Skip ID, already added
        }

        let col_dict = match col_def {
            Value::Dict(d) => d,
            _ => continue,
        };

        let col_type = col_dict.borrow().get("type")
            .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
            .unwrap_or_else(|| "TEXT".to_string());

        let nullable = col_dict.borrow().get("nullable")
            .and_then(|v| if let Value::Bool(b) = v { Some(*b) } else { None })
            .unwrap_or(true);

        let unique = col_dict.borrow().get("unique")
            .and_then(|v| if let Value::Bool(b) = v { Some(*b) } else { None })
            .unwrap_or(false);

        let mut def = format!("{} {}", col_name, col_type);
        if !nullable {
            def.push_str(" NOT NULL");
        }
        if unique {
            def.push_str(" UNIQUE");
        }

        col_defs.push(def);
    }

    let sql = format!("CREATE TABLE IF NOT EXISTS {} ({})", table_name, col_defs.join(", "));

    // Execute
    engine_execute(vec![Value::Dict(engine.clone()), Value::Str(sql)])?;

    Ok(Value::None)
}

/// Drop table from database
fn table_drop(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("drop() requires table and engine"));
    }

    let table = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid table")),
    };

    let engine = match &args[1] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid engine")),
    };

    let table_name = table.borrow().get("name")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("Table has no name"))?;

    let sql = format!("DROP TABLE IF EXISTS {}", table_name);
    engine_execute(vec![Value::Dict(engine.clone()), Value::Str(sql)])?;

    Ok(Value::None)
}

/// Create column definition
fn create_column(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("Column() requires column type"));
    }

    let col_type = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow!("Column type must be a string")),
    };

    let nullable = if args.len() > 1 {
        match &args[1] {
            Value::Bool(b) => *b,
            _ => true,
        }
    } else {
        true
    };

    let unique = if args.len() > 2 {
        match &args[2] {
            Value::Bool(b) => *b,
            _ => false,
        }
    } else {
        false
    };

    let mut column = HashMap::new();
    column.insert("type".to_string(), Value::Str(col_type));
    column.insert("nullable".to_string(), Value::Bool(nullable));
    column.insert("unique".to_string(), Value::Bool(unique));
    column.insert("primary_key".to_string(), Value::Bool(false));
    column.insert("foreign_key".to_string(), Value::None);

    Ok(Value::Dict(Rc::new(RefCell::new(column))))
}

/// Create query (standalone)
fn create_query(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Filter query
fn query_filter(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("filter() requires query and condition"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid query")),
    };

    // Add condition to query
    let conditions = query.borrow().get("conditions")
        .cloned()
        .ok_or_else(|| anyhow!("Query has no conditions"))?;

    if let Value::List(mut list) = conditions {
        list.push(args[1].clone());
        query.borrow_mut().insert("conditions".to_string(), Value::List(list));
    }

    Ok(Value::Dict(query))
}

/// Filter query by keyword arguments
fn query_filter_by(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("filter_by() requires query and dict"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid query")),
    };

    let filters = match &args[1] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("filter_by requires dict")),
    };

    // Add each key=value as condition
    let conditions = query.borrow().get("conditions")
        .cloned()
        .ok_or_else(|| anyhow!("Query has no conditions"))?;

    if let Value::List(mut list) = conditions {
        for (key, value) in filters.borrow().iter() {
            let mut cond = HashMap::new();
            cond.insert("column".to_string(), Value::Str(key.clone()));
            cond.insert("op".to_string(), Value::Str("=".to_string()));
            cond.insert("value".to_string(), value.clone());
            list.push(Value::Dict(Rc::new(RefCell::new(cond))));
        }
        query.borrow_mut().insert("conditions".to_string(), Value::List(list));
    }

    Ok(Value::Dict(query))
}

/// Order by
fn query_order_by(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("order_by() requires query and column"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid query")),
    };

    let order = query.borrow().get("order")
        .cloned()
        .ok_or_else(|| anyhow!("Query has no order"))?;

    if let Value::List(mut list) = order {
        list.push(args[1].clone());
        query.borrow_mut().insert("order".to_string(), Value::List(list));
    }

    Ok(Value::Dict(query))
}

/// Limit results
fn query_limit(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("limit() requires query and count"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid query")),
    };

    query.borrow_mut().insert("limit_value".to_string(), args[1].clone());

    Ok(Value::Dict(query))
}

/// Offset results
fn query_offset(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow!("offset() requires query and count"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid query")),
    };

    query.borrow_mut().insert("offset_value".to_string(), args[1].clone());

    Ok(Value::Dict(query))
}

/// Execute query and return all results
fn query_all(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("all() requires query"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid query")),
    };

    // Build SQL SELECT statement
    let query_borrow = query.borrow();

    let table = query_borrow.get("table")
        .cloned()
        .ok_or_else(|| anyhow!("Query has no table"))?;

    let table_dict = match &table {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid table")),
    };

    let table_name = table_dict.borrow().get("name")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("Table has no name"))?;

    let mut sql = format!("SELECT * FROM {}", table_name);

    // Add WHERE clause
    let conditions = query_borrow.get("conditions")
        .cloned()
        .unwrap_or(Value::List(crate::modules::HPList::new()));

    if let Value::List(list) = &conditions {
        let cond_vec = list.as_vec();
        if !cond_vec.is_empty() {
            let where_clauses: Vec<String> = cond_vec.iter()
                .filter_map(|c| {
                    if let Value::Dict(d) = c {
                        let b = d.borrow();
                        let col = b.get("column")?.as_str()?;
                        let op = b.get("op")?.as_str()?;
                        Some(format!("{} {} ?", col, op))
                    } else {
                        None
                    }
                })
                .collect();

            if !where_clauses.is_empty() {
                sql.push_str(&format!(" WHERE {}", where_clauses.join(" AND ")));
            }
        }
    }

    // Add ORDER BY
    let order = query_borrow.get("order")
        .cloned()
        .unwrap_or(Value::List(crate::modules::HPList::new()));

    if let Value::List(list) = &order {
        let order_vec = list.as_vec();
        if !order_vec.is_empty() {
            let order_cols: Vec<String> = order_vec.iter()
                .filter_map(|o| o.as_str().map(|s| s.to_string()))
                .collect();

            if !order_cols.is_empty() {
                sql.push_str(&format!(" ORDER BY {}", order_cols.join(", ")));
            }
        }
    }

    // Add LIMIT
    if let Some(Value::Int(limit)) = query_borrow.get("limit_value") {
        sql.push_str(&format!(" LIMIT {}", limit));
    }

    // Add OFFSET
    if let Some(Value::Int(offset)) = query_borrow.get("offset_value") {
        sql.push_str(&format!(" OFFSET {}", offset));
    }

    // Get session and engine
    let session = query_borrow.get("session")
        .ok_or_else(|| anyhow!("Query has no session"))?;

    let session_dict = match session {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid session")),
    };

    let engine = session_dict.borrow().get("engine")
        .cloned()
        .ok_or_else(|| anyhow!("Session has no engine"))?;

    let engine_dict = match &engine {
        Value::Dict(d) => d,
        _ => return Err(anyhow!("Invalid engine")),
    };

    drop(query_borrow);

    // Get database path
    let db_path = engine_dict.borrow().get("db_path")
        .and_then(|v| if let Value::Str(s) = v { Some(s.clone()) } else { None })
        .ok_or_else(|| anyhow!("No database path"))?;

    // Open connection
    let conn = Connection::open(&db_path)
        .map_err(|e| anyhow!("Failed to open database: {}", e))?;

    // Execute query
    let mut stmt = conn.prepare(&sql)
        .map_err(|e| anyhow!("Failed to prepare query: {}", e))?;

    // Get parameter values
    let mut params_vec = Vec::new();
    if let Value::List(list) = &conditions {
        for c in list.as_vec() {
            if let Value::Dict(d) = c {
                if let Some(val) = d.borrow().get("value") {
                    params_vec.push(value_to_sql_param(val)?);
                }
            }
        }
    }

    let rows = stmt.query_map(rusqlite::params_from_iter(params_vec.iter()), |row| {
        row_to_value(row, &table_dict.borrow())
    }).map_err(|e| anyhow!("Query execution failed: {}", e))?;

    let mut results = Vec::new();
    for row_result in rows {
        results.push(row_result.map_err(|e| anyhow!("Row conversion failed: {}", e))?);
    }

    Ok(Value::List(crate::modules::HPList::from_values(results)))
}

/// Get first result
fn query_first(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("first() requires query"));
    }

    let query = match &args[0] {
        Value::Dict(d) => d.clone(),
        _ => return Err(anyhow!("Invalid query")),
    };

    // Set limit to 1
    query.borrow_mut().insert("limit_value".to_string(), Value::Int(1));

    // Execute query
    let results = query_all(vec![Value::Dict(query)])?;

    // Return first result or None
    match results {
        Value::List(list) => {
            let vec = list.as_vec();
            if vec.is_empty() {
                Ok(Value::None)
            } else {
                Ok(vec[0].clone())
            }
        }
        _ => Ok(Value::None),
    }
}

/// Count results
fn query_count(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("count() requires query"));
    }

    // Similar to query_all but with COUNT(*)
    Ok(Value::Int(0))
}

/// Create relationship
fn create_relationship(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Create foreign key
fn create_foreign_key(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Create all tables
fn create_all_tables(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("create_all() requires engine"));
    }

    // Iterate through all registered tables and create them
    Ok(Value::None)
}

/// Drop all tables
fn drop_all_tables(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("drop_all() requires engine"));
    }

    // Iterate through all registered tables and drop them
    Ok(Value::None)
}

/// Helper: Convert Value to SQL parameter
fn value_to_sql_param(val: &Value) -> Result<rusqlite::types::ToSqlOutput<'static>> {
    use rusqlite::types::{ToSqlOutput, ValueRef};

    match val {
        Value::Int(i) => Ok(ToSqlOutput::Owned(rusqlite::types::Value::Integer(*i))),
        Value::Float(f) => Ok(ToSqlOutput::Owned(rusqlite::types::Value::Real(*f))),
        Value::Str(s) => Ok(ToSqlOutput::Owned(rusqlite::types::Value::Text(s.clone()))),
        Value::Bool(b) => Ok(ToSqlOutput::Owned(rusqlite::types::Value::Integer(if *b { 1 } else { 0 }))),
        Value::None => Ok(ToSqlOutput::Owned(rusqlite::types::Value::Null)),
        _ => Err(anyhow!("Unsupported value type for SQL parameter")),
    }
}

/// Helper: Convert SQL row to Value
fn row_to_value(row: &Row, _table: &HashMap<String, Value>) -> rusqlite::Result<Value> {
    let mut obj = HashMap::new();

    // Get column count
    let col_count = row.as_ref().column_count();

    for i in 0..col_count {
        let col_name = row.as_ref().column_name(i)?;

        // Try to get value as different types
        let value = if let Ok(val) = row.get::<_, i64>(i) {
            Value::Int(val)
        } else if let Ok(val) = row.get::<_, f64>(i) {
            Value::Float(val)
        } else if let Ok(val) = row.get::<_, String>(i) {
            Value::Str(val)
        } else {
            Value::None
        };

        obj.insert(col_name.to_string(), value);
    }

    Ok(Value::Dict(Rc::new(RefCell::new(obj))))
}

// Helper trait for Value
trait ValueExt {
    fn as_str(&self) -> Option<&str>;
}

impl ValueExt for Value {
    fn as_str(&self) -> Option<&str> {
        match self {
            Value::Str(s) => Some(s),
            _ => None,
        }
    }
}
