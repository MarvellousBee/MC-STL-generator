
# Helper script for parsing 3D objects genereted with an external 3d modelling program
# Not executed anywhere.

def print_vectr(vec) :
    print('    {', end = "")
    for i in range(3) :
        print(vec[i], end = "")
        if i < 2 :
            print(', ', end = "")
    print('}', end = "")

with open(r"STL_output\fsion\Body1.stl", "r") as input :
    arr = (i.strip() for i in input.readlines())

arr = list(arr)
for i in range(1, len(arr) - 1, 7):
    normal = arr[i].split(" ")[2:]
    coordinates = []
    for j in range(3):
        coordinates.append(arr[i + 2 + j].split(" ")[3:])

    print("{")
    print_vectr(normal)
    print(", \n    {")
    for h in range(len(coordinates)):
        print('    ', end = "")
        print_vectr(coordinates[h])
        if h < 2 :
            print(', ')

    if i < len(arr)-8:
        print('\n    }\n},')
    else:
        print('\n    }\n}')
    
