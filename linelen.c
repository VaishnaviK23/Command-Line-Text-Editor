int len(int lineno)
{
   int linelen = COLS - 1;

   while (linelen >= 0 && mvinch(lineno, linelen) == ' ')
       linelen--;
   return linelen + 1;
}

