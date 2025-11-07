import serveit

def app(scope):
    path = scope.get("path", "/")

    if path == "/":
        html = """<!DOCTYPE html>
<html>
<head>
    <title>Serveit Test</title>
</head>
<body>
    <h1 style="color: blue;">Hello from Serveit!</h1>
    <p style="color: red;">This is a test paragraph.</p>
    <ul>
        <li>Item 1</li>
        <li>Item 2</li>
    </ul>
</body>
</html>"""
        return serveit.HTMLResponse(html)
    elif path == "/simple":
        return serveit.HTMLResponse("<h1 style=\"color: blue;\">Hello from Serveit!</h1>")
    else:
        return serveit.Response(404, "Not Found")

serveit.run(app, "127.0.0.1", 8000)
