import serveit

def app(scope):
    # Test that scope parameter works - just print it
    print(f"Scope received: {scope}")
    print(f"Scope type: {type(scope)}")

    # Try to access path
    try:
        if "path" in scope:
            path = scope["path"]
            print(f"Path: {path}")

            if path == "/":
                html = "<h1 style='color: blue;'>Root Path Works!</h1><p>Scope parameter is accessible</p>"
                return serveit.HTMLResponse(html)
            elif path == "/test":
                return serveit.HTMLResponse("<h1 style='color: green;'>Test Path Works!</h1>")
    except Exception as e:
        print(f"Error accessing scope: {e}")

    # Default response
    return serveit.HTMLResponse("<h1>Default Response</h1>")

serveit.run(app, "127.0.0.1", 8000)
