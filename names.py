import sys

names = ["Jack","Tim","Jackson","Jackblack","Susan","Ben","Ron"]

if "Ron" in names:
    print("Found")
    sys.exit(0)

print("Not found")
sys.exit(1)