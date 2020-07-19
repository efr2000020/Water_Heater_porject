#define SETBIT(REG,BIT)  (REG |= 1 << BIT)
#define CLRBIT(REG,BIT)  (REG &= ~(1 << BIT))
#define TOGBIT(REG,BIT)  (REG ^= 1 << BIT)
#define READBIT(REG,BIT)  ((REG >> BIT) & 1)

#define SET_REGISTER_8BIT(REG)  (REG = 0xff)
#define CLR_REGISTER_8BIT(REG)  (REG = 0x00)


typedef unsigned char   uint8_t;
typedef unsigned int    uint16_t;
typedef unsigned long   uint32_t;
