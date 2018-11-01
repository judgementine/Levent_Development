#pragma once
#include <iostream>
#include <assert.h>

namespace Stacking
{
	struct Stacking
	{
		int *data;
		int n_data;
		int array_size;
	};

	void init(Stacking *a, int array_size)
	{
		a->n_data = 0;
		a->array_size = array_size;
		a->data = (int*)malloc(array_size * sizeof(int));
		assert(a->data);
	}

	void resize(Stacking *a)
	{
		a->array_size *= 1.2;
		a->array_size++;//just in case
		a->data = (int*)realloc(a->data, a->array_size * sizeof(int));
		assert(a->data);
	}

	void Push(Stacking *a, int value)
	{
		if (a->n_data >= a->array_size) resize(a);
		a->data[a->n_data++] = value;
	}

	int Pop(Stacking *a)
	{
		if (a->n_data == 0) return 0;
		return a->data[--a->n_data];

	}

	void clean(Stacking *a)
	{
		if (a->data != NULL) free(a->data);
		*a = {};//zero out everything
	}

}