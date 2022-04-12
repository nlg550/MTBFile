# MTB Library
Binary File Format for sparse matrices. This library includes C/C++ routines for reading/writing files in this format as well as a program to convert MTX ([Matrix Market](https://math.nist.gov/MatrixMarket/formats.html)) file to the MTB file format. There are also some routines for reading files in MTX file format. Only the `coordinate` format of the MTX files are supported.

## MTB File Format

The MTB file is composed by an header with the matrix properties, followed by the (non-zero) entries in the matrix.

![MTBFile](https://user-images.githubusercontent.com/38725499/163049631-4ea34dec-e559-4487-bb5d-23b7a8355606.png)

#### Matrix Type

The first byte in the header indicates the matrix type and includes information about its symmetry. Here *general matrices* only indicates that the file format do not take advantage of any symmetry properties. For symmetric matrices, the file will **only** store the entries at or below the diagonal. 

Types:

```
General Sparse Matrices: 0x11
Symmetric Sparse Matrices: 0x12
```

#### Datatype

The second byte indicates the datatype of each entry. The first 4 bits corresponds to the class of datatype, while the other 4 bits, to its size (in bytes). For example, a 32-bit floating-point (i.e., a real) number is codified as `0x24`. In the *pattern* datatype, the file *only* indicates the position of the non-zero entries (we assume that their value is always equal to `1`). In the *complex* datatype, the first value corresponds to the real part of the complex number, while the second, to its imaginary part.

Datatypes Classes: 

```
Pattern: 0x00
Integer: 0x1_
Real: 0x2_
Complex: 0x3_
```

#### Matrix Parameters

The last three blocks in the header correspond to the number of rows, number of cols and number of non-zeros entries in the matrix, respectively. All matrix parameters are in 64-bits.

#### Matrix Entries

Each non-zero entry in the sparse matrix is represented by a triplet containing its row index, column index and value. The type and size of the value are determined by the *Datatype* field in the header. 

## Usage

The MTB library only requires an compiler that supports C++17 (e.g., GNU Compiler v8.0+ and LLVM/Clang v6.0+). Use `make lib` to create a static library (`libmtb.a`) and `make converter` to compile the MTX-to-MTB converter. Alternatively, use `make all` to compile both.

Afterwards, include the desire header (`mtb.hpp` or `mtx.hpp`) in your program and then link it with the `libmtb.a` file during the compilation.

There is a combatibility layer for C programs, but the final linking should always be done with a C++ compiler.

### API

All C++ routines of the MTB library are within the `mtb` namespace. Please check the headers for the full documentation.

Routines in `mtx.hpp`:

```c++
void mtx_read_header(std::ifstream &ifile, std::vector<std::string> &properties, uint64_t &nrows, uint64_t &ncols, uint64_t &nz);

template<typename T>
void mtx_read_data(std::ifstream &ifile, Triplet<T> *array, uint64_t *size, uint64_t nz, bool is_weighted, bool is_symmetric);

void mtx_to_mtb(std::string mtx_file, std::string mtb_file, bool sort_data);
```

Routines in `mtb.hpp`:

```c++
void mtb_read_header(std::ifstream &ifile, char &mat_type, char &datatype, char &type_size, uint64_t &nrows, uint64_t &ncols, uint64_t &nz);

void mtb_write_header(std::ofstream &ofile, char mat_type, char datatype, char type_size, uint64_t nrows, uint64_t ncols, uint64_t nz);

template<typename T>
void mtb_read_data(std::ifstream &ifile, Triplet<T> *data, uint64_t nz, char mat_type, char datatype, char type_size);

template<typename T>
void mtb_write_data(std::ofstream &ofile, Triplet<T> *data, uint64_t nz, char mat_type, char datatype, char type_size);
```

Routines in `compatibility.h`:

```c++
void read_mtb_int(char filename[64], char *mat_type, char *datatype, char *type_size, uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_int_t **array);
void read_mtb_sp(char filename[64], char *mat_type, char *datatype, char *type_size, uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_sp_t **array);
void read_mtb_dp(char filename[64], char *mat_type, char *datatype, char *type_size, uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_dp_t **array);
```

### MTX-to-MTB Converter

Run the converter as follows:

```
./converter <MTX filename> <MTB filename> <sort the data? (0 or 1)>
```

### Example

Compile and run the example code as follows:

```
cd example/
gcc -c example.c -o example.o -O2
g++ example.o -o example -L../ -lmtb -O2
./example
```

## TODO

- Add support for dense matrices.
- Add option for different index size.
- Add other symmetries such as Hermitian.
- Test complex datatypes.
