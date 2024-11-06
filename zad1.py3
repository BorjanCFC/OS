#!/usr/bin/python3
import sys

def reverseWords(files):
    if len(files) < 2:
        print("Nedovolno argumenti!")
        return

    for file_path in files:
        with open(files, 'r') as file:
            lines = [line.strip().split() for line in file.readlines()]

        modified_lines = [' '.join(reversed(words)) for words in lines]

        with open(file_path, 'w') as file:
            file.write('\n'.join(modified_lines))

if __name__ == "__main__":
    
    reverseWords(sys.argv[1:])
