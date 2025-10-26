# Debug module loading
import stringutils

print("stringutils type:", type(stringutils))
print("dir() of stringutils:", dir(stringutils))

# Try to access functions
print("stringutils.reverse('test'):", stringutils.reverse('test'))
print("stringutils.count_chars('hello'):", stringutils.count_chars('hello'))
