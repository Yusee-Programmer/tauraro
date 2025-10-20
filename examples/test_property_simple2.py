# Minimal property setter test
class Temp:
    def __init__(self):
        self._c = 25

    def get_c(self):
        return self._c

    def set_c(self, val):
        self._c = val

    c = property(get_c, set_c)

t = Temp()
print("Initial:", t.c)
t.c = 30
print("After set:", t.c)
