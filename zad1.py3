import sys

def reverse_word_order(file_paths):
    if len(file_paths) < 2:
        print("Error: Script must be called with at least two file path arguments.")
        return

    for file_path in file_paths:
        with open(file_path, 'r') as file:
            lines = [line.strip().split() for line in file.readlines()]

        modified_lines = [' '.join(reversed(words)) for words in lines]

        with open(file_path, 'w') as file:
            file.write('\n'.join(modified_lines))

if __name__ == "__main__":
    reverse_word_order(sys.argv[1:])
