import serveit

def app(scope):
    path = scope.get("path", "/")

    # Print scope for debugging
    print(f"Request path: {path}")

    if path == "/html":
        response = serveit.HTMLResponse("<h1 style=\"color: blue;\">Blue Header</h1>")
        print("Created HTML response")
        return response
    elif path == "/json":
        response = serveit.JSONResponse({"test": "value"})
        print("Created JSON response")
        return response
    elif path == "/text":
        response = serveit.Response(200, "Plain text")
        print("Created text response")
        return response
    else:
        return serveit.HTMLResponse("""
<!DOCTYPE html>
<html>
<head><title>Test Page</title></head>
<body>
<h1 style="color: blue; font-size: 32px;">Styled Header</h1>
<p style="color: red; background-color: yellow; padding: 10px;">Styled Paragraph</p>
<div style="border: 2px solid green; padding: 20px; margin: 10px;">
    <span style="font-weight: bold; color: purple;">Bold Purple Text</span>
</div>
</body>
</html>
""")

serveit.run(app, "127.0.0.1", 8000)
