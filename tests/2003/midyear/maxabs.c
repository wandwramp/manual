int absolute(int x);

int max_absolute(int array[], int length)
{
  int maximum;
  int index;
  int item;
  int i;

  maximum = 0;

  for (i = 0 ; i < length ; i++) {
    item = absolute(array[i]);

    if (item >= maximum) {
      index = i;
      maximum = item;
    }
  }
  return index;
}
