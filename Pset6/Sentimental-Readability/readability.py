from cs50 import get_string

# prompt user for text
text = get_string("Please insert text: ")

# get number of sentences and words in text
sentences = text.count('.') + text.count('!') + text.count('?')
words = text.count(' ') + 1

# get number of letters in text
letters = 0
for c in text:
    if c.isalpha():
        letters += 1

# calculate letters and sentences per 100 words
letters_100_words = (100 / words) * letters
sentences_100_words = (100 / words) * sentences

# calculate index
index = round(0.0588 * letters_100_words - 0.296 * sentences_100_words - 15.8)

# print readability grade to user
if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade " + str(index))