int lookup(int index);
void printnum(int num);

void myfunc(int start, int count)
{
  int i, end, sum;

  sum = 0;
  end = start + count;

  for (i = start ; i <= end ; i++)
    sum = sum + lookup(i);

  if (sum < 0)
    sum = -sum;

  printnum(sum);
}
