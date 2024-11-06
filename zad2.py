import sys

def print_creation_modification_dates(file_path, flag):
    with open(file_path, 'r') as file:
        lines = file.readlines()

        new_lines = []
        dates = {}

    for line in lines:
        line = line.split()
        new_lines.append(line)

        if line[5] not in dates:
            dates[lines[5]] = 0


    if flag == '-d':
        for line in new_lines:
            print(line[7] + " : " + line[5])


    elif flag == '-f':
        for line in new_lines:
            dates[line[5]] += 1





if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Upotreba: python script.py <putanja_datoteke> <-d|-f>")
        sys.exit(1)

    file_path = sys.argv[1]
    flag = sys.argv[2]
    print_creation_modification_dates(file_path, flag)
