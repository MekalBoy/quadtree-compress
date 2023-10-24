
# Quadtree Image Compressor

An app made in a week as part of a learning course on Data Structures and Algorithms, making use of quadtrees and queues to encode and significantly compress .ppm images, depending on the factor provided. The factor determines the strength of the compression.

See how to build and use it yourself below.

```
$ make
gcc -c main.c -Wall -g -o main.o
gcc -c tree.c -Wall -g -o tree.o
gcc -c queue.c -Wall -g -o queue.o
gcc main.o tree.o queue.o -Wall -g -o quadtree

$ ls
main.c  main.o  Makefile  quadtree  queue.c  queue.h  queue.o  tree.c  tree.h  tree.o

$ ./quadtree
Correct usage:
./quadtree [-c1 factor | -c2 factor | -d] [input_file] [output_file]
```

## Options
Using the `-c1` option will allow you to see how many layers the underlying quadtree will have after it is formed, along with the total number of chunks the image will be composed of as well as the largest chunk size. All of this is written out in `output_file`.
```
$ ./quadtree -c1 2000 example.ppm details.out
$ cat epicoeee.out
2
4
128
```

Using the `-c2` option will do the compressing and encoding of the `input_file`, placing the output in the `output_file`.

Using the `-d` option will do the decompression of the encoded `input_file` and place the contents in the `output_file`. Make sure its extension is `.ppm`!

```
$ ./quadtree -d compressed_file.out decompressed.ppm
```
Now you can open the file in your favorite image viewer!

## Extra

[PPM Format Specification](https://netpbm.sourceforge.net/doc/ppm.html)
