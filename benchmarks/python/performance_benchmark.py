import time

def main():
    print("Starting Python performance benchmark...")
    
    start_time = time.time()
    
    a = 1000000
    b = 0
    
    for i in range(0, a):
        b = b + i
        b = b * 2
        b = b - i
        b = b // 2  # Integer division to match Tauraro behavior
    
    end_time = time.time()
    execution_time = end_time - start_time
    
    print("Python arithmetic benchmark result:", b)
    print("Iterations completed:", a)
    print("Execution time:", execution_time, "seconds")
    print("Python performance benchmark completed!")

if __name__ == "__main__":
    main()