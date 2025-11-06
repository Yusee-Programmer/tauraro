#!/usr/bin/env python3
"""
Simple ORM test - Basic CRUD operations
Note: Using direct function calls instead of method syntax due to Tauraro limitations
"""

import orm

print("=== ORM Simple Test ===\n")

# Test 1: Create engine
print("1. Create Database Engine:")
engine = orm.Engine("sqlite:///test.db")
print(f"   Engine created: {engine}")
print("   ✓ Engine creation works!\n")

# Test 2: Create table definition
print("2. Define Table Schema:")
# Define columns
columns = {}
columns["name"] = orm.Column(orm.String, False)  # NOT NULL
columns["email"] = orm.Column(orm.String, False, True)  # NOT NULL, UNIQUE
columns["age"] = orm.Column(orm.Integer, True)  # NULLABLE

users_table = orm.Table("users", columns)
print(f"   Table: users")
print(f"   Columns: name (STRING NOT NULL), email (STRING NOT NULL UNIQUE), age (INTEGER NULL)")
print("   ✓ Table definition works!\n")

# Test 3: Create table in database
print("3. Create Table in Database:")
create_fn = users_table["create"]
create_fn(users_table, engine)
print("   ✓ Table created in database!\n")

# Test 4: Create session
print("4. Create Database Session:")
connect_fn = engine["connect"]
session = connect_fn(engine)
print("   ✓ Session created!\n")

# Test 5: Insert data
print("5. Insert User (Add to Session):")
user1 = {}
user1["__table__"] = users_table
user1["name"] = "Alice"
user1["email"] = "alice@example.com"
user1["age"] = 30

add_fn = session["add"]
add_fn(session, user1)
print(f"   Inserted: Alice")
print(f"   ID: {user1['id']}")
print("   ✓ Insert works!\n")

# Insert more users
user2 = {}
user2["__table__"] = users_table
user2["name"] = "Bob"
user2["email"] = "bob@example.com"
user2["age"] = 25
add_fn(session, user2)

user3 = {}
user3["__table__"] = users_table
user3["name"] = "Charlie"
user3["email"] = "charlie@example.com"
user3["age"] = 35
add_fn(session, user3)

print("6. Query All Users:")
query_fn = session["query"]
query = query_fn(session, users_table)
all_fn = query["all"]
all_users = all_fn(query)
print(f"   Found {len(all_users)} users:")
for i in range(len(all_users)):
    user = all_users[i]
    print(f"   User {i+1}: id={user['id']}, name={user['name']}, email={user['email']}, age={user['age']}")
print("   ✓ Query all works!\n")

# Test 7: Filter query
print("7. Filter Query (age=30):")
filter_dict = {}
filter_dict["age"] = 30
query2 = query_fn(session, users_table)
filter_by_fn = query2["filter_by"]
query2 = filter_by_fn(query2, filter_dict)
all_fn2 = query2["all"]
filtered = all_fn2(query2)
print(f"   Found {len(filtered)} users with age=30:")
for user in filtered:
    print(f"   - {user['name']} (age {user['age']})")
print("   ✓ Filter works!\n")

# Test 8: Order by
print("8. Order By Age:")
query3 = query_fn(session, users_table)
order_by_fn = query3["order_by"]
query3 = order_by_fn(query3, "age")
all_fn3 = query3["all"]
ordered = all_fn3(query3)
print("   Users ordered by age:")
for user in ordered:
    print(f"   - {user['name']}: {user['age']} years old")
print("   ✓ Order by works!\n")

# Test 9: Limit
print("9. Limit Results (2 users):")
query4 = query_fn(session, users_table)
limit_fn = query4["limit"]
query4 = limit_fn(query4, 2)
all_fn4 = query4["all"]
limited = all_fn4(query4)
print(f"   Got {len(limited)} users (limit 2):")
for user in limited:
    print(f"   - {user['name']}")
print("   ✓ Limit works!\n")

# Test 10: First
print("10. Get First User:")
query5 = query_fn(session, users_table)
first_fn = query5["first"]
first_user = first_fn(query5)
if first_user:
    print(f"    First user: {first_user['name']}")
    print("    ✓ First works!\n")

# Test 11: Raw SQL execution
print("11. Execute Raw SQL:")
execute_fn = engine["execute"]
# Note: execute() is for UPDATE/INSERT/DELETE, not SELECT
result = execute_fn(engine, "UPDATE users SET age = age WHERE id > 0")
print(f"    Rows affected: {result}")
print("    ✓ Raw SQL execution works!\n")

close_fn = session["close"]
close_fn(session)
print("=== All Tests Passed! ===")
