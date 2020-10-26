from sys import argv
import csv
from cs50 import SQL

# ensure proper usage
if len(argv) != 2:
    print("usage: python import.py characters.csv")
    exit(1)

# open file and skip first line
csv_database = csv.reader(open(argv[1], "r"))
next(csv_database)

db = SQL("sqlite:///students.db")

# iterate over lines, save info in variables
for line in csv_database:
    name = line[0]
    house = line[1]
    birth = line[2]

    # check if person has middle name
    names = name.split()
    middle_name = False if name.count(" ") == 1 else True

    # save people to database
    if middle_name:
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                   names[0], names[1], names[2], house, birth)
    else:
        db.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)",
                   names[0], names[1], house, birth)