# Simple test to debug serveit response
import serveit

def app(scope):
    print(f"App called with scope type: {type(scope)}")
    print(f"Scope value: {scope}")

    # Create a simple response dict manually
    response = {
        "status": 200,
        "body": "Test HTML",
        "headers": {
            "content-type": "text/html; charset=utf-8"
        }
    }
    print(f"Returning response: {response}")
    return response

serveit.run(app, "127.0.0.1", 8000)
