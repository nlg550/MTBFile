/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
**************************************************************************/

#ifndef _MTX_HPP_
#define _MTX_HPP_

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "mtb_def.hpp"
#include "progress_bar.hpp"

namespace mtb
{

	//! Reads and parses the header of a MTX (Matrix Market) file. Only the "coordinate"
	//! format is supported.
	//!
	//! @param ifile[inout]			input file stream to the MTX file
	//! @param properties[out]		matrix properties (first line of the MTX file)
	//! @param nrows[out]			number of rows
	//! @param ncols[out]			number of columns
	//! @param nz[out]				number of nonzero entries
	//!
	//! @exception std::runtime_error if the header format is incorrect.
	void mtx_read_header(std::ifstream &ifile, std::vector<std::string> &properties,
	                     uint64_t &nrows, uint64_t &ncols, uint64_t &nz);

	//! Reads and parses the matrix entries of a MTX file. The entries are then
	//! stored in a @ref Triplet array.
	//!
	//! @param ifile[inout]			input file stream to the MTX file
	//! @param array[out]			triplet array containing the entries of the matrix
	//! @param size[out]			the size of the triplet array
	//! @param nz[in]				number of non-zeros entries
	//! @param is_weighted[in]		non-zero entries have a value or not
	//! @param is_symmetric[in]		the matrix is symmetric or not
	template<typename T>
	void mtx_read_data(std::ifstream &ifile, Triplet<T> *array, uint64_t *size, uint64_t nz,
	                   bool is_weighted, bool is_symmetric)
	{
		// Read buffer
		std::unique_ptr<char[]> input(new char[MTB_BUF_SIZE + 1]);

		ProgressBar bar(60);
		bar.init("Importing data from MTX...");

		while (ifile)
		{
			char *ptr = input.get();

			// Read a large data block from the file
			ifile.read(ptr, MTB_BUF_SIZE);

			if (ifile.gcount() > 0)
			{
				char *input_end = ptr + ifile.gcount();
				*input_end = '\0';

				// Find the end of the last complete line
				char *last = std::strrchr(ptr, '\n');

				if (last)
				{
					// If the last line is truncated, rewind file pointer
					// to the beggining of this line, so it can be read again
					// in the next block.
					ifile.seekg(last - input_end, ifile.cur);
					std::fill(last + 1, input_end, '\0');

					// Parse each line into triplets and then place them in the write buffer.
					// TODO: Replace strtol and strtod with std::from_char for better performance
					while (ptr < last)
					{
						Triplet<T> triplet;
						triplet.row = strtol(ptr, &ptr, 10) - 1;
						triplet.col = strtol(++ptr, &ptr, 10) - 1;

						if (!is_weighted)
						{
							triplet.val = 1;

						} else
						{
							if constexpr (std::is_integral_v<T>) triplet.val = strtol(++ptr, &ptr, 10);
							else if constexpr (std::is_floating_point_v<T>) triplet.val = strtod(++ptr, &ptr);
							else
							{
								typename T::value_type imag, real;

								real = strtod(++ptr, &ptr);
								imag = strtod(++ptr, &ptr);
								triplet.val.real(real);
								triplet.val.imag(imag);
							}
						}

						array[(*size)++] = triplet;

						if (is_symmetric)
						{
							std::swap(triplet.row, triplet.col);
							array[(*size)++] = triplet;
						}

						++ptr;
					}

					bar.set((float) *size / nz);
				}
			}
		}

		bar.finish();
	}

	//! Converts a MTX file to a MTB file. If `sort_data == true`, sort the data
	//! in a row-major format (first by row index, then by column index) before
	//! writing the data to the MTB file. This sorting requires that the entire
	//! matrix is loaded in memory. If `sort_data == false`, the memory footprint
	//! of this routine is very small.
	//!
	//! @param mtx_file[in]		MTX file name
	//! @param mtb_file[in]		MTB file name
	//! @param sort_data[in]	sort the data before writing to MTB file
	//!
	//! @exception std::runtime_error if this routine encounters some error (e.g.,
	//! wrong MTX format, unsupported matrix types, etc.).
	void mtx_to_mtb(std::string mtx_file, std::string mtb_file, bool sort_data);
}   // namespace mtb

#endif /* _MTX_HPP_ */
