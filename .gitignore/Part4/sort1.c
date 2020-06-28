/* Simple bubble sort implimentation */

void done() {}

int main(int argc, char **argv)
{
  int i;
  int j;
  int temp;
  int a[16];

  /* Hard coded array */
  a[0] = 13;
  a[1] = 3;
  a[2] = 8;
  a[3] = 15;
  a[4] = 12;
  a[5] = 5;
  a[6] = 10;
  a[8] = 1;
  a[9] = 16;
  a[10] = 6;
  a[11] = 4;
  a[12] = 14;
  a[13] = 2;
  a[14] = 9;
  a[15] = 7;

  /* Compares two adjacent values and swaps them if left > right */
  for(i = 0; i < 16; i++)
    {
      for(j = 0; j < 16 - i - 1; j++)
	{
	  if(a[j] > a[j + 1])
	    {
	      temp = a[j];
	      a[j] = a[j+1];
	      a[j+1] = temp;
	    }
	}
    }
  
  done();
  return 0;
}
