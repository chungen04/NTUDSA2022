/* we only ensure that this function works for 1 <= s, t <= 1e9.
If you are testing, price(s, t + n) > price(s, t) for all n >= (s - 1) / 1000000 + 1
In other words, the "N" in the problem description is equal to (s - 1) / 1000000 + 1 

Notice that, this function is slightly slower than promised. */

unsigned long long price(unsigned long long s, unsigned long long t){
	unsigned long long p, x = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ULL;
	p = (s - 1) / 1000000ULL + 1ULL;
	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
	x = x ^ (x >> 31);
	unsigned long long la = 0, lb = 0, ra = 0xffffffffffefULL, rb = 0xffffffffffefULL, 
		ma, mb, na, nb, y = x, ta = (t > (p >> 1)) ? (t - (p >> 1)) : 0, tb = t + (p >> 1), tr = t / p;

	for(int i = 28; i >= 1; i -= 3){
		ma = la + (((ra - la) * (x >> 48)) >> 16); 
		mb = lb + (((rb - lb) * (y >> 48)) >> 16); 
		if((1ULL << (i + 2)) & ta) la = ma;
		else ra = ma;
		if((1ULL << (i + 2)) & tb) lb = mb;
		else rb = mb;
		ma = la + (((ra - la) * ((x & 0xffff00000000ULL) >> 32)) >> 16); 
		mb = lb + (((rb - lb) * ((y & 0xffff00000000ULL) >> 32)) >> 16); 
		if((1ULL << (i + 1)) & ta) la = ma;
		else ra = ma;
		if((1ULL << (i + 1)) & tb) lb = mb;
		else rb = mb;
		ma = la + (((ra - la) * ((x & 0xffff0000ULL) >> 16)) >> 16); 
		mb = lb + (((rb - lb) * ((y & 0xffff0000ULL) >> 16)) >> 16); 
		x = (x >> 21) ^ (ta & (1ULL << i)) ^ (x * 0xc0ffee123456789ULL);
		y = (y >> 21) ^ (tb & (1ULL << i)) ^ (y * 0xc0ffee123456789ULL);
		if((1ULL << i) & ta) la = ma;
		else ra = ma;
		if((1ULL << i) & tb) lb = mb;
		else rb = mb;
	}

	ma = la + (((ra - la) * (x >> 48)) >> 16); 
	mb = lb + (((rb - lb) * (y >> 48)) >> 16); 
	if(1ULL & ta) la = ma;
	else ra = ma;
	if(1ULL & tb) lb = mb;
	else rb = mb;
	y = (y ^ (y >> 30)) * 0xbf58476d1ce4e5b9ULL;
	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
	x = x ^ (y << 13);

	return 	la + tr + (((lb - la) * (x >> 48)) >> 16);
}