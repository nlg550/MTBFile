/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico
 This file is part of the MTB library, which is licensed under the
 terms contained in the LICENSE file.
 **************************************************************************/

#include <cstdio>

#include "../include/mtb.hpp"
#include "../include/mtx.hpp"

int main(int argc, char **argv)
{
	if (argc != 4)
    {
	    std::fprintf(stderr, "Usage: ./%s <mtx file> <mtb file> <sort data>.\n", argv[0]);
	    std::fflush(stderr);
	    exit(-1);
    }

	std::string input = argv[1];
	std::string output = argv[2];
	bool sort_data = atoi(argv[3]);

	mtb::mtx_to_mtb(input, output, sort_data);

	return 0;
}
