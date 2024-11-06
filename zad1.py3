import sys

def reverse_word_order(file_paths):
    if len(file_paths) < 2:
        print("Error: Script must be called with at least two file path arguments.")
        return

    for file_path in file_paths:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        modified_lines = []
        for line in lines:
            words = line.split()
            reversed_words = [words[-i-1] for i in range(len(words))]
            modified_line = ' '.join(reversed_words)
            modified_lines.append(modified_line + '\n')

        with open(file_path, 'w') as file:
            file.writelines(modified_lines)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Error: Script must be called with at least two file path arguments.")
        sys.exit(1)

    reverse_word_order(sys.argv[1:])