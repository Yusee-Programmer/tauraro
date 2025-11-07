"""
FastAPI benchmark application
Simple routes for performance testing
"""
from fastapi import FastAPI
from fastapi.responses import HTMLResponse, JSONResponse

app = FastAPI()

@app.get("/")
async def root():
    return HTMLResponse("<h1>FastAPI Root</h1>")

@app.get("/api/hello")
async def hello():
    return JSONResponse({"message": "Hello, World!"})

@app.get("/api/user/{user_id}")
async def get_user(user_id: int):
    return JSONResponse({
        "id": user_id,
        "name": f"User {user_id}",
        "email": f"user{user_id}@example.com"
    })

@app.get("/api/data")
async def get_data():
    return JSONResponse({
        "items": [
            {"id": i, "value": f"Item {i}"}
            for i in range(100)
        ],
        "total": 100
    })

@app.get("/html/page")
async def html_page():
    return HTMLResponse("""
    <!DOCTYPE html>
    <html>
    <head>
        <title>Test Page</title>
        <style>
            body { font-family: Arial; padding: 20px; }
            h1 { color: blue; }
        </style>
    </head>
    <body>
        <h1>FastAPI Test Page</h1>
        <p>This is a test page for benchmarking.</p>
    </body>
    </html>
    """)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8000, log_level="error")
