# Release Notes - Week of June 13-19, 2026

## Overview
This release focuses on significant improvements to memory management, concurrency features, I/O operations, and performance optimization. Major enhancements include memory leak prevention across all collection types, advanced concurrency primitives, FFI support, and a comprehensive benchmarking suite.

---

## ✨ Major Features

### 1. **Memory Management Enhancements**
- **Core Memory Allocation System**: Implemented core memory allocation, I/O, map, string, and vector functionalities
- **Safe Empty String Handling**: Introduced `_tr_empty_heap_str` for safe empty string handling and prevention of use-after-free issues
- **Memory Leak Prevention**: Added `free` methods to Pair, StrPair, Triple, HttpHeader, Url, TimeDelta, DateTime, Date, and Time classes
- **String Retention Logic**: Enhanced CGenerator with safe C variable names and improved string retention logic for memory safety
- **Collection Memory Safety**: 
  - Added `Dict_free_strval` and `_tr_idict_free_strval` functions for proper TrStr handling in Dict and Map types
  - Updated List and Set append methods to intelligently increase capacity (by 50-100%) to improve performance and prevent memory issues
  - Added escape tracking for List/Vec/Dict/Map/Set locals to prevent use-after-free issues
- **HttpConn Resource Management**: Added dispose method to HttpConn for proper connection cleanup and memory release

### 2. **Concurrency Model Improvements**
- **Sendable Interface**: Enhanced concurrency model with new Sendable interface for thread-safe type assertions
- **UnsafeSendable Interface**: Implemented UnsafeSendable for explicit thread safety assertions in specialized scenarios
- **Memory Leak Detection Framework**: Added comprehensive memory leak detection framework for concurrent code
- **Enhanced Threading Model**: Improved threading implementation and added async accept method for TcpListener
- **Detached Task Spawning**: Added support for detached task spawning and async I/O operations

### 3. **I/O and Networking Features**
- **Async I/O Operations**: Implemented async I/O operations for TcpStream and TcpListener
- **HTTP Server Enhancements**:
  - Added request body size limit and oversized request handling in HttpServer
  - Added memory allocation tracking in HttpParser for better resource management
- **WSAPoll Integration**: Replaced select() with WSAPoll for improved scalability in _TrIOPoll
- **HttpServer and HttpConn**: Improved memory handling in TcpStream and HttpParser with enhanced string allocation tracking

### 4. **FFI (Foreign Function Interface)**
- **FFI Export Support**: Added FFI support for exporting functions to shared libraries
- **Function Export Field**: Added `is_export` field to FunctionDef for FFI export visibility control
- **Seamless Interoperability**: Enable functions to be exported to shared libraries for C/C++ integration

### 5. **Standard Library Additions**
- **TOML Parser & Serializer**: Implemented complete TOML parser and serializer in `std.encoding.toml`
- **Enhanced String Utilities**: Updated string handling and slicing across JSON and Path modules
- **Memory-Safe String Operations**: Refactored string slicing to use native slice method with proper memory cleanup

### 6. **Type System Enhancements**
- **Tuple Type Integration**: Enhanced tuple handling by integrating Tuple type and updating related functions
- **Improved Collections Support**: Updated code generation and semantic analysis for better tuple and collection support
- **Better Type Safety**: Improved semantic analysis for local variable management

### 7. **Semantic Analysis & MIR Improvements**
- **Drop Handling**: Enhanced MIR drop handling for collections and improved semantic analysis for local variable management
- **Memory Management Analysis**: 
  - Enhanced MIR memory management with unsafe block handling and manual free recognition
  - Improved drop analysis to prevent double-dropping of locals
- **Block Scope Tracking**: Added block ID tracking and methods for opening/closing blocks in semantic analyzer
- **String Escape Tracking**: Added str_escaped tracking for local variables with improved auto-drop logic

### 8. **Benchmarking Suite**
- **Performance Analysis**: Added comprehensive benchmarking suite with results reporting for performance analysis
- **Benchmark Tests**: Includes 10 different benchmark categories (sum, fibonacci, float_mul, xorshift, newton, mandelbrot, sieve, nbody, collatz, matmul)
- **Script Improvements**: Updated benchmark results and improved script directory handling for accurate measurements

---

## 🔧 Technical Improvements

### Code Generation
- Enhanced CGenerator for safer C code generation with improved variable naming
- Better handling of string types in collections
- Improved code generation for drop and cleanup operations

### Build & Bootstrap
- Rebootstrapped compiler with latest features
- Updated build scripts for improved compilation

### Testing
- Added regression tests for enhanced error handling
- Improved error handling in collections and concurrency features
- Added examples for auditing class field release, enum handling, and tuple destructuring

---

## 🐛 Bug Fixes & Optimizations

- **Memory Leak Fixes**: 
  - Fixed binop concat handler in code generation - eliminated string memory leaks in Str.trim (16MB→0MB)
  - Fixed over-conservative collection escape marking
  - Enhanced encoding/decoding functions (Base64, Hex, JSON, URL, DateTime) with proper cleanup
- **Code Structure**: Refactored code for improved readability and maintainability
- **Performance**: Improved capacity allocation strategies for better performance

---

## 📚 Documentation
- Updated documentation for new features and improvements
- Added examples for advanced patterns and memory management

---

## 🎯 Focus Areas
- **Stability**: Significant focus on memory safety and leak prevention
- **Performance**: Benchmarking and optimization improvements
- **Interoperability**: FFI support for C/C++ integration
- **Developer Experience**: Enhanced type system and better error handling

---

## 🚀 Commits Included
Total commits: 54 commits across 7+ days of development

---

## Notes
- All changes maintain backward compatibility
- Memory management improvements may affect existing code performance positively
- FFI export feature enables new integration possibilities with C/C++ libraries
- Benchmarking suite provides tools for performance validation and optimization

---

*Generated on: June 19, 2026*
