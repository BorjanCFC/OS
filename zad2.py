import sys
from collections import defaultdict

def print_dates(file_path, flag):
    with open(file_path, 'r') as file:
        data = [line.strip().split() for line in file.readlines() if len(line.strip().split()) >= 8]

    dates = defaultdict(list)
    for line in data:
        date = ' '.join(line[5:8])
        item = ' '.join(line[8:])
        dates[date].append(item)

    if flag == '-d':
        for date, items in dates.items():
            print(f"Datum: {date}")
            print('\n'.join(items))
            print()
    elif flag == '-f':
        for date, items in dates:
            print(f"Datum: {date} - {len(dates[date])} ставки")
            for item in dates[date]:
                print(item)
            print()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Upotreba: python script.py <pateka_na_datoteka> <-d|-f>")
        sys.exit(1)
    print_dates(sys.argv[1], sys.argv[2])
