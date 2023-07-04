# Quadtree-homework
This assignment aims to introduce you to the vast world of images and proposes an analysis and implementation method for image compression. The term "image compression" refers to a wide range of techniques and methods whose goal is to represent a given image using the fewest possible number of bits. With the advancement of technology and the subsequent increase in image quality, the need to reduce the amount of information required to represent an image has become evident.

Requirement 1

Firstly, we will build the compression tree and determine some statistics based on it. Therefore, we will start by reading the image from the PPM file and then construct the compression tree for it.

Important !
The tree implementation will be done using pointers.

To determine when a block can be represented in the quadtree compression tree as a leaf node, meaning it doesn't need to be further divided into four equally-sized regions, we will calculate the average color of the block. This involves finding the arithmetic mean of the channel values (RED, GREEN, and BLUE) within the pixel submatrix corresponding to the block. After calculating the average color, we compute a similarity score (mean) for the block.

Once the compression tree has been built, we want to determine the following information:

- The number of levels in the quadtree.
- The number of blocks in the image for which the similarity score of the pixels is less than or equal to the provided factor.
- The side length of the square for the largest undivided area in the image.

Important !
These pieces of information will be written to a text file, with each item on a separate line. The filename is provided as an argument in the command line.

Requirement 2

For the second requirement of the assignment, you need to compress an image in PPM format using the compression algorithm detailed in the previous section. It is mandatory to implement the quadtree compression tree for this task.

To construct the compression tree, you will use the method described in the previous requirement.

Once the compression tree is built, it is traversed level by level, and the compression file is generated. The compressed file will contain the information resulting from the compression process:

- (image_size) - an unsigned int that specifies the size of the image (remember that we have square images).

For each node in the level-by-level traversal of the compression tree, the compressed file will include the following information:

If the node is an internal node: (node_type) - an unsigned char that will have the value 0 in this case.

If the node is a leaf node:

- (node_type) - an unsigned char that will have the value 1 in this case.

- (red_value) - an unsigned char indicating the value of the Red component responsible for the color of the pixels in the area described by the node.

- (green_value) - an unsigned char indicating the value of the Green component responsible for the color of the pixels in the area described by the node.

- (blue_value) - an unsigned char indicating the value of the Blue component responsible for the color of the pixels in the area described by the node.


Your program will receive command-line arguments as follows:
./quadtree [-c1 factor | -c2 factor] [input_file] [output_file], where:

-c1 factor indicates that the program will solve requirement 1 (factor represents the threshold for the compression tree).

-c2 factor indicates that the program will solve requirement 2 (factor represents the threshold for the compression tree).

-input_file represents the name of the input file that contains the image.

-output_file represents the name of the output file where the program's result will be written, depending on the received command.




For the implementation, I used two files: tema2.c (where I performed the input and the task) and tema2.h (where I defined the functions used in tema2.c).

In tema1.c, we have the main function:
If we have 5 arguments, it means we are dealing with requirements 1 or 2. In this case, we read a ppm file (specifically, the type, image dimensions, and color maximum value) and create the pixel matrix (in .h, I defined a Pixel type that holds the color values for each pixel). Memory is allocated for the matrix, and we read the RGB values for each corresponding pixel. Once the matrix is created, the createTree function is called (with the arguments being the matrix size, the starting point for mean analysis, the pixel matrix, and the factor).

createTree recursively creates the tree by allocating memory for a cell (for a Tree element ~struct tree*, which stores the node type, a pointer to a block representing the region defined by a node, i.e., the first element from the pixel matrix where the desired region starts, the region's size, and the color means for that region, and an array of children). The color means are then calculated, and based on the mean and median, the node is assigned a type of 0 or 1. Memory is allocated for the array of children, and if the type is 0, it means the node is not a leaf, and the region will be divided. Therefore, the block receives NULL, and each "child" is created recursively by calling createTree(size/2 - the region is divided into 4 ~width divided by 2, and the corresponding indices for dividing the matrix into 4, the matrix, and the factor). If the type is 1, it means the node is a leaf and represents a compact color region, so it will not be further divided, and the "children" receive NULL. Memory is allocated for the block, which will be initialized with the calculated data. Finally, the function returns the node.

If the argument is c1, it means we are solving requirement 1, so the output file is opened in text mode, and we write the number of levels (equal to 1 + the height calculated using the heightTree function from .h), the number of leaves (calculated using the colorNodes function from .h), and the largest compact region by its size, which is the leaf on the level closest to the root. Therefore, the matrix dimension is divided by 2^minimum_height. The next steps involve freeing the memory for the tree (using freeTree from .h), the matrix, and closing the files.

If we receive c2, requirement 2 is solved, and a binary file is opened to write the dimension. Then, the tree is traversed level by level using the printLevel function, which traverses the tree and prints the type and color means. Finally, the memory is freed as in the case of the first requirement.

The printLevel function takes the tree, the file to write the information to, and the height as arguments and uses a queue.
