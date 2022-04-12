/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
**************************************************************************/

#ifndef _MTB_COMPATIBILITY_H_
#define _MTB_COMPATIBILITY_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	//! Matrix types with information about symmetry. TODO: Add dense matrix support.
	enum MTBMatrixType
	{
		kGeneralDense = 0x01,		//!< General dense matrices
		kSymmetricDense = 0x02,		//!< Symmetric dense matrices
		kGeneralSparse = 0x11,		//!< General sparse matrices
		kSymmetricSparse = 0x12		//!< Symmetric sparse matrices
	};

	//! Datatype of the non-zero entries in the matrix
	enum MTBDatatype
	{
		kPattern = 0x00,		//!< The non-zero entries are always equal to 1
		kInteger = 0x10,		//!< Integer
		kReal = 0x20,		//!< Floating-point datatype
		kComplex = 0x30	  //!< Complex always uses floating-point datatype for the complex and real parts
	};

	//! This struct represents a nonzero entry in a sparse matrix.
	typedef struct
	{
		ptrdiff_t row;
		ptrdiff_t col;
		float val;
	} triplet_sp_t;

	//! This struct represents a nonzero entry in a sparse matrix.
	typedef struct
	{
		ptrdiff_t row;
		ptrdiff_t col;
		double val;
	} triplet_dp_t;

	//! This struct represents a nonzero entry in a sparse matrix.
	typedef struct
	{
		ptrdiff_t row;
		ptrdiff_t col;
		int val;
	} triplet_int_t;

	//! Reads and parses a MTB file. This routine will allocate the
	//! nescessary memory space for the `array`.
	//!
	//! @param filename[in]		name of MTB file
	//! @param mat_type[out]	matrix type (@ref MTBMatrixType)
	//! @param datatype[out]	datatype (@ref MTBDatatype)
	//! @param type_size[out]	size of the data type (in bytes)
	//! @param nrows[out]		number of rows
	//! @param ncols[out]		number of columns
	//! @param nz[out]			number of nonzero entries
	//! @param array[out]		triplet array containing the entries of the matrix
	void read_mtb_int(char filename[64], char *mat_type, char *datatype, char *type_size,
	                  uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_int_t **array);

	//! Reads and parses a MTB file. This routine will allocate the
	//! nescessary memory space for the `array`.
	//!
	//! @param filename[in]		name of MTB file
	//! @param mat_type[out]	matrix type (@ref MTBMatrixType)
	//! @param datatype[out]	datatype (@ref MTBDatatype)
	//! @param type_size[out]	size of the data type (in bytes)
	//! @param nrows[out]		number of rows
	//! @param ncols[out]		number of columns
	//! @param nz[out]			number of nonzero entries
	//! @param array[out]		triplet array containing the entries of the matrix
	void read_mtb_sp(char filename[64], char *mat_type, char *datatype, char *type_size,
	                 uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_sp_t **array);

	//! Reads and parses a MTB file. This routine will allocate the
	//! nescessary memory space for the `array`.
	//!
	//! @param filename[in]		name of MTB file
	//! @param mat_type[out]	matrix type (@ref MTBMatrixType)
	//! @param datatype[out]	datatype (@ref MTBDatatype)
	//! @param type_size[out]	size of the data type (in bytes)
	//! @param nrows[out]		number of rows
	//! @param ncols[out]		number of columns
	//! @param nz[out]			number of nonzero entries
	//! @param array[out]		triplet array containing the entries of the matrix
	void read_mtb_dp(char filename[64], char *mat_type, char *datatype, char *type_size,
	                 uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_dp_t **array);

#ifdef __cplusplus
}
#endif

#endif /* _MTB_COMPATIBILITY_H_ */
