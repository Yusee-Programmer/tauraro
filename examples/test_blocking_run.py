"""
Test that window.run() blocks until window closes
"""

import webviewtk as wv

print("Creating window...")
window = wv.Window("Test Blocking Run", 600, 400)

html = "<!DOCTYPE html>" + wv.html(
    wv.render(
        wv.head(wv.cdn_tailwind()),
        wv.body(
            wv.div(
                wv.render(
                    wv.h1("Blocking Run Test", "text-4xl font-bold text-center mb-4 text-blue-600"),
                    wv.p("This window should stay open until you close it.", "text-center text-gray-700 text-xl mb-4"),
                    wv.p("The script is blocked at window.run().", "text-center text-gray-600"),
                    wv.p("Close this window to exit the program.", "text-center text-gray-600 font-semibold")
                ),
                "container mx-auto p-8 flex flex-col justify-center h-screen"
            ),
            "bg-gray-50"
        )
    )
)

window.set_html(html)

print("Calling window.run() - this should block...")
window.run()  # This should block until window closes

print("Window was closed! Script finishing...")
