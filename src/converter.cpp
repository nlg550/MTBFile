#include <cstdio>

#include "../include/mtb.hpp"
#include "../include/mtx.hpp"

#include "../../MOCCA/mocca/mocca.h"

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

	mtb::mtx_to_mtb(input, output);

	std::ifstream ifile(output);
	char mat_type, datatype, type_size;
	uint64_t nrows, ncols;
	uint64_t nz;

	mtb::mtb_read_header(ifile, mat_type, datatype, type_size, nrows, ncols, nz);

	mocca::Array<mtb::Triplet<double>> array(nz + (mat_type == mtb::kSymmetricSparse) * nz);
	mtb::mtb_read_data<double>(ifile, array.data(), nz, mat_type, datatype, type_size);

	mocca::CSRMatrix<double> mat(nrows, ncols, nz);
	mat.fill(array.begin(), array.end());

	mocca::io::print(mat);

	return 0;
}
