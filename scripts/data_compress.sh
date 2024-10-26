#!/bin/bash

# Compress the data files
files=$(ls data/*.txt)

echo " [*] Data files to be compressed:"
for file in $files
do
    echo "  - $file"
done

compressed_counter=0

echo
echo " [*] Compressing data files..."
for file in $files
do
    echo
    echo " - Compressing $file... "
    ./bin/huffman.out -c "$file" > /dev/null
    if [ $? -ne 0 ]; then
        echo "   ! ERROR: Compression failed for $file"
    else
        echo "   + OK: Compression successful for $file"
        compressed_counter=$((compressed_counter+1))
    fi
done

echo
echo " [*] Compression summary:"
echo "  - Total files compressed: $compressed_counter / $(ls data/*.txt | wc -l)"

exit 0