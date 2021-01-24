void decode1(long *xp, long *yp, long *zp)
{
    /* xp in %rdi, yp in %rsi, zp in %rdx */
    long r8 = *xp;
    long cx = *yp;
    long ax = *zp;
    *yp = r8;
    *zp = cx;
    *xp = ax;
    return;
}