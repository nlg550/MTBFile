/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
**************************************************************************/

#ifndef COMPATIBILITY_H_
#define COMPATIBILITY_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	enum MTBMatrixType
	{
		kGeneralDense = 0x01,		//!< General dense matrices
		kSymmetricDense = 0x02,		//!< Symmetric dense matrices
		kGeneralSparse = 0x11,	//!< General sparse matrices
		kSymmetricSparse = 0x12		//!< Symmetric sparse matrices
	};

	enum MTBDatatype
	{
		kPattern = 0x00,		//!< The non-zero entries are always equal to 1
		kInteger = 0x10,		//!< Integer
		kReal = 0x20,		//!< Floating-point datatype
		kComplex = 0x30	  //!< Complex always uses floating-point datatype for the complex and real parts
	};

	typedef struct
	{
		ptrdiff_t row;
		ptrdiff_t col;
		float val;
	} triplet_sp_t;

	typedef struct
	{
		ptrdiff_t row;
		ptrdiff_t col;
		double val;
	} triplet_dp_t;

	typedef struct
	{
		ptrdiff_t row;
		ptrdiff_t col;
		int val;
	} triplet_int_t;

	void read_mtb_int(char filename[64], char *mat_type, char *datatype, char *type_size,
	                  uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_int_t **array);

	void read_mtb_sp(char filename[64], char *mat_type, char *datatype, char *type_size,
	                 uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_sp_t **array);

	void read_mtb_dp(char filename[64], char *mat_type, char *datatype, char *type_size,
	                 uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_dp_t **array);

#ifdef __cplusplus
}
#endif

#endif /* COMPATIBILITY_H_ */
