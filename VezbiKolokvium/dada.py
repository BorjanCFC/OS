import sys

if len(sys.argv) < 2:
    print("Nedovolno argumenti")
    sys.exit()

flag = 0
dat = sys.argv[1]

if len(sys.argv) > 2:
    if len(sys.argv[2].split("-")) == 3:
        flag = 1

# Generate output file name with "_out" suffix
output_file = dat.split(".")[0] + "_out." + dat.split(".")[-1]

# Open the input file for reading and the output file for writing
with open(dat, "r") as d, open(output_file, "w") as out:
    pole = []
    for l in d.readlines():
        l = l.strip()  # Remove trailing spaces

        zborovi = l.split(" ")  # Get words in the line
        line = ""  # We'll append words that aren't dates here
        p = []  # Store the dates

        # Process the words and separate dates from text
        for z in zborovi:
            if len(z.split("-")) == 3:  # Found a date
                p.append(z)
            else:
                line = line + " " + z  # Append non-date words

        # For each date found, write it with the associated line
        for z in p:
            out.write(z + " " + line + "\n")

        # If there's a specific date argument, check and print
        if flag == 1:
            if sys.argv[2] in p:
                pole.append(line)

    # If a date argument was provided, print those lines with the date
    if flag == 1:
        for z in pole:
            out.write(sys.argv[2] + " " + z + "\n")

