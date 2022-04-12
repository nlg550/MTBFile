/*************************************************************************
 Copyright (C) 2022 Instituto Superior Tecnico

 This file is part of the MOCCA library, which is licensed under the
 terms contained in the LICENSE file.
 **************************************************************************/

#include "../include/compatibility.h"
#include "../include/mtb.hpp"

void read_mtb_int(char filename[64], char *mat_type, char *datatype, char *type_size,
                  uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_int_t **array)
{
	std::ifstream ifile(filename, std::fstream::binary);
	mtb::mtb_read_header(ifile, *mat_type, *datatype, *type_size, *nrows, *ncols, *nz);

	if (*mat_type == kSymmetricSparse) *nz *= 2;
	*array = new triplet_int_t[*nz];

	mtb::mtb_read_data(ifile, (mtb::Triplet<int> *) *array, *nz, *mat_type, *datatype, *type_size);
}

void read_mtb_sp(char filename[64], char *mat_type, char *datatype, char *type_size,
                 uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_sp_t **array)
{
	std::ifstream ifile(filename, std::fstream::binary);
	mtb::mtb_read_header(ifile, *mat_type, *datatype, *type_size, *nrows, *ncols, *nz);

	if (*mat_type == kSymmetricSparse) *nz *= 2;
	*array = new triplet_sp_t[*nz];

	mtb::mtb_read_data(ifile, (mtb::Triplet<float> *) *array, *nz, *mat_type, *datatype, *type_size);
}

void read_mtb_dp(char filename[64], char *mat_type, char *datatype, char *type_size,
                 uint64_t *nrows, uint64_t *ncols, uint64_t *nz, triplet_dp_t **array)
{
	std::ifstream ifile(filename, std::fstream::binary);
	mtb::mtb_read_header(ifile, *mat_type, *datatype, *type_size, *nrows, *ncols, *nz);

	if (*mat_type == kSymmetricSparse) *nz *= 2;
	*array = new triplet_dp_t[*nz];

	mtb::mtb_read_data(ifile, (mtb::Triplet<double> *) *array, *nz, *mat_type, *datatype, *type_size);
}
