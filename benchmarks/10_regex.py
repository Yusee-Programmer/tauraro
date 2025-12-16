#!/usr/bin/env python3
"""
Benchmark: Regular Expression Operations
Tests: Regex compilation, matching, searching, replacing
"""
import time
import sys
import re

def regex_compile_test(patterns):
    """Test regex compilation"""
    compiled = []
    for pattern in patterns:
        compiled.append(re.compile(pattern))
    return compiled

def regex_match_test(regex_list, text, iterations):
    """Test regex matching"""
    match_count = 0
    for _ in range(iterations):
        for regex in regex_list:
            if regex.search(text):
                match_count += 1
    return match_count

def regex_findall_test(text):
    """Test findall operation"""
    # Find all email-like patterns
    emails = re.findall(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b', text)
    # Find all numbers
    numbers = re.findall(r'\d+', text)
    # Find all words
    words = re.findall(r'\b\w+\b', text)
    return len(emails), len(numbers), len(words)

def regex_replace_test(text):
    """Test regex replacement"""
    # Replace emails with [EMAIL]
    text1 = re.sub(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b', '[EMAIL]', text)
    # Replace numbers with [NUM]
    text2 = re.sub(r'\d+', '[NUM]', text1)
    # Normalize whitespace
    text3 = re.sub(r'\s+', ' ', text2)
    return len(text3)

def main():
    iterations = 1000 if len(sys.argv) < 2 else int(sys.argv[1])

    # Create test data
    test_text = """
    Contact us at support@example.com or sales@company.org.
    Our phone numbers are 123-456-7890 and 098-765-4321.
    Visit our website at https://www.example.com for more information.
    Order #12345 was processed on 2024-01-15 at 14:30:00.
    User john.doe@email.com made 42 purchases totaling $1,234.56.
    """ * 100  # Repeat to create larger text

    # Test patterns
    patterns = [
        r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b',  # Email
        r'\d{3}-\d{3}-\d{4}',  # Phone
        r'https?://[^\s]+',  # URL
        r'\$\d+\.\d{2}',  # Price
        r'\d{4}-\d{2}-\d{2}',  # Date
    ]

    # Test 1: Regex compilation
    print(f"Test 1: Compiling {len(patterns)} regex patterns")
    start = time.time()
    regex_list = regex_compile_test(patterns)
    elapsed1 = time.time() - start
    print(f"  Patterns compiled: {len(regex_list)}")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: Regex matching
    print(f"\nTest 2: Regex matching ({iterations} iterations)")
    start = time.time()
    match_count = regex_match_test(regex_list, test_text, iterations)
    elapsed2 = time.time() - start
    print(f"  Total matches: {match_count}")
    print(f"  Time: {elapsed2:.4f} seconds")

    # Test 3: Findall operations
    print(f"\nTest 3: Findall operations")
    start = time.time()
    email_count, number_count, word_count = regex_findall_test(test_text)
    elapsed3 = time.time() - start
    print(f"  Emails: {email_count}, Numbers: {number_count}, Words: {word_count}")
    print(f"  Time: {elapsed3:.4f} seconds")

    # Test 4: Replacement operations
    print(f"\nTest 4: Replacement operations")
    start = time.time()
    result_len = regex_replace_test(test_text)
    elapsed4 = time.time() - start
    print(f"  Result length: {result_len}")
    print(f"  Time: {elapsed4:.4f} seconds")

    total_time = elapsed1 + elapsed2 + elapsed3 + elapsed4
    print(f"\nTotal time: {total_time:.4f} seconds")
    return total_time

if __name__ == "__main__":
    main()
