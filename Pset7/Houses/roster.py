from sys import argv
import csv
from cs50 import SQL

# ensure proper usage
if len(argv) != 2:
    print("usage: python roster.py House")
    exit(1)

# save house in variable
house = argv[1]

db = SQL("sqlite:///students.db")

# get dict of people from house
people = db.execute("SELECT * FROM students WHERE house=? ORDER BY last, first", house)
for person in people:
    first = person.get("first")
    middle = person.get("middle")
    last = person.get("last")
    birth = person.get("birth")

    # print names to user
    if middle == None:
        print(first, last + ", born " + str(birth))
    else:
        print(first, middle, last + ", born " + str(birth))
