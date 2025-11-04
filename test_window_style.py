# Test window style computation (the actual DUITK use case)

class WindowTest:
    def __init__(self):
        # WS_OVERLAPPEDWINDOW | WS_VISIBLE (real Windows API values)
        style = 0x10000000 | 0x00CF0000
        print(f"Window style: {style}")
        print(f"In hex: {hex(style)}")

        self.style = style

obj = WindowTest()
print(f"Stored style: {obj.style}")
print(f"In hex: {hex(obj.style)}")
