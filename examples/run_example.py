"""
WebViewTK Example Launcher
A simple menu to run any WebViewTK example
"""

def print_menu():
    print("\n" + "="*60)
    print("  WebViewTK Comprehensive Examples")
    print("="*60)
    print("\nChoose an example to run:\n")
    print("  1. Dashboard Pro - Analytics dashboard with animations")
    print("  2. TechStore - E-commerce store with shopping cart")
    print("  3. SocialHub - Social media feed with posts & likes")
    print("  4. Modern Portfolio - Portfolio/landing page")
    print("  5. Basic WebViewTK - Simple demonstration")
    print("  6. Simple Window - Minimal window test")
    print("\n  0. Exit")
    print("\n" + "="*60)

def get_example_file(choice):
    examples = {
        "1": "./examples/webviewtk_dashboard.py",
        "2": "./examples/webviewtk_ecommerce.py",
        "3": "./examples/webviewtk_social_media.py",
        "4": "./examples/webviewtk_portfolio.py",
        "5": "./examples/test_webviewtk.py",
        "6": "./examples/test_window_display.py"
    }
    return examples.get(choice)

def main():
    while True:
        print_menu()
        choice = input("\nEnter your choice (0-6): ").strip()

        if choice == "0":
            print("\nThank you for trying WebViewTK examples!")
            print("Happy coding! 🚀\n")
            break

        example_file = get_example_file(choice)

        if example_file:
            print(f"\n{'='*60}")
            print(f"  Launching: {example_file}")
            print(f"{'='*60}\n")
            print("Instructions:")
            print("  - The window will open automatically")
            print("  - Close the window to return to this menu")
            print("  - Press Ctrl+C to force quit if needed")
            print(f"\n{'='*60}\n")

            # Import and run the example
            import subprocess
            import sys

            try:
                # Run the example
                result = subprocess.run(
                    [sys.executable, "-c", f"exec(open('{example_file}').read())"],
                    cwd=".",
                    check=False
                )

                if result.returncode != 0:
                    print(f"\nExample exited with code {result.returncode}")

            except KeyboardInterrupt:
                print("\n\nExample interrupted by user.")
            except Exception as e:
                print(f"\nError running example: {e}")

            input("\nPress Enter to continue...")
        else:
            print("\nInvalid choice! Please select 0-6.")
            input("\nPress Enter to continue...")

if __name__ == "__main__":
    print("\n" + "🚀 "*20)
    print("  Welcome to Tauraro WebViewTK Examples!")
    print("🚀 "*20)

    try:
        main()
    except KeyboardInterrupt:
        print("\n\nExiting... Goodbye! 👋\n")
