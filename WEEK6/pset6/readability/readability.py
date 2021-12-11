from cs50 import get_string

def display(index) :
    if index < 1 :
        print("Before Grade 1")
    elif index > 16 :
        print("Grade 16+")
    else :
        print(f"Grade {index}")

def Coleman_Liau_formula(text) :
    text1 = text.split()
    sentences = 0
    words = len(text1)
    letters = 0
    for word in text1 :
        if word[-1] in ["!", "?", "."] :
            sentences += 1
            letters += len(word[:-1])
        else :
            for c in word :
                if c.isalpha() :
                    letters += 1
    index = 0.0588 * (letters/(words/100)) - 0.296 * (sentences/(words/100)) - 15.8
    return round(index)

def main() :
    text = get_string("Text: ")
    index = Coleman_Liau_formula(text)
    display(index)

main()