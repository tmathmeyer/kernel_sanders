
static unsigned int sand_seed_hash = 1337;

void sand_seed(const unsigned int seed) {
	sand_seed_hash = sand_seed_hash * 31 + seed;
}

unsigned int sand_rand() {
    unsigned int y = sand_seed_hash;

    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9D2C5680U;
    y ^= (y << 15) & 0xEFC60000U;
    sand_seed(y);
    return (y ^ (y >> 18));
}
