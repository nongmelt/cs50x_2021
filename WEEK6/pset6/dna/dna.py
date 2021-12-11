from sys import argv, exit
import csv

def STR_count(seq, STR) :
    length = len(STR)
    consecutive = STR
    while seq.find(consecutive) != -1:
        consecutive += STR
    return int(len(consecutive)/length) - 1

def match(db, STRs_count) :
    for person in db :
        if person[1] == STRs_count :
            return person[0]
    return "No match"

def main() :
    if len(argv) != 3 :
        exit("Usage: python dna.py data.csv sequence.txt")
    db = []
    with open(argv[1]) as csvfile :
        db_reader = csv.reader(csvfile)
        STRs = next(db_reader)[1:]
        for db_row in db_reader :
            db.append([db_row[0], "".join(db_row[1:])])
    with open(argv[2]) as textfile :
        seq_reader = csv.reader(textfile)
        seq = next(seq_reader)[0]

    STRs_count = ""
    for STR in STRs :
        STRs_count += str(STR_count(seq, STR))
    print(match(db, STRs_count))
main()