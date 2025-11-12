"""
Simple WebViewTK Window Test
Opens a window with HTML content
"""

import webviewtk as wv

print("Creating window with WebViewTK...")

# Build a simple HTML page
html_content = wv.render(
    wv.cdn_tailwind(),
    wv.div(
        wv.render(
            wv.h1("Hello from Tauraro!", "text-5xl font-bold text-center text-blue-600 mb-8"),
            wv.p("This is a cross-platform GUI application!", "text-xl text-center text-gray-700 mb-4"),
            wv.p("Built with WebViewTK framework", "text-center text-gray-600"),
            wv.div(
                wv.button("Button 1", "bg-blue-500 hover:bg-blue-700 text-white font-bold py-3 px-6 rounded mr-4"),
                "text-center mt-8"
            )
        ),
        "container mx-auto p-12 flex flex-col justify-center h-screen"
    )
)

# Build complete HTML document
full_html = "<!DOCTYPE html>" + wv.html(
    wv.render(
        wv.head(
            wv.render(
                wv.title("Tauraro App"),
                wv.meta({"charset": "utf-8"}),
                wv.meta({"name": "viewport", "content": "width=device-width, initial-scale=1"}),
                html_content
            )
        ),
        wv.body(
            html_content,
            "bg-gradient-to-br from-blue-50 to-purple-50"
        )
    )
)

print("HTML generated successfully")
print(f"HTML length: {len(full_html)} characters")

# Create window
window = wv.Window("Tauraro WebViewTK Demo", 1024, 768)
print("Window object created")

# Set HTML content
window.set_html(full_html)
print("HTML content set")

print("\nOpening window... (Close the window to exit)")

# Run the window (this will block until window is closed)
window.run()

print("Window closed")
