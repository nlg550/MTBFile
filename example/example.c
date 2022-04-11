/*************************************************************************
	Copyright (C) 2022 Instituto Superior Tecnico

	This file is part of the MTB library, which is licensed under the
	terms contained in the LICENSE file.
**************************************************************************/

#include <stdio.h>

#include "../include/compatibility.h"

int main(int argc, char **argv)
{
	char filename[64] = "example.mtb";
	char mat_type, datatype, type_size;
	uint64_t nrows, ncols;
	uint64_t nz;

	triplet_dp_t *array;

	read_mtb_dp(filename, &mat_type, &datatype, &type_size, &nrows, &ncols, &nz, &array);

	printf("File = %s\n", filename);
	printf("Header: Matrix Type = %d | DataType = %d | DataType Size = %d\n", mat_type, datatype,
	       type_size);
	printf("Matrix Parameters: NRows = %ld | NCols = %ld | NonZeros = %ld\n", nrows, ncols, nz);
	printf("Data:\n");

	for (int i = 0; i < nz; ++i)
	    printf("(%ld, %ld) %lf\n", array[i].row, array[i].col, array[i].val);

	printf("\n");

	return 0;
}

