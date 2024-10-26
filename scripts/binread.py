import sys
from typing import NamedTuple

def read_file_in_bits(file_path):
    n = 0


    with open(file_path, 'rb') as f:
        byte = f.read(1)
        while byte:
            # Zamiana bajtu na reprezentację binarną
            bits = bin(ord(byte))[2:].zfill(8)
            print(bits, end=' ')
            
            n += 1
            if n % 2 == 0:
                print(' ', end='')
            if n % 4 == 0:
                print()

            byte = f.read(1)


print()
print()
print(' -- BinRead --')
print()

if len(sys.argv) != 2:
    print("Usage: python binread.py <file_path>")
    sys.exit(1)

file_path = sys.argv[1]

print(f"Reading file <{file_path}> in bits:\n")
read_file_in_bits(file_path)
print()