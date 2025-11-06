#!/usr/bin/env python3
"""
Test application for ServEit ASGI server
A simple web application to demonstrate Tauraro's web server capabilities
"""

import serveit

# Simple app that returns a response
def app(scope):
    """
    Simple ASGI application
    """
    method = scope.get("method", "GET")
    path = scope.get("path", "/")

    print(f"Request: {method} {path}")

    # Create response based on path
    if path == "/":
        return serveit.HTMLResponse("""
        <!DOCTYPE html>
        <html>
        <head>
            <title>ServEit Test App</title>
            <style>
                body { font-family: Arial, sans-serif; margin: 40px; }
                h1 { color: #333; }
                .info { background: #f0f0f0; padding: 15px; border-radius: 5px; }
                code { background: #e0e0e0; padding: 2px 5px; border-radius: 3px; }
            </style>
        </head>
        <body>
            <h1>🚀 ServEit - Tauraro ASGI Server</h1>
            <p>Welcome to ServEit, a high-performance ASGI server for Tauraro!</p>

            <div class="info">
                <h2>Try these endpoints:</h2>
                <ul>
                    <li><a href="/hello">GET /hello</a> - Simple greeting</li>
                    <li><a href="/json">GET /json</a> - JSON response</li>
                    <li><a href="/redirect">GET /redirect</a> - Redirect test</li>
                    <li><a href="/status/404">GET /status/404</a> - Custom status code</li>
                </ul>
            </div>

            <h3>Server Info:</h3>
            <p>
                <strong>Method:</strong> <code>""" + method + """</code><br>
                <strong>Path:</strong> <code>""" + path + """</code><br>
                <strong>Powered by:</strong> Tauraro + Rust (Tokio + Hyper)
            </p>
        </body>
        </html>
        """)

    elif path == "/hello":
        return serveit.HTMLResponse("<h1>Hello from ServEit!</h1><p>Tauraro is awesome! 🎉</p>")

    elif path == "/json":
        data = {
            "message": "Hello from ServEit",
            "server": "Tauraro ASGI",
            "status": "running",
            "features": ["Fast", "Pythonic", "Powered by Rust"]
        }
        return serveit.JSONResponse(data)

    elif path == "/redirect":
        return serveit.RedirectResponse("/hello")

    elif path.startswith("/status/"):
        # Extract status code
        parts = path.split("/")
        if len(parts) >= 3:
            try:
                status_code = int(parts[2])
                return serveit.Response(status_code, f"Status code: {status_code}")
            except:
                pass

    # 404 Not Found
    return serveit.HTMLResponse("""
    <h1>404 Not Found</h1>
    <p>The page you're looking for doesn't exist.</p>
    <p><a href="/">Go back home</a></p>
    """, 404)

# Run the server
if __name__ == "__main__":
    print("Starting ServEit test application...")
    print()

    # Run with default options (host=127.0.0.1, port=8000)
    serveit.run(app)

    # Or run with custom options:
    # serveit.run(app, "0.0.0.0", 8080, {"log_level": "debug", "reload": False, "workers": 1})
