# MTBFile
Binary File Format for sparse matrices. This library includes C/C++ routines for reading/writing files in this format as well as a program to convert MTX ([Matrix Market](https://math.nist.gov/MatrixMarket/formats.html)) file to the MTB file format. 

## MTB File Format

The MTB file is composed by an header with the matrix properties, followed by the (non-zero) entries in the matrix.

### Header
![header](https://user-images.githubusercontent.com/38725499/162941435-cec2a15f-c819-4a73-88ac-d744f2c8de7f.png)

#### Matrix Type

The first byte in the file indicate the matrix type, including information about its symmetry. Here *general matrices* only indicates that the file format do not take advantage of any symmetry properties. For symmetric matrices, the file format **only** needs to store the entries at or below the diagonal. 

Codes:

```
General Sparse Matrices: 0x11
Symmetric Sparse Matrices: 0x12

```

#### Data Types

The second byte indicate the datatype of the matrix entries. The first 4-bit correspond to the class of datatype, while the last 4-bits, the size of the datatype (in bytes). For example, a 32-bit floating-point number is codified as `0x24`. In the *pattern* datatype, the file only indicate the position of the non-zero entries (we assume that their value is always equal to `1`). In the *complex* datatype, the first value correspond to the real part, while the second, to the imaginary part of the complex number.

```
Pattern: 0x00
Integer: 0x1X
Real: 0x2X
Complex: 0x3X 

```
#### Matrix Parameters

The last three blocks in the header correspond to the number of rows, number of cols and number of non-zeros entries in the matrix, respectively. 

### Matrix Entries

![nonzero_entry](https://user-images.githubusercontent.com/38725499/162927986-a489a093-8057-4f55-a19a-93d9f3402252.png)

Each non-zero entry in the sparse matrix is represented by a triplet containing its row index, column index and value. The number of bytes of the value depends on the *Datatype* field in the header.

## Compilation

The MTB library only requires an compiler with support for C++17 (e.g., GNU Compiler v8.0+ and LLVM/Clang v6.0+). There is a combatility layer for C programs, but the final linking should be done with a C++ compiler. 

## MTX-to-MTB Converter

## TODO