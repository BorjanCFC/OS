import sys
from collections import defaultdict

def print_creation_modification_dates(file_path, flag):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    dates = defaultdict(list)
    for line in lines:
        parts = line.strip().split()
        if len(parts) >= 8:
            date = ' '.join(parts[5:8])
            item = ' '.join(parts[8:])
            dates[date].append(item)

    if flag == '-d':
        for date, items in dates.items():
            print(f"Datum: {date}")
            print('\n'.join(items))
            print()
    elif flag == '-f':
        for date, items in dates.items():
            for item in items:
                print(f"{item} - {date}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Upotreba: python script.py <putanja_datoteke> <-d|-f>")
        sys.exit(1)

    file_path = sys.argv[1]
    flag = sys.argv[2]
    print_creation_modification_dates(file_path, flag)
