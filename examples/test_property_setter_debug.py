# Test property setter
class Temperature:
    def __init__(self, celsius):
        print("__init__ called with celsius =", celsius)
        self._celsius = celsius
        print("After setting _celsius, self._celsius =", self._celsius)

    def get_celsius(self):
        print("get_celsius called, returning", self._celsius)
        return self._celsius

    def set_celsius(self, value):
        print("set_celsius called with value =", value)
        print("Before setting, self._celsius =", self._celsius)
        self._celsius = value
        print("After setting, self._celsius =", self._celsius)

    celsius = property(get_celsius, set_celsius)

print("Creating Temperature(25)")
temp = Temperature(25)
print("temp.celsius =", temp.celsius)
print("\nSetting temp.celsius = 30")
temp.celsius = 30
print("After setting, temp.celsius =", temp.celsius)
