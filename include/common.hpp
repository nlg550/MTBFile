/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MOCCA library, which is licensed under the
 terms contained in the LICENSE file.
 **************************************************************************/

#ifndef _MTB_COMMON_HPP_
#define _MTB_COMMON_HPP_

#include <cstddef>
#include <cstdint>
#include <complex.h>
#include <type_traits>

#define MTB_BUF_SIZE (1 << 24)

namespace mtb
{
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

	//! The @ref Triplet represent a nonzero entry in a sparse matrix.
	//! **Template Parameters:**
	//! - ``T`` - Type of nonzero value.
	template<typename T>
	struct Triplet
	{
		std::ptrdiff_t row;
		std::ptrdiff_t col;
		T val;
	};

}   // namespace mtb

#endif /* _MTB_COMMON_HPP_ */
