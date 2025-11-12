import webviewtk as wv

print("Starting WebViewTK test...")

# Create simple HTML
html_content = """<!DOCTYPE html>
<html>
<head>
    <title>WebViewTK Test</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
        }
        .container {
            text-align: center;
            background: white;
            padding: 50px;
            border-radius: 20px;
            box-shadow: 0 10px 50px rgba(0,0,0,0.3);
        }
        h1 {
            color: #667eea;
            font-size: 48px;
            margin: 0;
        }
        p {
            color: #666;
            font-size: 20px;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Hello from Tauraro!</h1>
        <p>WebViewTK is working! 🎉</p>
    </div>
</body>
</html>"""

print("Creating window...")
window = wv.Window("Tauraro WebViewTK Test", 800, 600)

print("Setting HTML...")
window.set_html(html_content)

print("Running window...")
print("(The window should appear now - close it to exit)")
window.run()

print("Window closed!")
