import re

text = input("Text: ")

words = re.findall(" ", text)
words = len(words) + 1

sentences = re.findall(r"\.", text)
sentences += re.findall(r"\?", text)
sentences = len(sentences)

letters = re.findall(r"\w", text)
letters = len(letters)


l = letters / (words / 100)
s = sentences / (words / 100)

grade = round(0.0588 * l - 0.296 * s - 15.8)

if grade < 1:
    print("Before Grade 1")
elif grade > 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")
