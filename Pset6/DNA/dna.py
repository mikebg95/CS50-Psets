from sys import argv
import csv

# ensure correct usage
if len(argv) != 3:
    print("usage: ./dna database.csv sequence.txt")

# open files
database = csv.reader(open(argv[1], "r"))
sequence = open(argv[2], "r").read()

# check header of csv file
csv_header = next(database)

# create lists for str's and their counts
strs = []
str_counts = []

# add strs to list (exclude "name")
for word in csv_header:
    if word == "name":
        continue
    strs.append(word)

# count highest subsequent occurences of str and add to list
for str in strs:
    counts = []
    temp_count = 0
    for i in range(len(sequence)):
        if sequence[i: i + len(str)] == str and sequence[i - len(str): i] == str:
            temp_count += 1
        if sequence[i: i + len(str)] != str and sequence[i - len(str): i] == str:
            counts.append(temp_count + 1)
            temp_count = 0
    count = max(counts) if counts else 0
    str_counts.append(count)

# add personal dna data to list
for line in database:
    person_strs = []
    for word in line:
        person_strs.append(word)

    # check if personal dna data aligns with sequences found
    for i in range(len(str_counts)):

        # if not aligned, break and move to next person
        if (int(person_strs[i+1]) != int(str_counts[i])):
            break

        # if all sequences align
        else:
            if i == len(str_counts) - 1:
                print(person_strs[0])
                exit(0)

# if no match found
print("No match")
exit(1)