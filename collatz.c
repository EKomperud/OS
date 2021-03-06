#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, const char* argv[])
{
  if (argc != 2)
    return -1;

  int i = 0;
  const char* num = argv[1];
  while (num[i] != '\0')
    {
      if (!isdigit(num[i]))
	return -1;
      i++;
    }

  pid_t pid;
  pid = fork();
  if (pid == 0)
    {
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
    }
  else if (pid == -1)
    {
      return -1;
    }
  else
    {
      wait(NULL);
    }
  return 0;
}
