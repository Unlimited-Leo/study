#from cs50 import get_int
#import cs50
#x = cs50.get_int("x:")
#y = cs50.get_int("y:")
try:
    x = int(input("x:"))
except ValueError:
    print("That is not an int!")
    exit()
try:
    y = int(input("y:"))
except:
    print("That is not an int!")
    exit()
print(x+y)