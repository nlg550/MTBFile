/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
 **************************************************************************/

#include "../include/mtx.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <filesystem>

#include "../include/common.hpp"
#include "../include/mtb.hpp"
#include "../include/progress_bar.hpp"

namespace mtb
{
	/*********************************************************************************************
	 MTX File Handler
	 *********************************************************************************************/

	void mtx_read_header(std::ifstream &ifile, std::vector<std::string> &properties,
	                     uint64_t &nrows,
	                     uint64_t &ncols,
	                     uint64_t &nz)
	{
		int err;
		std::string line, token;

		// Read and parse the file header
		std::getline(ifile, line);

		std::stringstream ss(line);
		properties.reserve(5);
		while (std::getline(ss, token, ' '))
			properties.push_back(token);

		if (properties.size() != 5)
		    throw std::runtime_error("Error: Wrong MTX format!");

		if (properties[0] != "%MatrixMarket" && properties[0] != "%%MatrixMarket")
		    throw std::runtime_error("Error: Wrong MTX format!");

		// Ignore all lines starting with "%" (comments)
		while (ifile.peek() == '%')
			ifile.ignore(2048, '\n');

		// Read matrix parameters
		std::getline(ifile, line);
		err = std::sscanf(line.c_str(), "%ld %ld %ld\n", &nrows, &ncols, &nz);
		if (err != 3) throw std::runtime_error("Error: Wrong MTX format!");
	}

	void mtx_to_mtb(std::string mtx_file, std::string mtb_file)
	{
		std::ifstream ifile(mtx_file, std::fstream::in);
		std::ofstream ofile(mtb_file, std::fstream::binary);
		std::vector<std::string> properties;
		uint64_t nrows, ncols;
		uint64_t nonzeros;

		if (ifile)   // Check if the file is open
		{
			mtx_read_header(ifile, properties, nrows, ncols, nonzeros);

			// Verify the properties of the matrix
			char mat_type, datatype, type_size;

			if (properties[2] == "coordinate" && properties[4] == "general") mat_type = kGeneralSparse;
			else if (properties[2] == "coordinate" && properties[4] == "symmetric") mat_type = kSymmetricSparse;
			else throw std::runtime_error("Error: Unsupported matrix mat_type!");

			if (properties[3] == "pattern") datatype = kPattern;
			else if (properties[3] == "real") datatype = kReal;
			else if (properties[3] == "integer") datatype = kInteger;
			else if (properties[3] == "complex") datatype = kComplex;
			else throw std::runtime_error("Error: Wrong MTX format!");

			if (datatype == kPattern) type_size = 0;
			else if (datatype == kInteger) type_size = sizeof(int);
			else type_size = sizeof(double);

			if (ofile)
			{
				int triplet_size = 2 * sizeof(uint64_t) + type_size;
				std::unique_ptr<char[]> input(new char[MTB_BUF_SIZE + 1]);
				std::unique_ptr<char[]> output(new char[MTB_BUF_SIZE * triplet_size]);

				mtb_write_header(ofile, mat_type, datatype, type_size, nrows, ncols, nonzeros);

				std::ptrdiff_t filesize = std::filesystem::file_size(mtx_file);
				ProgressBar bar(60);
				bar.init("Converting MTX to MTB...");

				while (ifile)
				{
					uint64_t output_size = 0;
					char *ptr = input.get();

					// Read a large data block from the file
					ifile.read(ptr, MTB_BUF_SIZE);

					if (ifile.gcount() > 0)
					{
						char *input_end = ptr + ifile.gcount();
						*input_end = '\0';
						char *last = std::strrchr(ptr, '\n');

						if (last)
						{
							std::fill(last + 1, input_end, '\0');
							ifile.seekg(last - input_end, ifile.cur);

							// Parse each line into triplets and place them into the array
							while (ptr < last)
							{
								uint64_t coord[2];
								coord[0] = strtol(ptr, &ptr, 10) - 1;
								coord[1] = strtol(++ptr, &ptr, 10) - 1;

								std::memcpy(output.get() + output_size, &coord, 2 * sizeof(uint64_t));
								output_size += 2 * sizeof(uint64_t);

								if (datatype == kReal)
								{
									double val = strtod(++ptr, &ptr);
									std::memcpy(output.get() + output_size, &val, type_size);
									output_size += type_size;

								} else if (datatype == kComplex)
								{
									double real = strtod(++ptr, &ptr);
									std::memcpy(output.get() + output_size, &real, type_size);
									output_size += type_size;

									double imag = strtod(++ptr, &ptr);
									std::memcpy(output.get() + output_size, &imag, type_size);
									output_size += type_size;

								} else if (datatype == kInteger)
								{
									int val = strtol(++ptr, &ptr, 10);
									std::memcpy(output.get() + output_size, &val, type_size);
									output_size += type_size;
								}

								++ptr;
							}

							if (output_size > 0) ofile.write(output.get(), output_size);
							bar.update(last - input.get(), filesize);
						}
					}

				}

				bar.finish();

			} else
			{
				throw std::runtime_error("Error: Cannot write to MTB File!");
			}

		} else
		{
			throw std::runtime_error("Error: Cannot read from MTX file!");
		}
	}

}   // namespace mtb

