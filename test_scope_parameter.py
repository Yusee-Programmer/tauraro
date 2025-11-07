import serveit

def app(scope):
    # Test that scope parameter is accessible
    print(f"Scope type: {type(scope)}")
    print(f"Scope is dict: {isinstance(scope, dict)}")

    # Access scope directly (not using .get())
    if scope and "path" in scope:
        path = scope["path"]
        print(f"Path from scope: {path}")

        if path == "/":
            return serveit.HTMLResponse("<h1 style='color: blue;'>Hello from Serveit!</h1>")
        elif path == "/test":
            return serveit.HTMLResponse("<h1 style='color: green;'>Test page works!</h1>")

    # Default response
    return {
        "status": 200,
        "body": "Default response",
        "headers": {
            "content-type": "text/html; charset=utf-8"
        }
    }

serveit.run(app, "127.0.0.1", 8000)
