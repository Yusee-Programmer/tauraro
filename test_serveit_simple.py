import serveit

# Simple application
def app(scope):
    method = scope.get("method", "GET")
    path = scope.get("path", "/")

    if path == "/":
        return serveit.HTMLResponse("<h1 style=\"color: blue;\">Hello from Serveit!</h1>")
    elif path == "/json":
        return serveit.JSONResponse({"message": "Hello", "status": "ok"})
    elif path == "/redirect":
        return serveit.RedirectResponse("/")
    else:
        return serveit.Response(404, "Not Found")

# Run server
serveit.run(app, host="127.0.0.1", port=8000)
