# Test user's example
def serveit_run(app, host, port, config):
    print("Server starting:")
    print(f"  App: {app}")
    print(f"  Host: {host}")
    print(f"  Port: {port}")
    print(f"  Config: {config}")

serveit_run("myapp", "0.0.0.0", 8080, {
    "log_level": "info",    # Log level: debug, info, warn, error
    "reload": False,        # Hot reload on code changes
    "workers": 1            # Number of worker processes
})

# Test multi-line list
myList = [
    1,
    2
]
print("List:", myList)
