import serveit

def app(scope):
    # Workaround: Store method before calling
    scope_get = scope.get
    path = scope_get("path", "/")

    if path == "/":
        return serveit.HTMLResponse("<h1 style='color: blue;'>Root Works!</h1>")
    elif path == "/test":
        return serveit.HTMLResponse("<h1 style='color: green;'>Test Works!</h1>")

    return serveit.HTMLResponse("<h1>Default</h1>")

print("Testing with dict.get() workaround...")
serveit.run(app, "127.0.0.1", 8001)
