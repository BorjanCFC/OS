import sys

def print_creation_modification_dates(file_path, flag):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        new_lines = []
        dates = {}

        # Process each line
        for line in lines:
            line = line.split()  # Splitting the line into columns
            new_lines.append(line)

            if len(line) > 5:  # Ensure there's enough data in the line
                date = line[5]  # Assuming line[5] contains the date
                if date not in dates:
                    dates[date] = 0

        # Processing flags
        if flag == '-d':  # Print the date and associated information
            for line in new_lines:
                if len(line) > 7:  # Ensure there are enough columns
                    print(f"{line[7]} : {line[5]}")

        elif flag == '-f':  # Count occurrences of each date
            for line in new_lines:
                if len(line) > 5:
                    dates[line[5]] += 1

            # Print the date count
            for date, count in dates.items():
                print(f"{date}: {count}")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <file_path> <-d|-f>")
        sys.exit(1)

    file_path = sys.argv[1]
    flag = sys.argv[2]
    print_creation_modification_dates(file_path, flag)
