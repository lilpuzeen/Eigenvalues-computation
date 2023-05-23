#include "return_codes.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int n, m;
	double *elements;
} Matrix;

void InitializeMatrix(Matrix *matrix, size_t n, size_t m);
size_t ReadMatrix(FILE *in, Matrix *matrix);
void PrintEigenvalues(FILE *out, Matrix *matrix);
void CountComplex(double a, double b, double c, double d, FILE *out);
void CopyMatrix(Matrix *m1, Matrix *m2);
void ClearMatrix(Matrix *matrix);
void MultiplyMatrices(Matrix *a, Matrix *b, Matrix *out);
size_t QRDecomposition(Matrix *matrix, Matrix *Q_out, Matrix *R_out);

void InitializeMatrix(Matrix *matrix, size_t n, size_t m)
{
	matrix->n = n;
	matrix->m = m;
	matrix->elements = malloc(sizeof(double) * n * m);
}

size_t ReadMatrix(FILE *in, Matrix *matrix)
{
	for (size_t i = 0; i < matrix->n * matrix->m; i++)
	{
		if (!fscanf(in, "%lf", matrix->elements + i))
		{
			fprintf(stderr, "Error: invalid data in input file");
			return ERROR_DATA_INVALID;
		}
	}

	return SUCCESS;
}

void PrintEigenvalues(FILE *out, Matrix *matrix)
{
	double eps = 1e-10;
	bool lastIterationOperation = false;
	for (size_t i = 0; i < matrix->n - 1; i++)
	{
		if (fabs(matrix->elements[(i + 1) * matrix->n + i]) < eps)
		{
			fprintf(out, "%g\n", matrix->elements[i * matrix->n + i]);
			lastIterationOperation = false;
		}
		else
		{
			double a = matrix->elements[i * matrix->n + i];
			double b = matrix->elements[i * matrix->n + i + 1];
			double c = matrix->elements[(i + 1) * matrix->n + i];
			double d = matrix->elements[(i + 1) * matrix->n + i + 1];
			CountComplex(a, b, c, d, out);
			lastIterationOperation = true;
			i++;
		}
	}
	if (lastIterationOperation == false)
	{
		fprintf(out, "%g\n", matrix->elements[(matrix->n - 1) * matrix->n + matrix->n - 1]);
	}
}

void CountComplex(double a, double b, double c, double d, FILE *out)
{
	double m = (a + d) / 2;
	double p = (a * d - b * c);
	double imaginary = sqrt(fabs(m * m - p));
	fprintf(out, "%g +%gi\n", m, imaginary);
	fprintf(out, "%g -%gi\n", m, imaginary);
}

void CopyMatrix(Matrix *m1, Matrix *m2)
{
	m2->n = m1->n;
	m2->m = m1->m;

	for (size_t i = 0; i < m1->n; i++)
	{
		for (size_t j = 0; j < m1->m; j++)
		{
			m2->elements[i * m1->m + j] = m1->elements[i * m1->m + j];
		}
	}
}

void ClearMatrix(Matrix *matrix)
{
	free(matrix->elements);
}

void MultiplyMatrices(Matrix *a, Matrix *b, Matrix *out)
{
	out->n = a->n;
	out->m = b->m;

	for (size_t i = 0; i < out->n; i++)
	{
		for (size_t j = 0; j < out->m; j++)
		{
			out->elements[i * out->m + j] = 0;

			for (size_t k = 0; k < a->m; k++)
			{
				out->elements[i * out->m + j] += a->elements[i * a->m + k] * b->elements[k * b->m + j];
			}
		}
	}
}

double GetColumnNorm(Matrix *matrix, size_t col)
{
	double result = 0;

	for (size_t i = 0; i < matrix->n; i++)
	{
		result += matrix->elements[i * matrix->m + col] * matrix->elements[i * matrix->m + col];
	}

	return sqrt(result);
}

void NormalizeColumn(Matrix *matrix, size_t col)
{
	double col_norm = GetColumnNorm(matrix, col);

	if (col_norm != 0)
	{
		for (size_t j = 0; j < matrix->n; j++)
		{
			matrix->elements[j * matrix->m + col] /= col_norm;
		}
	}
}

size_t QRDecomposition(Matrix *matrix, Matrix *Q_out, Matrix *R_out)
{
	size_t n = matrix->n;
	CopyMatrix(matrix, Q_out);

	for (size_t j = 0; j < n; j++)
	{
		R_out->elements[j * n + j] = GetColumnNorm(Q_out, j);
		NormalizeColumn(Q_out, j);
		if (R_out->elements[j * n + j] == 0)
		{
			continue;
		}
		for (size_t i = j + 1; i < n; i++)
		{
			R_out->elements[j * n + i] = 0;

			for (size_t k = 0; k < n; k++)
			{
				R_out->elements[j * n + i] += Q_out->elements[k * n + j] * Q_out->elements[k * n + i];
			}
			for (size_t k = 0; k < n; k++)
			{
				Q_out->elements[k * n + i] -= Q_out->elements[k * n + j] * R_out->elements[j * n + i];
			}
		}
	}

	return SUCCESS;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Invalid number of arguments");
		return ERROR_PARAMETER_INVALID;
	}

	FILE *input = fopen(argv[1], "rb");

	if (input == NULL)
	{
		fprintf(stderr, "Cannot open file");
		return ERROR_CANNOT_OPEN_FILE;
	}

	size_t n;
	fscanf(input, "%zd\n", &n);
	Matrix matrix;
	InitializeMatrix(&matrix, n, n);

	if (matrix.elements == NULL)
	{
		fclose(input);
		fprintf(stderr, "Memory allocation error");
		return ERROR_OUT_OF_MEMORY;
	}

	if (ReadMatrix(input, &matrix) != 0)
	{
		fclose(input);
		ClearMatrix(&matrix);
		fprintf(stderr, "Invalid data in file");
		return ERROR_DATA_INVALID;
	}

	fclose(input);

	FILE *out = fopen(argv[2], "w");

	if (out == NULL)
	{
		ClearMatrix(&matrix);
		fprintf(stderr, "Cannot open file");
		return ERROR_CANNOT_OPEN_FILE;
	}

	Matrix Q, R;
	InitializeMatrix(&Q, n, n);
	InitializeMatrix(&R, n, n);

	if (R.elements == NULL || Q.elements == NULL)
	{
		ClearMatrix(&R);
		ClearMatrix(&Q);
		ClearMatrix(&matrix);
		fprintf(stderr, "Memory allocation error");
		return ERROR_OUT_OF_MEMORY;
	}

	for (size_t i = 0; i < 2000; i++)
	{
		QRDecomposition(&matrix, &Q, &R);
		MultiplyMatrices(&R, &Q, &matrix);
	}
	PrintEigenvalues(out, &matrix);

	fclose(out);
	ClearMatrix(&R);
	ClearMatrix(&Q);
	ClearMatrix(&matrix);
	return SUCCESS;
}
