# Test simple duitk usage without import

print("Testing DUITK classes...")

# Simplified DUITK classes
class Application:
    def __init__(self):
        self.windows = []
        self.running = False

class Window:
    def __init__(self, title, width, height):
        self.title = title
        self.width = width
        self.height = height

# Test
app = Application()
print("Application created")

window = Window("Test", 800, 600)
print(f"Window created: {window.title}")

print("Simple DUITK test complete!")
