import serveit

def app(scope):
    path = scope.get("path", "/")

    if path == "/":
        # Full HTML document
        html = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Serveit HTML Test</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            background-color: #f0f0f0;
        }
        .header {
            background-color: #4CAF50;
            color: white;
            padding: 20px;
            border-radius: 5px;
        }
        .content {
            background-color: white;
            padding: 20px;
            margin-top: 20px;
            border-radius: 5px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        .styled-text {
            color: blue;
            font-size: 24px;
            font-weight: bold;
        }
        ul {
            list-style-type: square;
        }
        li {
            margin: 10px 0;
        }
    </style>
</head>
<body>
    <div class="header">
        <h1>Welcome to Serveit!</h1>
        <p>High-Performance ASGI Server for Tauraro</p>
    </div>

    <div class="content">
        <h2 class="styled-text">HTML Rendering Test</h2>
        <p style="color: red;">This paragraph should be <strong>red</strong>.</p>
        <p style="color: green;">This paragraph should be <em>green</em>.</p>

        <h3>Features:</h3>
        <ul>
            <li>Proper HTML rendering</li>
            <li>CSS styling support</li>
            <li>Inline and external styles</li>
            <li>UTF-8 character encoding</li>
        </ul>

        <h3>Special Characters:</h3>
        <p>Testing special chars: &lt; &gt; &amp; &quot; &#39;</p>
        <p>Unicode: 你好 مرحبا Здравствуй</p>

        <h3>Links:</h3>
        <a href="/simple" style="color: blue; text-decoration: underline;">Visit Simple Page</a> |
        <a href="/json" style="color: blue; text-decoration: underline;">Visit JSON API</a>
    </div>
</body>
</html>"""
        return serveit.HTMLResponse(html)

    elif path == "/simple":
        # Simple inline HTML
        return serveit.HTMLResponse("<h1 style=\"color: blue;\">Hello from Serveit!</h1><p style=\"color: red;\">This is a test.</p>")

    elif path == "/json":
        return serveit.JSONResponse({
            "message": "HTML rendering is working!",
            "status": "success",
            "features": ["HTML", "CSS", "UTF-8"]
        })

    else:
        return serveit.Response(404, "Page Not Found")

print("Starting Serveit server...")
print("Visit http://127.0.0.1:8000 to test HTML rendering")
print("Visit http://127.0.0.1:8000/simple for simple HTML test")
print("Visit http://127.0.0.1:8000/json for JSON API test")
print()

serveit.run(app, "127.0.0.1", 8000)
