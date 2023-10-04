# Huffman-Encoding
Lossless Compression via the Huffman Encoding algorithm.

 Creates ".hi" (huffman information) files for a dataset which creates a file that you can use to decode whichever file you want -- file entails what sequence of bits represents each letter .
 This ".hi" file is used to compress and decompress your datasets -- in other words, if you mix and match your dataset and .hi files, you will most likely get a different dataset than what it originally was.
 Displays Compression ratio, space saving (%), and the number of characters in the original file vs the compressed file.

 Compressed files have an added ".hc" extension added to the original file name.

To begin, make and execute target.


Example on how to use:
```
  1 <filename> - create a new Huffman Information file from an original file
  2 <filename> - load a Huffman Information file
  3 <filename> - compress a file using the current Huffman Information file
  4 <filename> - decompress a file using the current Huffman Information file
  5            - quit the program
```
  ```
  cmd>  1 somefile.txt
  .hi file built
  cmd> 2 somefile.txt.hi
  loaded .hi file
  cmd> 3 somefile.txt
  Number of characters in original file: 1032469
  Number of bytes in compressed file: 631391
  Compression Ratio: 1.63523
  Space Saving (%): 38.84649
  cmd> 4 somefile.txt.hc
  decompressed
  cmd> 5
```

  As a note, the decompressed file is stored back into the original file.
