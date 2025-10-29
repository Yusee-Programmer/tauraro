# Test minimal GUI parsing
user32 = load_library("user32.dll")

# Define a simple function
MessageBoxA = define_function("user32.dll", "MessageBoxA", "int", ["pointer", "pointer", "pointer", "int"])

#Test basic class
class SimpleWindow:
    def __init__(self, title):
        self.title = title
        print(f"Window created: {title}")

# Test it
window = SimpleWindow("Test")
print("Success!")
