# Binary Data Services

The modules described in this chapter provide interfaces for manipulating binary data and for using binary data in various contexts.

## `struct` — Interpret bytes as packed binary data

This module performs conversions between Python values and C structs represented as Python bytes objects.

### Functions and Exceptions

#### `pack(format, v1, v2, ...)`

Return a bytes object containing the values v1, v2, ... packed according to the format string format.

```tauraro
import struct

# Pack integers
>>> struct.pack('i', 42)
b'*\x00\x00\x00'

# Pack multiple values
>>> struct.pack('iii', 1, 2, 3)
b'\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00'

# Pack with different formats
>>> struct.pack('hhl', 1, 2, 3)  # short, short, long
b'\x01\x00\x02\x00\x03\x00\x00\x00'

# Pack floating point
>>> struct.pack('f', 3.14)
b'\xc3\xf5H@'

# Pack string
>>> struct.pack('5s', b'hello')
b'hello'
```

#### `unpack(format, buffer)`

Unpack from the buffer buffer (presumably packed by pack(format, ...)) according to the format string format.

```tauraro
import struct

# Unpack integers
>>> struct.unpack('i', b'*\x00\x00\x00')
(42,)

# Unpack multiple values
>>> struct.unpack('iii', b'\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00')
(1, 2, 3)

# Unpack floating point
>>> struct.unpack('f', b'\xc3\xf5H@')
(3.1400001049041748,)
```

#### `calcsize(format)`

Return the size of the struct (and hence of the bytes object produced by pack(format, ...)) corresponding to the format string format.

```tauraro
import struct

>>> struct.calcsize('i')  # int
4
>>> struct.calcsize('h')  # short
2
>>> struct.calcsize('f')  # float
4
>>> struct.calcsize('iii')  # three ints
12
```

### Format Strings

Format characters have the following meaning:

| Character | Type | Standard size |
|-----------|------|---------------|
| `x` | pad byte | no value | 1 |
| `c` | char | bytes of length 1 | 1 |
| `b` | signed char | integer | 1 |
| `B` | unsigned char | integer | 1 |
| `?` | _Bool | bool | 1 |
| `h` | short | integer | 2 |
| `H` | unsigned short | integer | 2 |
| `i` | int | integer | 4 |
| `I` | unsigned int | integer | 4 |
| `l` | long | integer | 4 |
| `L` | unsigned long | integer | 4 |
| `q` | long long | integer | 8 |
| `Q` | unsigned long long | integer | 8 |
| `n` | ssize_t | integer |  |
| `N` | size_t | integer |  |
| `e` | half precision | float | 2 |
| `f` | float | float | 4 |
| `d` | double | float | 8 |
| `s` | char[] | bytes |  |
| `p` | char[] | bytes |  |
| `P` | void * | integer |  |

```tauraro
import struct

# Byte order and size
# @ - native byte order, native size
# = - native byte order, standard size
# < - little-endian, standard size
# > - big-endian, standard size
# ! - network (=big-endian), standard size

# Little-endian vs big-endian
>>> struct.pack('<i', 0x12345678)
b'xV4\x12'
>>> struct.pack('>i', 0x12345678)
b'\x124Vx'

# Native vs standard size
>>> import sys
>>> sys.byteorder
'little'
>>> struct.pack('@i', 42)  # Native byte order
b'*\x00\x00\x00'
>>> struct.pack('!i', 42)  # Network byte order (big-endian)
b'\x00\x00\x00*'
```

### Examples

#### Packing and unpacking integers

```tauraro
import struct

# Pack signed 32-bit integer
def pack_int32(value):
    return struct.pack('i', value)

def unpack_int32(data):
    return struct.unpack('i', data)[0]

>>> packed = pack_int32(123456789)
>>> packed
b'\x15\xcd[\x07'
>>> unpack_int32(packed)
123456789
```

#### Packing and unpacking floating point numbers

