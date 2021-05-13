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

# Q6
print("Q6 : Matrix using numpy")
import numpy as np
data = np.array([[4,2,7,11,8,80],
                [9,22,73,41,57,20],
                [47,29,87,41,33,92],
                [3,47,44,14,62,80],
                [34,61,1,51,8,34]])

print('a. max = ', data.max())
print('b. sum = ', data.sum(axis=1))
print('c. mean = ', data.mean(axis=0))

print()



# Q7
print("Q7 : Continent")

import csv
f_name = "./files/UN.txt"
f = open(f_name, 'r', encoding='utf-8')
rdr = csv.reader(f)
data = []
for line in rdr:
    data.append(line)

continent = input('Enter the name of a continent:')

for country in data:
    if country[1].lower() == continent.lower():
        print(country[0])
#             print("True" if strData[i].find("python") > -1 else "False")

f.close()
print()


# Q8
print("Q8 : String search")


strData = ['good item', 'hello word', 'python programming', 'real data', 'script python']


def search_coroutine():
    try:
        while True:
            x = (yield)
            print("True" if strData[i].find("python") > -1 else "False")

    except GeneratorExit:
        print()
        print("coroutine end")


co = search_coroutine()
next(co)

for i in range(len(strData)):
    co.send(i)

co.close()
