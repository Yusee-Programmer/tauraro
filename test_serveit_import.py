#!/usr/bin/env python3
# Test serveit module import

print("Testing serveit import...")

try:
    import serveit
    print("✓ serveit imported successfully")
    print(f"  Version: {serveit.VERSION}")
    print(f"  Server Name: {serveit.SERVER_NAME}")
    print()
    print("Available functions:")
    print("  - serveit.run()")
    print("  - serveit.serve()")
    print("  - serveit.JSONResponse()")
    print("  - serveit.HTMLResponse()")
    print("  - serveit.RedirectResponse()")
    print("  - serveit.FileResponse()")
    print("  - serveit.Response()")
    print("  - serveit.Server()")
    print("  - serveit.Router()")
    print()
    print("Status codes available:")
    print(f"  - serveit.status.OK = {serveit.status['OK']}")
    print(f"  - serveit.status.NOT_FOUND = {serveit.status['NOT_FOUND']}")
    print(f"  - serveit.status.INTERNAL_SERVER_ERROR = {serveit.status['INTERNAL_SERVER_ERROR']}")
    print()
    print("ServEit is ready to use!")
except ImportError as e:
    print(f"✗ Failed to import serveit: {e}")
except Exception as e:
    print(f"✗ Error: {e}")
