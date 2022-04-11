/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
**************************************************************************/

#include "../include/mtb.hpp"

namespace mtb
{
	/*********************************************************************************************
	 MTB File Handler
	 *********************************************************************************************/

	void mtb_read_header(std::ifstream &ifile, char &mat_type, char &datatype, char &type_size,
	                     uint64_t &nrows, uint64_t &ncols, uint64_t &nz)
	{
		char tmp;

		ifile.read((char *) &mat_type, 1);
		ifile.read((char *) &tmp, 1);
		ifile.read((char *) &ncols, sizeof(uint64_t));
		ifile.read((char *) &nrows, sizeof(uint64_t));
		ifile.read((char *) &nz, sizeof(uint64_t));

		datatype = (tmp & 0xF0);
		type_size = (tmp & 0x0F);
	}

	void mtb_write_header(std::ofstream &ofile, char mat_type, char datatype, char type_size,
	                      uint64_t nrows, uint64_t ncols, uint64_t nz)
	{
		char tmp = datatype | type_size;

		ofile.write((char *) &mat_type, 1);
		ofile.write((char *) &tmp, 1);
		ofile.write((char *) &ncols, sizeof(uint64_t));
		ofile.write((char *) &nrows, sizeof(uint64_t));
		ofile.write((char *) &nz, sizeof(uint64_t));
	}
}   // namespace mtb



