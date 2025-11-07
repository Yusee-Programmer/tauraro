"""
Standard Python HTTP server benchmark
For comparison with serveit
"""
from http.server import HTTPServer, BaseHTTPRequestHandler
import json

class BenchmarkHandler(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        """Suppress log messages for cleaner benchmark output"""
        pass

    def do_GET(self):
        if self.path == "/":
            self.send_response(200)
            self.send_header("Content-Type", "text/html")
            self.end_headers()
            self.wfile.write(b"<h1>Python HTTP Root</h1>")

        elif self.path == "/api/hello":
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            response = json.dumps({"message": "Hello, World!"})
            self.wfile.write(response.encode())

        elif self.path.startswith("/api/user/"):
            parts = self.path.split("/")
            if len(parts) >= 4:
                try:
                    user_id = int(parts[3])
                    self.send_response(200)
                    self.send_header("Content-Type", "application/json")
                    self.end_headers()
                    response = json.dumps({
                        "id": user_id,
                        "name": f"User {user_id}",
                        "email": f"user{user_id}@example.com"
                    })
                    self.wfile.write(response.encode())
                    return
                except:
                    pass

            self.send_response(400)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(b'{"error": "Invalid user ID"}')

        elif self.path == "/api/data":
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            items = [{"id": i, "value": f"Item {i}"} for i in range(100)]
            response = json.dumps({"items": items, "total": 100})
            self.wfile.write(response.encode())

        elif self.path == "/html/page":
            self.send_response(200)
            self.send_header("Content-Type", "text/html")
            self.end_headers()
            html = b"""<!DOCTYPE html>
<html>
<head>
    <title>Test Page</title>
    <style>
        body { font-family: Arial; padding: 20px; }
        h1 { color: blue; }
    </style>
</head>
<body>
    <h1>Python HTTP Test Page</h1>
    <p>This is a test page for benchmarking.</p>
</body>
</html>"""
            self.wfile.write(html)

        else:
            self.send_response(404)
            self.send_header("Content-Type", "text/html")
            self.end_headers()
            self.wfile.write(b"<h1>404 - Not Found</h1>")

if __name__ == "__main__":
    server = HTTPServer(("127.0.0.1", 8000), BenchmarkHandler)
    print("Starting Python HTTP server on http://127.0.0.1:8000")
    server.serve_forever()
