# Test to see bytecode for property setter
class T:
    def __init__(self):
        self.x = 1

t = T()
t.x = 2
print(t.x)
