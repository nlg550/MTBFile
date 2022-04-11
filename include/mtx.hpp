/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
**************************************************************************/

#ifndef _MTX_HANDLER_HPP_
#define _MTX_HANDLER_HPP_

#include <stdint.h>
#include <fstream>
#include <string>
#include <vector>

namespace mtb
{
	void mtx_read_header(std::ifstream &ifile, std::vector<std::string> &properties,
	                     uint64_t &nrows, uint64_t &ncols, uint64_t &nz);

	void mtx_to_mtb(std::string mtx_file, std::string mtb_file, bool sort_data);
}   // namespace mtb

#endif /* _MTX_HANDLER_HPP_ */
