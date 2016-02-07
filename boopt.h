#ifndef BOOPTHEADER
#define BOOPTHEADER
static inline void outb(unsigned char v, unsigned short port)
{
	asm volatile("outb %0,%1" : : "a" (v), "dN" (port));
}
static inline unsigned char inb(unsigned short port)
{
	unsigned char v;
	asm volatile("inb %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void outw(unsigned short v, unsigned short port)
{
	asm volatile("outw %0,%1" : : "a" (v), "dN" (port));
}
static inline unsigned short inw(unsigned short port)
{
	unsigned short v;
	asm volatile("inw %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void outl(unsigned int v, unsigned short port)
{
	asm volatile("outl %0,%1" : : "a" (v), "dN" (port));
}
static inline unsigned int inl(unsigned short port)
{
	unsigned int v;
	asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void io_delay(void)
{
	const unsigned short DELAY_PORT = 0x80;
	asm volatile("outb %%al,%0" : : "dN" (DELAY_PORT));
}
#endif
