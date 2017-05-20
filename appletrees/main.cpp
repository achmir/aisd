#include <cstdio>
#include <stdlib.h>
#define MAX 100000000

int Parent(int v)
{
	return v / 2;
}

int Max(int a, int b)
{
	if (a > b) return a;
	return b;
}

int Min(int a, int b)
{
	if (a > b) return b;
	return a;
}

void query(int a, int b, int n, int *min, int *max, int *highest, int *lowest)
{
	int l = n + a, r = n + b;
	*max = 0;
	*min = MAX;
	*max = Max(highest[l], highest[r]);
	*min = Min(lowest[l], lowest[r]);
	while (l >= 1 && l < r - 1)
	{
		if (l % 2 == 0)
		{
			*max = Max(*max, highest[l + 1]);
			*min = Min(*min, lowest[l + 1]);
		}
		if (r % 2 == 1)
		{
			*max = Max(*max, highest[r - 1]);
			*min = Min(*min, lowest[r - 1]);
		}
		l = Parent(l); r = Parent(r);
	}
}

void insert(int v, int apples, int n, int *highest, int *lowest)
{
	int pos = n + v;
	highest[pos] = lowest[pos] = apples;

	while (pos >= 1)
	{
		if (pos < n)
		{
			highest[pos] = Max(highest[2 * pos], highest[2 * pos + 1]);
			lowest[pos] = Min(lowest[2 * pos], lowest[2 * pos + 1]);
		}
		pos = Parent(pos);
	}
}
int main()
{
	int n = 0, m = 0, apples = 0, min = MAX, max = 0, a = 0, b = 0;
	scanf("%d %d", &n, &m);
	int *highest, *lowest;
	highest = new int[2 * n];
	lowest = new int[2 * n];
	for (int i = 0; i < 2 * n; i++)
	{
		highest[i] = 0;
		lowest[i] = 0;
	}
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &apples);
		insert(i, apples, n, highest, lowest);
	}
	for (int i = 0; i < m; i++)
	{
		scanf("%d %d", &a, &b);
		query(a - 1, b - 1, n, &min, &max, highest, lowest);
		printf("\n%d %d", min, max);
	}
	delete[] highest;
	delete[] lowest;
	return 0;
}