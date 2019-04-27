#ifndef __BIT_OPER_H__
#define __BIT_OPER_H__


#define OPER_LEFT_MOVE(x, y)					((x) << (y))
#define OPER_RIGHT_MOVE(x, y)					((x) >> (y))


#define OPER_GET_BIT(x, y)						((x) & (1 << (y)))
#define OPER_CLR_BIT(x, y)						((x) &= ~(1 << (y)))
#define OPER_SET_BIT(x, y)						((x) |= (1 << (y)))
#define OPER_WRI_BIT(x, y, z)					((z) ? OPER_SET_BIT(x, y) : OPER_CLR_BIT(x, y))	
#define OPER_TOG_BIT(x, y)						((x) ^= ( 1 << (y)))


#define OPER_GET_BITS(x, y)						((x) & (y))
#define OPER_CLR_BITS(x, y)						((x) &= ~(y))
#define OPER_SET_BITS(x, y)						((x) |= (y))
#define OPER_WIR_BITS(x, y, z)					((z) ? OPER_SET_BITS(x, y) : OPER_CLR_BITS(x, y))
#define OPER_TOG_BITS(x, y)						((x) ^= (y))


#define OPER_GET_BIT_0(x)							(OPER_GET_BIT((x), 0))
#define OPER_GET_BIT_1(x)							(OPER_GET_BIT((x), 1))
#define OPER_GET_BIT_2(x)							(OPER_GET_BIT((x), 2))
#define OPER_GET_BIT_3(x)							(OPER_GET_BIT((x), 3))
#define OPER_GET_BIT_4(x)							(OPER_GET_BIT((x), 4))
#define OPER_GET_BIT_5(x)							(OPER_GET_BIT((x), 5))
#define OPER_GET_BIT_6(x)							(OPER_GET_BIT((x), 6))
#define OPER_GET_BIT_7(x)							(OPER_GET_BIT((x), 7))
#define OPER_GET_BIT_8(x)							(OPER_GET_BIT((x), 8))
#define OPER_GET_BIT_9(x)							(OPER_GET_BIT((x), 9))
#define OPER_GET_BIT_10(x)							(OPER_GET_BIT((x), 10))
#define OPER_GET_BIT_11(x)							(OPER_GET_BIT((x), 11))
#define OPER_GET_BIT_12(x)							(OPER_GET_BIT((x), 12))
#define OPER_GET_BIT_13(x)							(OPER_GET_BIT((x), 13))
#define OPER_GET_BIT_14(x)							(OPER_GET_BIT((x), 14))
#define OPER_GET_BIT_15(x)							(OPER_GET_BIT((x), 15))
#define OPER_GET_BIT_16(x)							(OPER_GET_BIT((x), 16))
#define OPER_GET_BIT_17(x)							(OPER_GET_BIT((x), 17))
#define OPER_GET_BIT_18(x)							(OPER_GET_BIT((x), 18))
#define OPER_GET_BIT_19(x)							(OPER_GET_BIT((x), 19))
#define OPER_GET_BIT_20(x)							(OPER_GET_BIT((x), 20))
#define OPER_GET_BIT_21(x)							(OPER_GET_BIT((x), 21))
#define OPER_GET_BIT_22(x)							(OPER_GET_BIT((x), 22))
#define OPER_GET_BIT_23(x)							(OPER_GET_BIT((x), 23))
#define OPER_GET_BIT_24(x)							(OPER_GET_BIT((x), 24))
#define OPER_GET_BIT_25(x)							(OPER_GET_BIT((x), 25))
#define OPER_GET_BIT_26(x)							(OPER_GET_BIT((x), 26))
#define OPER_GET_BIT_27(x)							(OPER_GET_BIT((x), 27))
#define OPER_GET_BIT_28(x)							(OPER_GET_BIT((x), 28))
#define OPER_GET_BIT_29(x)							(OPER_GET_BIT((x), 29))
#define OPER_GET_BIT_30(x)							(OPER_GET_BIT((x), 30))
#define OPER_GET_BIT_31(x)							(OPER_GET_BIT((x), 31))








#define PI											3.1415927
#define GET_BIT_UINT_MAX_NUM(x)						(~(OPER_LEFT_MOVE(1, x)))

#ifndef UINT8_MAX
#define UINT8_MAX									(GET_BIT_UINT_MAX_NUM(7))
#endif
#define UINT8_MIN									0
#define UINT8_LEN									(UINT8_MAX - UINT8_MIN)

#ifndef UINT16_MAX
#define UINT16_MAX									(GET_BIT_UINT_MAX_NUM(15))
#endif
#define UINT16_MIN									0
#define UINT16_LEN									(UINT16_MAX - UINT16_MIN)

#define UINT24_MAX									(GET_BIT_UINT_MAX_NUM(23))
#define UINT24_MIN									0
#define UINT24_LEN									(UINT24_MAX - UINT24_MIN)

#ifndef UINT32_MAX
#define UINT32_MAX									(GET_BIT_UINT_MAX_NUM(31))
#endif
#define UINT32_MIN									0
#define UINT32_LEN									(UINT32_MAX - UINT32_MIN)

#define GET_1_31_INT_MAX_NUM(n)						(~(OPER_LEFT_MOVE((unsigned long)1, (n))))
#define GET_0_INT_MAX_NUM()							(0?)
#define GET_BIT_INT_MAX_NUM(x)						(((x) == 0) ? GET_0_INT_MAX_NUM() : GET_1_31_INT_MAX_NUM(x))
#define GET_BIT_INT_MIN_NUM(n)						(-(GET_BIT_INT_MAX_NUM(n) + 1))

#ifndef INT8_MAX
#define INT8_MAX									(GET_BIT_INT_MAX_NUM(7))
#define INT8_MIN									(GET_BIT_INT_MIN_NUM(7))
#endif
#define INT8_LEN									(INT8_MAX - INT8_MIN)

#ifndef INT16_MAX
#define INT16_MAX									(GET_BIT_INT_MAX_NUM(15))
#define INT16_MIN									(GET_BIT_INT_MIN_NUM(15))
#endif
#define INT16_LEN									(INT16_MAX - INT16_MIN)

#ifndef INT24_MAX
#define INT24_MAX									(GET_BIT_INT_MAX_NUM(23))
#define INT24_MIN									(GET_BIT_INT_MIN_NUM(23))
#endif
#define INT24_LEN									(INT24_MAX - INT24_MIN)

#ifndef INT32_MAX
#define INT32_MAX									(GET_BIT_INT_MAX_NUM(31))
#define INT32_MIN									(GET_BIT_INT_MIN_NUM(31))
#endif
#define INT32_LEN									(INT32_MAX - INT32_MIN)



#define IS_ODD_NUM(x)								(((x) & 1) == 1)
#define IS_SAME_SIGN(x, y)							(((x) ^ (y)) >= 0)
#define IS_POWER_OF_2(x)							(((x) > 0) && (((x) & ((x) - 1))) == 0)



#define GET_MAX_NUM(x, y)							((x) > (y) ? (x) : (y))
#define GET_MIN_NUM(x, y)							((x) < (y) ? (x) : (y))
#define GET_AVG_2_NUM(x, y)							(((x) + (y)) >> 1)


#endif

