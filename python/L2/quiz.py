# Q1
print("Q1 : set")
answer = {i for i in range(1, 101) if i % 3 == 0 and i % 5 == 0}
print(answer)
print()

# Q2
print("Q2 : tuple & comprehension")
a = (5,33,77)
b = (44,823,11)
c = (10,50,90)
answer = {a[i]+b[i]+c[i] for i in range(len(a))}
print(answer)
print()

# Q3
print("Q3 : Iterator")

# def MultiIterator(a, b):
#     return tuple(i for i in range(1, a) if i % b == 0)

class MultiIterator:
    def __init__(self, stop, mul):
        self.current = 1
        self.stop = stop
        self.mul = mul

    def __iter__(self):
        return self

    def __next__(self):
        if self.current * self.mul < self.stop:
            ret = self.current * self.mul
            self.current += 1
            return ret
        else:
            raise StopIteration

for i in MultiIterator(20,3):
    print(i, end=' ')

print()
print()

# Q4
print("Q4 : Generator")
f_name = "./files/words.txt"
with open(f_name) as ifp:
    # lines = ifp.readlines()
    lines = ifp.read().split('\n')
    for line in lines:
        print(line)
print()

# Q5
print("Q5 : email checker")

import re
regex = re.compile('[A-Za-z0-9\._+-]+@[A-Za-z0-9\._+-]+\.(com|org|edu|net|kr|info)') # regex pattern described in textbooks
# regex = re.compile('^[a-zA-Z0-9+-_.]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$') # More(?) accurate regex pattern
emails = ['python@mail.example.com', 'python+kr@example.com', 'python-dojang@example.co.kr', 'python_10@example.info', 'python.dojang@e-xample.com', '@example.com', 'python@example', 'python@example-com']

for email in emails:
    if (regex.match(email) != None):
        print(email, ": True")
    else:
        print(email, ": False")

print()
