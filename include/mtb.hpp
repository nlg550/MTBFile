/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
 **************************************************************************/

#ifndef _MTB_HANDLER_HPP_
#define _MTB_HANDLER_HPP_

#include <fstream>
#include <string>
#include <cstring>
#include <memory>

#include "../include/common.hpp"

namespace mtb
{
	// Partial specialization to check if type T is complex or not
	template<typename T>
	struct is_complex: std::false_type {};

	template<typename T>
	struct is_complex<std::complex<T>> : std::true_type {};

	void mtb_read_header(std::ifstream &ifile, char &mat_type, char &datatype, char &type_size,
	                     uint64_t &nrows, uint64_t &ncols, uint64_t &nz);

	void mtb_write_header(std::ofstream &ofile, char mat_type, char datatype, char type_size,
	                      uint64_t nrows, uint64_t ncols, uint64_t nz);

	template<typename T>
	void mtb_read_data(std::ifstream &ifile, Triplet<T> *data, uint64_t nz, char mat_type,
	                   char datatype, char type_size)
	{
		nz += (mat_type == kSymmetricSparse) * nz;
		int batch_size = MTB_BUF_SIZE + (mat_type == kSymmetricSparse) * MTB_BUF_SIZE;
		int step_size = 1 + (mat_type == kSymmetricSparse);
		int raw_max_size = MTB_BUF_SIZE * (2 * sizeof(uint64_t) + type_size);
		std::unique_ptr<char[]> raw(new char[raw_max_size]);

		switch (datatype)
		{
			case kPattern:
				for (uint64_t k = 0; k < nz; k += batch_size)
				{
					char *ptr = raw.get();

					// Read a large data block from the file
					ifile.read(raw.get(), raw_max_size);

					for (int i = 0; i < batch_size && i + k < nz; i += step_size)
					{
						uint64_t row, col;

						std::memcpy(&row, ptr, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&col, ptr, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						data[k + i].col = col;
						data[k + i].row = row;
						data[k + i].val = (T) 1.0;

						if (mat_type == kSymmetricSparse)
                        {
							std::swap(row, col);
							data[k + i + 1].col = col;
							data[k + i + 1].row = row;
							data[k + i + 1].val = (T) 1.0;
                        }
					}
				}
				break;

			case kInteger:
				for (uint64_t k = 0; k < nz; k += batch_size)
				{
					char *ptr = raw.get();

					// Read a large data block from the file
					ifile.read(raw.get(), raw_max_size);

					for (int i = 0; i < batch_size && i + k < nz; i += step_size)
					{
						uint64_t row, col;

						union
						{
							int64_t integer;
							char bytes[sizeof(uint64_t)];
						} val;

						std::memcpy(&row, ptr, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&col, ptr, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&val.bytes, ptr, type_size);
						ptr += type_size;

						data[k + i].col = col;
						data[k + i].row = row;
						data[k + i].val = val.integer;

						if (mat_type == kSymmetricSparse)
                        {
							std::swap(row, col);
							data[k + i + 1].col = col;
							data[k + i + 1].row = row;
							data[k + i + 1].val = val.integer;
                        }
					}
				}
				break;

			case kReal:
				for (uint64_t k = 0; k < nz; k += batch_size)
				{
					char *ptr = raw.get();

					// Read a large data block from the file
					ifile.read(raw.get(), raw_max_size);

					for (int i = 0; i < batch_size && i + k < nz; i += step_size)
					{
						uint64_t row, col;

						union
						{
								double fp;
							char bytes[sizeof(double)];
						} val;

						std::memcpy(&row, ptr, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&col, ptr, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&val.bytes, ptr, type_size);
						ptr += type_size;

						data[k + i].col = col;
						data[k + i].row = row;
						data[k + i].val = val.fp;

						if (mat_type == kSymmetricSparse)
                        {
							std::swap(row, col);
							data[k + i + 1].col = col;
							data[k + i + 1].row = row;
							data[k + i + 1].val = val.fp;
                        }
					}
				}
				break;

			case kComplex:
				if constexpr (is_complex<T>())
				{
					for (uint64_t k = 0; k < nz; k += batch_size)
					{
						char *ptr = raw.get();

						// Read a large data block from the file
						ifile.read(raw.get(), raw_max_size);

						for (int i = 0; i < batch_size && i + k < nz; i += step_size)
						{
							uint64_t row, col;

							union
							{
								double fp;
								char bytes[sizeof(double)];
							} real, imag;

							std::memcpy(&row, ptr, sizeof(uint64_t));
							ptr += sizeof(uint64_t);

							std::memcpy(&col, ptr, sizeof(uint64_t));
							ptr += sizeof(uint64_t);

							std::memcpy(&real.bytes, ptr, type_size);
							ptr += type_size;

							std::memcpy(&imag.bytes, ptr, type_size);
							ptr += type_size;

							data[k + i].col = col;
							data[k + i].row = row;
							data[k + i].val.real = real.fp;
							data[k + i].val.imag = imag.fp;

							if (mat_type == kSymmetricSparse)
	                        {
								std::swap(row, col);
								data[k + i + 1].col = col;
								data[k + i + 1].row = row;
								data[k + i + 1].val.real = real.fp;
								data[k + i + 1].val.imag = imag.fp;
	                        }
						}
					}
				}
				break;

			default:
				throw std::runtime_error("Error: Unsupported MTB datatype");
				break;
		}
	}

	template<typename T>
	void mtb_write_data(std::ofstream &ofile, Triplet<T> *data, uint64_t nz, char mat_type,
	                    char datatype, char type_size)
	{
		int raw_max_size = MTB_BUF_SIZE * (2 * sizeof(uint64_t) + type_size);
		std::unique_ptr<char[]> raw(new char[raw_max_size]);

		switch (datatype)
		{
			case kPattern:
				for (uint64_t k = 0; k < nz; k += MTB_BUF_SIZE)
				{
					char *ptr = raw.get();

					for (int i = 0; i < MTB_BUF_SIZE && i + k < nz; ++i)
					{
						std::memcpy(&ptr, &data[k + i].row, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&ptr, &data[k + i].col, sizeof(uint64_t));
						ptr += sizeof(uint64_t);
					}

					auto size = ptr - raw.get();
					ofile.write(raw.get(), size);

				}
				break;

			case kInteger:
				for (uint64_t k = 0; k < nz; k += MTB_BUF_SIZE)
				{
					char *ptr = raw.get();

					for (int i = 0; i < MTB_BUF_SIZE && i + k < nz; ++i)
					{
						std::memcpy(&ptr, &data[k + i].row, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&ptr, &data[k + i].col, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						union
						{
							int64_t integer;
							char bytes[sizeof(uint64_t)];
						} val;

						val.integer = data[k + i].val;
						std::memcpy(ptr, &val.bytes, type_size);
						ptr += type_size;
					}

					auto size = ptr - raw.get();
					ofile.write(raw.get(), size);
				}
				break;

			case kReal:
				for (uint64_t k = 0; k < nz; k += MTB_BUF_SIZE)
				{
					char *ptr = raw.get();

					for (int i = 0; i < MTB_BUF_SIZE && i + k < nz; ++i)
					{
						std::memcpy(&ptr, &data[k + i].row, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						std::memcpy(&ptr, &data[k + i].col, sizeof(uint64_t));
						ptr += sizeof(uint64_t);

						union
						{
							double fp;
							char bytes[sizeof(double)];
						} val;

						val.fp = data[k + i].val;
						std::memcpy(ptr, &val.bytes, type_size);
						ptr += type_size;
					}

					auto size = ptr - raw.get();
					ofile.write(raw.get(), size);
				}
				break;

			case kComplex:

				if constexpr (is_complex<T>())
				{
					for (uint64_t k = 0; k < nz; k += MTB_BUF_SIZE)
					{
						char *ptr = raw.get();

						for (int i = 0; i < MTB_BUF_SIZE && i + k < nz; ++i)
						{
							std::memcpy(&ptr, &data[k + i].row, sizeof(uint64_t));
							ptr += sizeof(uint64_t);

							std::memcpy(&ptr, &data[k + i].col, sizeof(uint64_t));
							ptr += sizeof(uint64_t);

							union
							{
								double fp;
								char bytes[sizeof(double)];
							} real, imag;

							real.fp = data[k + i].val.real;
							std::memcpy(ptr, &real.bytes, type_size);
							ptr += type_size;

							imag.fp = data[k + i].val.imag;
							std::memcpy(ptr, &imag.bytes, type_size);
							ptr += type_size;
						}

						auto size = ptr - raw.get();
						ofile.write(raw.get(), size);
					}
				}
				break;

			default:
				throw std::runtime_error("Error: Unsupported MTB datatype");
				break;
		}
	}

}   // namespace mtb

#endif /* _MTB_HANDLER_HPP_ */
