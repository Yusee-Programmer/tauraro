"""
Test Multiple Windows Running Simultaneously
This demonstrates that multiple Tauraro WebViewTK windows can run at the same time
"""

import webviewtk as wv
import time

def create_window(title, content, color, position_x, position_y):
    """Create a simple window with custom content"""

    tailwind = wv.cdn_tailwind()

    # Create content with the specified color
    app_content = wv.div(
        wv.render(
            wv.h1(title, f"text-5xl font-bold text-center {color} mb-6"),
            wv.p(
                content,
                "text-xl text-center text-gray-700 mb-8"
            ),
            wv.div(
                wv.render(
                    wv.p(f"Window Position: ({position_x}, {position_y})", "text-sm text-gray-500"),
                    wv.p("This window runs independently!", "text-sm text-gray-500 mt-2")
                ),
                "text-center"
            )
        ),
        "container mx-auto p-8 flex flex-col justify-center h-screen"
    )

    head_content = wv.render(
        wv.title(title),
        wv.meta({"charset": "utf-8"}),
        wv.meta({"name": "viewport", "content": "width=device-width, initial-scale=1"}),
        tailwind
    )

    full_html = "<!DOCTYPE html>" + wv.html(
        wv.render(
            wv.head(head_content),
            wv.body(app_content, "bg-gray-50")
        )
    )

    return full_html

print("=" * 70)
print("  Testing Multiple Windows - Each window runs in its own thread!")
print("=" * 70)
print()

# Create Window 1
print("Creating Window 1 (Blue)...")
window1 = wv.Window("Window 1 - Blue", 600, 400)
html1 = create_window(
    "Window 1",
    "I'm the BLUE window! I can run alongside other windows.",
    "text-blue-600",
    100, 100
)
window1.set_html(html1)
window1.run_async()  # Use run_async() for multiple windows in one program
print("✓ Window 1 started!")
time.sleep(0.2)

# Create Window 2
print("Creating Window 2 (Green)...")
window2 = wv.Window("Window 2 - Green", 600, 400)
html2 = create_window(
    "Window 2",
    "I'm the GREEN window! We're running simultaneously.",
    "text-green-600",
    200, 150
)
window2.set_html(html2)
window2.run_async()  # Use run_async() for multiple windows in one program
print("✓ Window 2 started!")
time.sleep(0.2)

# Create Window 3
print("Creating Window 3 (Purple)...")
window3 = wv.Window("Window 3 - Purple", 600, 400)
html3 = create_window(
    "Window 3",
    "I'm the PURPLE window! All three of us are independent.",
    "text-purple-600",
    300, 200
)
window3.set_html(html3)
window3.run_async()  # Use run_async() for multiple windows in one program
print("✓ Window 3 started!")
time.sleep(0.2)

# Create Window 4
print("Creating Window 4 (Red)...")
window4 = wv.Window("Window 4 - Red", 600, 400)
html4 = create_window(
    "Window 4",
    "I'm the RED window! Four windows at once - amazing!",
    "text-red-600",
    400, 250
)
window4.set_html(html4)
window4.run_async()  # Use run_async() for multiple windows in one program
print("✓ Window 4 started!")
time.sleep(0.2)

print()
print("=" * 70)
print("  SUCCESS! All 4 windows are now running simultaneously!")
print("=" * 70)
print()
print("Instructions:")
print("  • All windows are independent and can be moved/resized separately")
print("  • Close windows individually - others will keep running")
print("  • Each window runs in its own thread")
print("  • You can now run multiple Tauraro programs at the same time!")
print()
print("Keep this terminal open to keep all windows alive.")
print("Press Ctrl+C to close all windows and exit.")
print()

# Keep the program alive while windows are open
try:
    # Sleep indefinitely - windows will stay open in their threads
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\nClosing all windows... Goodbye!")
