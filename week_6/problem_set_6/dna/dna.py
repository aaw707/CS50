import csv
from sys import argv
from sys import exit

# script takes 2 command-line arguments
if (len(argv) != 3):
    print("Usage: python dna.py data.csv sequence.txt")
    exit()


def main():

    # first command-line argument: a csv file containing the STR counts
    count_filename = argv[1]
    # second command-line argument: a csv file containing DNA sequence to identify
    id_filename = argv[2]

    # save the STR counts in a list
    db_counts = []
    with open(count_filename, "r") as count_file:
        reader = csv.DictReader(count_file)
        for row in reader:
            db_counts.append(row)

    # list the STRs to count
    STRs = list(db_counts[0].keys())[1:]

    # save the dna in file into a string
    dna = ""
    with open(id_filename, "r") as id_file:
        reader = csv.reader(id_file)
        for row in reader:
            dna = row[0]
            # print(dna)

    # count the STRs in the given dna
    # e.g. dna_count = {"AGATC": 3, "AATG": 0, "TATC": 5}
    dna_counts = {}

    for STR in STRs:
        #print("STR:", STR)
        # set the counter for maximum repetition of a STR
        max_rep = 0
        current_max = 0

        dna_copy = dna
        STR_length = len(STR)
        starting_idx = dna_copy.find(STR)
        # keep counting the continuous repetition of the STR until the end of dna
        while starting_idx > -1:

            #print("starting_idx:", starting_idx)

            current_max = count_continuous_STR(STR, dna_copy[starting_idx:])
            #print("current_max:", current_max)
            if current_max > max_rep:
                max_rep = current_max

            dna_copy = dna_copy[starting_idx + current_max * STR_length:]
            starting_idx = dna_copy.find(STR)

        # save the max repetition into the count dict
        dna_counts[STR] = max_rep

    # find the same occurence of the target dna in the database
    #print("target_dna_counts:", dna_counts)
    #print("STRs:", STRs)
    # loop each person in the database
    for counts_record in db_counts:
        # print(counts_record)
        # set a counter for matching STRs
        match = 0

        # loop each STR for this person
        for STR in STRs:

            # compare this person's STR count with the target dna
            # if not an exact match, skip to the next person
            if int(counts_record[STR]) == dna_counts[STR]:
                match += 1
                #print("match:", match)
            else:
                break

        # if all STR counts match
        if match == len(STRs):
            print(counts_record['name'])
            return
        else:
            continue

    # if no person matches the target dna
    print("No match")
    return


def count_continuous_STR(STR, dna):
    # the beginning of the dna must be STR
    STR_length = len(STR)
    count = 0

    while True:
        # print(dna)
        # count the number of continuous occurence
        count += 1
        # print(count)
        # truncate the first occurence
        dna = dna[STR_length:]

        # STR is no longer continuous
        if dna[:STR_length] != STR:
            break

    return count


# calling the main function
main()