```tauraro
import struct

# Pack float (32-bit)
def pack_float(value):
    return struct.pack('f', value)

def unpack_float(data):
    return struct.unpack('f', data)[0]

>>> packed = pack_float(3.14159)
>>> packed
b'\xdb\x0fI@'
>>> unpack_float(packed)
3.141590118408203
```

#### Packing and unpacking strings

```tauraro
import struct

# Pack fixed-length string
def pack_string(s, length):
    return struct.pack(f'{length}s', s.encode('utf-8')[:length])

def unpack_string(data):
    return struct.unpack(f'{len(data)}s', data)[0].decode('utf-8').rstrip('\x00')

>>> packed = pack_string("Hello", 10)
>>> packed
b'Hello\x00\x00\x00\x00\x00'
>>> unpack_string(packed)
'Hello'
```

#### Complex data structures

```tauraro
import struct

# Define a simple data structure: header with id, size, and flags
def pack_header(id, size, flags):
    return struct.pack('III', id, size, flags)

def unpack_header(data):
    return struct.unpack('III', data)

# Define a point structure: x, y coordinates as floats
def pack_point(x, y):
    return struct.pack('ff', x, y)

def unpack_point(data):
    return struct.unpack('ff', data)

# Example usage
>>> header_data = pack_header(1, 100, 0x01)
>>> header_data
b'\x01\x00\x00\x00d\x00\x00\x00\x01\x00\x00\x00'
>>> unpack_header(header_data)
(1, 100, 1)

>>> point_data = pack_point(3.14, 2.71)
>>> point_data
b'\xc3\xf5H@\x9a\x99\xae@'
>>> unpack_point(point_data)
(3.1400001049041748, 2.7100000381469727)
```

### Struct Objects

The `Struct` class represents a compiled struct object capable of packing and unpacking data.

```tauraro
import struct

# Create a struct object
point_struct = struct.Struct('ff')  # Two floats

# Pack data
>>> point_struct.pack(1.0, 2.0)
b'\x00\x00\x80?\x00\x00\x00@'

# Unpack data
>>> point_struct.unpack(b'\x00\x00\x80?\x00\x00\x00@')
(1.0, 2.0)

# Get format and size
>>> point_struct.format
'ff'
>>> point_struct.size
8
```

## `codecs` — Codec registry and base classes

This module defines the core API for codecs and provides various codec implementations.

### Codec Base Classes

#### Codec

Codec is the base class for all codec classes.

```tauraro
import codecs

# Example of a simple codec
class Rot13Codec(codecs.Codec):
    def encode(self, input, errors='strict'):
        return codecs.encode(input, 'rot13'), len(input)
    
    def decode(self, input, errors='strict'):
        return codecs.decode(input, 'rot13'), len(input)

# Register the codec
def rot13_search_function(encoding):
    if encoding == 'rot13':
        return codecs.CodecInfo(
            name='rot13',
            encode=Rot13Codec().encode,
            decode=Rot13Codec().decode,
        )
    return None

codecs.register(rot13_search_function)

# Use the codec
>>> codecs.encode('Hello World', 'rot13')
'Uryyb Jbeyq'
>>> codecs.decode('Uryyb Jbeyq', 'rot13')
'Hello World'
```

### Standard Encodings

Python comes with a number of built-in codecs for various encodings.

```tauraro
import codecs

# UTF-8 encoding
>>> codecs.encode('Hello 世界', 'utf-8')
b'Hello \xe4\xb8\x96\xe7\x95\x8c'
>>> codecs.decode(b'Hello \xe4\xb8\x96\xe7\x95\x8c', 'utf-8')
'Hello 世界'

# ASCII encoding
>>> codecs.encode('Hello', 'ascii')
b'Hello'
>>> codecs.decode(b'Hello', 'ascii')
'Hello'

# Base64 encoding
>>> codecs.encode(b'Hello World', 'base64')
b'SGVsbG8gV29ybGQ=\n'
>>> codecs.decode(b'SGVsbG8gV29ybGQ=\n', 'base64')
b'Hello World'

# Hexadecimal encoding
>>> codecs.encode(b'Hello', 'hex')
b'48656c6c6f'
>>> codecs.decode(b'48656c6c6f', 'hex')
b'Hello'
```

