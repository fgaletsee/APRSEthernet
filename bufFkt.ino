void uchar2buff(char *p, unsigned char val, char leng)
{
  char k = 0;
  char tmp = 0;
  for (k = leng; k > 0; k--)
  {
    tmp = val / (pow(10, (k - 1)));
    if (tmp > 0)
      val = val - tmp * (pow(10, (k - 1)));
    *p = 0x30 + tmp;
    p++;
  }

}

void uint162buff(char *p, uint16_t val, char leng)
{
  char k = 0;
  uint16_t tmp = 0;
  for (k = leng; k > 0; k--)
  {
    tmp = val / (pow(10, (k - 1)));
    if (tmp > 0)
      val = val - tmp * (pow(10, (k - 1)));
    *p = 0x30 + tmp;
    p++;
  }
}

void adduint162buff(uint16_t val, char leng)
{
  char *p = findend();
  char k = 0;
  uint16_t tmp = 0;
  for (k = leng; k > 0; k--)
  {
    tmp = val / (pow(10, (k - 1)));
    if (tmp > 0)
      val = val - tmp * (pow(10, (k - 1)));
    *p = 0x30 + tmp;
    p++;
  }
}

void def2buff(char *ziel, char *quelle)
{
  while (*quelle)
  {
    *ziel = *quelle;
    ziel++;
    quelle++;
  }

}

void chearchar()
{
  uint8_t k = 0;
  char *p;
  p = txdata;
  for (k = 0; k < buflen; k++)
  {
    *p = 0x00;
    p++;
  }
}

void copychar(char *px)
{
  char *p;
  p = txdata;
  while (*px)
  {
    *p = *px;
    p++;
    px++;
  }
}

void add2char(char *ps)
{
  char *p;
  p = findend();
  while (*ps)
  {
    *p = *ps;
    ps++;
    p++;
  }
}



char * findend()
{
  char *p;
  p = txdata;
  while (*p)
  {
    p++;
  }
  return p;
}
