#!/bin/bash

# Compress the data files
files=$(ls out/compressed/*.huff)

echo " [*] Data files to be decompressed:"
for file in $files
do
    echo "  - $file"
done

decompressed_counter=0

echo
echo " [*] Decompressing data files..."
for file in $files
do
    echo
    echo " - Deompressing $file... "
    ./bin/huffman.out -d "$file" > /dev/null
    if [ $? -ne 0 ]; then
        echo "   ! ERROR: Decompression failed for $file"
    else
        echo "   + OK: Decompression successful for $file"
        decompressed_counter=$((decompressed_counter+1))
    fi
done

echo
echo " [*] Decompression summary:"
echo "  - Total files decompressed: $decompressed_counter / $(ls out/compressed/*.huff | wc -l)"

exit 0