### Incremental Encoding and Decoding

Incremental codecs allow encoding and decoding data in chunks.

```tauraro
import codecs

# Incremental encoder
encoder = codecs.getincrementalencoder('utf-8')()
>>> encoder.encode('Hello ')
b'Hello '
>>> encoder.encode('World')
b'World'
>>> encoder.encode('', final=True)  # Finalize
b''

# Incremental decoder
decoder = codecs.getincrementaldecoder('utf-8')()
>>> decoder.decode(b'Hello ')
'Hello '
>>> decoder.decode(b'World')
'World'
>>> decoder.decode(b'', final=True)  # Finalize
''
```

### Stream Encoding and Decoding

Stream codecs provide encoding and decoding for file-like objects.

```tauraro
import codecs

# Stream writer
with open('output.txt', 'wb') as f:
    writer = codecs.getwriter('utf-8')(f)
    writer.write('Hello 世界\n')

# Stream reader
with open('output.txt', 'rb') as f:
    reader = codecs.getreader('utf-8')(f)
    content = reader.read()
    print(content)  # Hello 世界
```

## Working with Binary Data

### Bytes and Bytearray

Python provides two main types for binary data: `bytes` (immutable) and `bytearray` (mutable).

```tauraro
# Creating bytes
b1 = b'hello'
b2 = bytes([104, 101, 108, 108, 111])
b3 = 'hello'.encode('utf-8')

# Creating bytearray
ba1 = bytearray(b'hello')
ba2 = bytearray([104, 101, 108, 108, 111])
ba3 = bytearray(5)  # Creates 5 zero bytes

# Bytes methods
>>> b'hello'.upper()
b'HELLO'
>>> b'hello world'.split()
[b'hello', b'world']
>>> b'hello'.startswith(b'he')
True

# Bytearray methods (mutable)
>>> ba = bytearray(b'hello')
>>> ba[0] = ord(b'H')
>>> ba
bytearray(b'Hello')
>>> ba.append(33)  # Append '!'
>>> ba
bytearray(b'Hello!')
```

### Memory Views

Memory views allow you to access the internal data of an object that supports the buffer protocol without copying.

```tauraro
# Create a bytearray
data = bytearray(b'Hello World')

# Create a memoryview
view = memoryview(data)

# Access data
>>> view[0]
72  # ASCII value of 'H'
>>> view[0:5].tobytes()
b'Hello'

# Modify data through view
>>> view[0] = ord(b'h')
>>> data
bytearray(b'hello World')

# Create views of different formats
import struct
int_data = bytearray(8)
int_view = memoryview(int_data).cast('i')  # View as integers
>>> len(int_view)
2  # 8 bytes / 4 bytes per int = 2 ints
>>> int_view[0] = 42
>>> int_data
bytearray(b'*\x00\x00\x00\x00\x00\x00\x00')
```

## Best Practices

### Working with Binary Data

1. **Use appropriate data types**: Use `bytes` for immutable binary data and `bytearray` for mutable binary data
2. **Specify encoding explicitly**: Always specify the encoding when encoding/decoding text
3. **Use struct for binary protocols**: Use the `struct` module for packing/unpacking binary data according to specific formats
4. **Handle errors appropriately**: Use appropriate error handling strategies for encoding/decoding

