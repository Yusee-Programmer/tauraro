"""
Serveit benchmark application
Simple routes for performance testing - with dynamic routing
"""
import serveit

def app(scope):
    path = scope.get("path", "/")

    if path == "/":
        return serveit.HTMLResponse("<h1>Serveit Root</h1>")
    elif path == "/api/hello":
        return serveit.JSONResponse({"message": "Hello, World!"})
    elif path.startswith("/api/user/"):
        # Extract user_id from path using split()
        parts = path.split("/")
        if len(parts) >= 4:
            try:
                user_id = int(parts[3])
                return serveit.JSONResponse({
                    "id": user_id,
                    "name": f"User {user_id}",
                    "email": f"user{user_id}@example.com"
                })
            except:
                pass
        return serveit.JSONResponse({"error": "Invalid user ID"}, status_code=400)
    elif path == "/api/data":
        items = []
        for i in range(100):
            items.append({"id": i, "value": f"Item {i}"})
        return serveit.JSONResponse({"items": items, "total": 100})
    elif path == "/html/page":
        html = """<!DOCTYPE html>
<html>
<head>
    <title>Test Page</title>
    <style>
        body { font-family: Arial; padding: 20px; }
        h1 { color: blue; }
    </style>
</head>
<body>
    <h1>Serveit Test Page</h1>
    <p>This is a test page for benchmarking.</p>
</body>
</html>"""
        return serveit.HTMLResponse(html)

    return serveit.HTMLResponse("<h1>404 - Not Found</h1>", status_code=404)

print("Starting Serveit benchmark server on http://127.0.0.1:8001")
serveit.run(app, "127.0.0.1", 8001)
