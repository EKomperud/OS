#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[])
{
  if (argc != 2)
    return -1;

  int i;
  char* num = argv[1];
  while (num[i] != '\0')
    {
      if (!isdigit(num[i]))
	return -1;
      i++;
    }

  int n = atoi(argv[1]);
  if (n == 0)
    return -1;
  printf("%d",n);
  div_t output;
  while (n != 1)
    {
      output = div(n,2);
      if (output.rem == 0)
	  n = output.quot;
      else
	  n = (3 * n) + 1;
      printf(", %d",n);
    }
  printf("\n");
  return 0;
}
