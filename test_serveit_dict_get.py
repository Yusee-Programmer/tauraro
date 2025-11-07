import serveit

def app(scope):
    # Now we can use dict.get() directly without workaround!
    path = scope.get("path", "/")

    if path == "/":
        return serveit.HTMLResponse("<h1 style='color: blue;'>Root Path!</h1>")
    elif path == "/test":
        return serveit.HTMLResponse("<h1 style='color: green;'>Test Path!</h1>")
    elif path == "/about":
        return serveit.HTMLResponse("<h1 style='color: purple;'>About Page!</h1>")

    return serveit.HTMLResponse(f"<h1>404 - Path '{path}' not found</h1>")

print("Testing serveit with direct dict.get() calls...")
serveit.run(app, "127.0.0.1", 8000)