```tauraro
import struct
import codecs

# Good practices for binary data

# 1. Use appropriate types
def process_binary_data(data):
    if isinstance(data, str):
        # Convert string to bytes
        data = data.encode('utf-8')
    elif not isinstance(data, (bytes, bytearray)):
        raise TypeError("Expected bytes, bytearray, or str")
    
    # Process binary data
    return data

# 2. Specify encoding explicitly
def safe_encode(text, encoding='utf-8'):
    return text.encode(encoding)

def safe_decode(data, encoding='utf-8'):
    return data.decode(encoding)

# 3. Use struct for binary protocols
def pack_network_message(msg_id, payload):
    # Pack message header: 4-byte ID, 4-byte payload length
    header = struct.pack('!II', msg_id, len(payload))
    return header + payload

def unpack_network_message(data):
    # Unpack header
    header_size = struct.calcsize('!II')
    msg_id, payload_len = struct.unpack('!II', data[:header_size])
    payload = data[header_size:header_size + payload_len]
    return msg_id, payload

# 4. Handle errors appropriately
def robust_decode(data, encoding='utf-8'):
    try:
        return data.decode(encoding)
    except UnicodeDecodeError as e:
        # Handle decode error
        print(f"Decode error: {e}")
        # Try with error handling
        return data.decode(encoding, errors='replace')
```

### Performance Considerations

1. **Use memoryview for large data**: Avoid copying large binary data
2. **Compile struct formats**: Use `Struct` objects for repeated packing/unpacking
3. **Use incremental codecs**: For processing large streams of data

```tauraro
import struct
import codecs

# Performance optimizations

# 1. Use memoryview for large data
def process_large_buffer(buffer):
    view = memoryview(buffer)
    # Process chunks without copying
    for i in range(0, len(view), 1024):
        chunk = view[i:i+1024]
        # Process chunk
        pass

# 2. Compile struct formats
point_struct = struct.Struct('ff')  # Compile once, use many times

def process_points(points):
    for x, y in points:
        packed = point_struct.pack(x, y)
        # Process packed data
        pass

# 3. Use incremental codecs for streams
def process_text_stream(input_stream, output_stream, encoding='utf-8'):
    decoder = codecs.getincrementaldecoder(encoding)()
    encoder = codecs.getincrementalencoder(encoding)()
    
    while True:
        chunk = input_stream.read(1024)
        if not chunk:
            break
        
        # Decode chunk
        text = decoder.decode(chunk)
        
        # Process text
        processed_text = text.upper()
        
        # Encode processed text
        encoded = encoder.encode(processed_text)
        output_stream.write(encoded)
    
    # Finalize
    final_text = decoder.decode(b'', final=True)
    final_encoded = encoder.encode(final_text, final=True)
    output_stream.write(final_encoded)
```

### Error Handling

1. **Handle encoding/decoding errors**: Use appropriate error handling strategies
2. **Validate binary data**: Check data integrity and format
3. **Use context managers**: Ensure proper resource cleanup

```tauraro
import struct
import codecs

# Error handling best practices

# 1. Handle encoding/decoding errors
def safe_text_processing(data, input_encoding='utf-8', output_encoding='utf-8'):
    try:
        # Decode input
        text = data.decode(input_encoding)
    except UnicodeDecodeError as e:
        print(f"Input decode error: {e}")
        # Try with error handling
        text = data.decode(input_encoding, errors='replace')
    
    # Process text
    processed_text = text.upper()
    
    try:
        # Encode output
        result = processed_text.encode(output_encoding)
    except UnicodeEncodeError as e:
        print(f"Output encode error: {e}")
        # Try with error handling
        result = processed_text.encode(output_encoding, errors='replace')
    
    return result

# 2. Validate binary data
def validate_struct_data(data, format_string):
    expected_size = struct.calcsize(format_string)
    if len(data) != expected_size:
        raise ValueError(f"Expected {expected_size} bytes, got {len(data)}")
    
    try:
        return struct.unpack(format_string, data)
    except struct.error as e:
        raise ValueError(f"Invalid data format: {e}")

# 3. Use context managers
def process_binary_file(filename):
    with open(filename, 'rb') as f:
        # Read binary data
        data = f.read()
        
        # Process data
        # ...
        
        # Write result
        with open(filename + '.out', 'wb') as out_f:
            out_f.write(processed_data)
```