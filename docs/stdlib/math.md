# Math Module

The `math` module provides mathematical functions and constants for numerical computations.

## Constants

```python
import math

print(math.pi)      # 3.141592653589793
print(math.e)       # 2.718281828459045
print(math.tau)     # 6.283185307179586 (2*pi)
print(math.inf)     # Infinity
print(math.nan)     # Not a Number
```

## Basic Functions

### Powers and Logarithms

```python
import math

# Square root
print(math.sqrt(16))        # 4.0
print(math.sqrt(2))         # 1.41421356...

# Power
print(math.pow(2, 10))      # 1024.0

# Exponential
print(math.exp(1))          # 2.718... (e^1)
print(math.exp(2))          # 7.389... (e^2)

# Logarithms
print(math.log(10))         # 2.302... (natural log)
print(math.log10(100))      # 2.0 (base-10 log)
print(math.log2(8))         # 3.0 (base-2 log)
print(math.log(100, 10))    # 2.0 (custom base)
```

### Trigonometric Functions

```python
import math

# Sine, cosine, tangent (in radians)
print(math.sin(math.pi / 2))    # 1.0
print(math.cos(0))               # 1.0
print(math.tan(math.pi / 4))    # 1.0

# Inverse trigonometric
print(math.asin(1))             # π/2
print(math.acos(0))             # π/2
print(math.atan(1))             # π/4
print(math.atan2(1, 1))         # π/4

# Hyperbolic functions
print(math.sinh(0))             # 0.0
print(math.cosh(0))             # 1.0
print(math.tanh(0))             # 0.0
```

### Degree/Radian Conversion

```python
import math

# Convert degrees to radians
radians = math.radians(180)     # π
radians = math.radians(90)      # π/2

# Convert radians to degrees
degrees = math.degrees(math.pi)     # 180.0
degrees = math.degrees(math.pi/2)   # 90.0
```

## Rounding Functions

```python
import math

# Ceiling (round up)
print(math.ceil(4.3))       # 5
print(math.ceil(-4.3))      # -4

# Floor (round down)
print(math.floor(4.8))      # 4
print(math.floor(-4.8))     # -5

# Truncate (round towards zero)
print(math.trunc(4.8))      # 4
print(math.trunc(-4.8))     # -4
```

## Advanced Functions

### Factorial and Combinations

```python
import math

# Factorial
print(math.factorial(5))    # 120 (5!)
print(math.factorial(10))   # 3628800

# Combinations
print(math.comb(5, 2))      # 10 (C(5,2))
print(math.comb(10, 3))     # 120

# Permutations
print(math.perm(5, 2))      # 20 (P(5,2))
```

### GCD and LCM

```python
import math

# Greatest common divisor
print(math.gcd(48, 18))     # 6
print(math.gcd(100, 75))    # 25

# Least common multiple
print(math.lcm(12, 18))     # 36
print(math.lcm(4, 6))       # 12
```

### Other Useful Functions

```python
import math

# Absolute value
print(math.fabs(-5.5))      # 5.5

# Check if values are close
print(math.isclose(0.1 + 0.2, 0.3))  # True

# Check for special values
print(math.isnan(math.nan))     # True
print(math.isinf(math.inf))     # True
print(math.isfinite(42))        # True
```

## Complete Examples

### Distance Between Points

```python
import math

def distance(x1: float, y1: float, x2: float, y2: float) -> float:
    """Calculate Euclidean distance between two points."""
    return math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

print(distance(0, 0, 3, 4))  # 5.0
```

### Circle Calculations

```python
import math

class Circle:
    def __init__(self, radius: float):
        self.radius = radius

    def area(self) -> float:
        return math.pi * self.radius ** 2

    def circumference(self) -> float:
        return 2 * math.pi * self.radius

circle = Circle(5)
print(f"Area: {circle.area()}")                # 78.54
print(f"Circumference: {circle.circumference()}")  # 31.42
```

### Angle Calculations

```python
import math

def angle_between_vectors(x1: float, y1: float, x2: float, y2: float) -> float:
    """Calculate angle between two vectors in degrees."""
    dot_product = x1 * x2 + y1 * y2
    mag1 = math.sqrt(x1**2 + y1**2)
    mag2 = math.sqrt(x2**2 + y2**2)

    cos_angle = dot_product / (mag1 * mag2)
    angle_rad = math.acos(cos_angle)

    return math.degrees(angle_rad)

print(angle_between_vectors(1, 0, 0, 1))  # 90.0 degrees
```

## Next Steps

- [Random Module](random.md) - Random number generation
- [Statistics](#) - Statistical functions
- [Numeric Types](../language/data-types.md) - Working with numbers
