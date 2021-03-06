#include "setup.h"
#include "log.h"
#include "opcodes.h"

//Contains code for execution of instructions

//SINGLE OPERAND
//General
int op_clr(uint16_t instruction)
{
    uint8_t dst = instruction & 077;
	uint16_t dst_addr;

    log(LOG_INST, "CLR function called\n");

    operand_value_read_word(dst, &dst_addr);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, 0);
    else
        reg[dst] = 0;

    psw.negative = 0;
    psw.zero = 1;
    psw.overflow = 0;
    psw.carry = 0;
    return 0;
}

int op_clrb(uint16_t instruction){
    uint8_t dst = instruction & 077;
    log(LOG_INST, "CLRB function called\n");

	uint16_t dst_addr;	
    operand_value_read_word(dst, &dst_addr);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, 0);
    else
		reg[dst] = (reg[dst] & 0177400); // only clear the low byte of register

    psw.negative = 0;
    psw.zero = 1;
    psw.overflow = 0;
    psw.carry = 0;
    return 0;
}


int op_com(uint16_t instruction){
    uint8_t dst = instruction & 077;
    log(LOG_INST, "COM function called\n");
    uint16_t value;
    uint16_t dst_addr;

    value = operand_value_read_word(dst, &dst_addr);

    value = (~value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

    //if msb of result is set 
    psw.negative = ((value >> 15) == 1);
    //set if result is 0
    psw.zero = (value == 0);
    psw.overflow = 0;
    psw.carry = 1;
	return 0;
}

int op_comb(uint16_t instruction){
	uint8_t dst = instruction & 077;
    log(LOG_INST, "COMB function called\n");
    uint8_t value;
    uint16_t dst_addr;

    value = operand_value_read_word(dst, &dst_addr);
	value = (~value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


    //if msb of result is set 
    psw.negative = ((value >> 7) == 1);
    //set if result is 0
    psw.zero = (value == 0);
    psw.overflow = 0;
    psw.carry = 1;
	return 0;
}


int op_inc(uint16_t instruction){
    uint8_t dst = instruction & 077;
//    uint8_t mode = instruction & 70;
//   uint8_t dst_reg = instruction & 007;
	log(LOG_INST, "INC function called\n");

    uint16_t dst_addr;
	int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);
	value = (uint16_t)(old_value + 1);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;


	//set if result is less than 0
	psw.negative = (value < 0);
	//set if result is 0
	psw.zero = (value == 0);
	//set if dst held 077777 cleared otherwise
	psw.overflow = (old_value == 0077777);

	return 0;
}

int op_incb(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "INCB function called\n");
	int8_t old_value, value;
    uint16_t dst_addr;

    old_value = operand_value_read_word(dst, &dst_addr);
	value = (1 + old_value);


    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


	//set if result is less than 0
	psw.negative = (value < 0);
	//set if result is 0
   	psw.zero = (value == 0);
	//set if dst held 077777 cleared otherwise
    psw.overflow = (old_value == 0377); 
	return 0;
}


int op_dec(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "DEC function called\n");

	uint16_t dst_addr;
	int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);
	value = (uint16_t)(old_value - 1);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

	//set if result is less than 0
    psw.negative = (value < 0);
    //set if result is 0
    psw.zero = (value == 0);
    //set if dst held 100000 cleared otherwise
    psw.overflow = (old_value == 0100000);
	return 0;
}


int op_decb(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "DECB function called\n");

	int8_t value;
    uint16_t dst_addr;

    value = operand_value_read_word(dst, &dst_addr);
	value = value - 1;

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


	//set if result is less than 0
    psw.negative = (value < 0);
    //set if result is 0
    psw.zero = (value == 0);
    psw.overflow = (value == 0100000); //not sure about this
	return 0;
}


int op_neg(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "NEG function called\n");

	uint16_t dst_addr;
	int16_t old_value, value, i;

    old_value = operand_value_read_word(dst, &dst_addr);

	i = (~old_value);
	value = (uint16_t)(i + 1);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

	
	//set if result is less than 0
    psw.negative = (value < 0);
    //set if result is 0
    psw.zero = (value == 0);
    //set if dst held 100000 cleared otherwise
    psw.overflow = (old_value == 0100000);
    psw.carry = (value != 0);

	return 0;
}


int op_negb(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "NEGB function called\n");

	int8_t old_value, value, i;
	uint16_t dst_addr;

	old_value = operand_value_read_word(dst, &dst_addr);
	i = (~old_value);
	value = i + 1;


    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register
	
	//set if result is less than 0
    psw.negative = (value < 0);
    //set if result is 0
    psw.zero = (value == 0);
    psw.overflow = (old_value == 0100000);  // max 8 bit + 1, not sure about this
    psw.carry = (value != 0);

	return 0;
}


//Shift and Rotate
int op_asr(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "ASR function called\n");
    uint16_t dst_addr;
    int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);
	
	value = (old_value & 0100000) | (old_value >> 1);
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

	//set if high order bit of the result is set(result < 0)
	psw.negative = ((value >> 15) == 1);
	//set if result = 0
	psw.zero = (value == 0);
	//loaded from the low order bit of the destination
	psw.carry = (old_value & 0000001);
	//loaded from the exclusive OR of the N-bit and C-bit
    psw.overflow = (psw.negative ^ psw.carry);

	return 0;
}

int op_asrb(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "ASRB function called\n");
	uint8_t old_value, value;
	uint16_t dst_addr;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = (old_value & 0100000) | (old_value >> 1); //value = (old_value & 0200) | (old_value >> 1);
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


	//set if high order bit of the result is set(result < 0)
	psw.negative = ((value >> 7) == 1);
	//set if result = 0
	psw.zero = (value == 0);
	//loaded from the exclusive OR of the N-bit and C-bit
	psw.carry = (old_value & 0001);
	//loaded from the low order bit of the destination
    psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}


int op_asl(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "ASL function called\n");

    uint16_t dst_addr;
    int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = (0177776) & (old_value << 1);
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;


    psw.negative = (value >> 15);
    psw.zero = (value == 0);
    psw.carry = (value >> 15);
    psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}

int op_aslb(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "ASLB function called\n");
	uint8_t old_value, value;
    uint16_t dst_addr;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = ((0374) & (old_value << 1));
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


    psw.negative = ((value >> 7) == 1);
    psw.zero = (value == 0);
    psw.carry = ((old_value & 0200) >> 7);
    psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}


int op_ror(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "ROR function called\n");

    uint16_t dst_addr;
    int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);	

	value = ((old_value >> 1) | (psw.carry << 15));


    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;


	psw.carry = (old_value & 0000001);
	psw.zero = (value == 0);
	psw.negative = (value >> 15);
	psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}

int op_rorb(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "RORB function called\n");
	uint8_t old_value, value;
    uint16_t dst_addr;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = ((old_value >> 1) | (psw.carry << 7));

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


	psw.carry = (old_value & 0001);
	psw.zero = (value == 0);
	psw.negative = ((value >> 7) == 1);
	psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}


int op_rol(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "ROL function called\n");
	uint16_t dst_addr;
    int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = (old_value << 1) | (psw.carry);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

	psw.carry = ((old_value & 0100000) >> 15);
	psw.zero = (value == 0);
	psw.negative = ((value >> 15) == 1);
	psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}

int op_rolb(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "ROLB function called\n");
	uint8_t old_value, value;
    uint16_t dst_addr;

    old_value = operand_value_read_word(dst, &dst_addr);
	value = (old_value << 1) | (psw.carry);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


	psw.carry = ((old_value & 0200) >> 7);
	psw.zero = (value == 0);
	psw.negative = ((value >> 7) == 1);
	psw.overflow = (psw.negative ^ psw.carry);
	return 0;
}


int op_swab(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "SWAB function called\n");
    uint16_t dst_addr;
	uint16_t old_value, value;

	old_value = operand_value_read_word(dst, &dst_addr);

	value = (((old_value & 0177400) >> 8) | ((old_value & 0000377) << 8));

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

	psw.carry = 0;
	psw.zero = ((value & 0000377) == 0);
	psw.negative = ((value & 0000200) >> 7);
	psw.overflow = 0;
	return 0;
}


//Multiple Precision
int op_adc(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "ADC function called\n");

    uint16_t dst_addr;
    int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = old_value + psw.carry;
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.carry = ((old_value == 0177777) && (psw.carry));
    psw.overflow = ((old_value == 0077777) && (psw.carry));
	return 0;
}

int op_adcb(uint16_t instruction){
	uint8_t dst = instruction & 077;
	log(LOG_INST, "ADCB function called\n");
	int8_t old_value, value;
    uint16_t dst_addr;

    old_value = operand_value_read_word(dst, &dst_addr);
	value = old_value + psw.carry;
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.carry = ((old_value == 0377) && (psw.carry));
    psw.overflow = ((old_value == 0177) && (psw.carry));
	return 0;
}


int op_sbc(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "SBC function called\n");

	uint16_t dst_addr;
    int16_t old_value, value;

    old_value = operand_value_read_word(dst, &dst_addr);

	value = old_value - psw.carry;
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;


    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.carry = !((old_value == 0) && (psw.carry));
    psw.overflow = (old_value == 0100000);  // bit 15 was set
	return 0;
}

int op_sbcb(uint16_t instruction){
    uint8_t dst = instruction & 077;
	log(LOG_INST, "SBCB function called\n");
	int8_t old_value, value;
	uint16_t dst_addr;

    old_value = (int8_t)operand_value_read_word(dst, &dst_addr);
	value = old_value - psw.carry;
	log(LOG_INFO, "Value %d\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.carry = !((old_value == 0) && (psw.carry));
    psw.overflow = (old_value == 0200);  // bit 7 was set
	return 0;
}


//DOUBLE OPERAND
//General
int op_mov(uint16_t instruction)
{
    uint8_t src = (instruction >> 6) & 077; // bits 11-6
    uint8_t dst = instruction & 077; // bits 5-0
    //uint8_t dst_reg = instruction & 07; // bits 5-0
    //uint8_t dst_mode = instruction >> 3 & 07; // bits 5-0
    uint16_t value;
    uint16_t src_addr, dst_addr;

    log(LOG_INST, "MOV function called\n");

    value = operand_value_read_word(src, &src_addr);
    operand_value_read_word(dst, &dst_addr);

	if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
	else
		reg[dst] = value;

    
/*
    N: set if (src) <0; cleared otherwise
    Z: set if (src) = 0; cleared otherwise
    V: cleared
    C: not affected 
*/
    psw.negative = ((int8_t)value < 0);
    psw.zero = (src == 0);
    psw.overflow = 0;

    return 0;

}
/*
The MOVB to a register (unique among
byte instructions) extends the most significant bit of the low
order byte (sign extension). Otherwise MOVB operates on
bytes exactly as MOV operates on words. 
*/
int op_movb(uint16_t instruction)
{
    uint8_t src = (instruction >> 6) & 077; // bits 11-6
    uint8_t dst = instruction & 077; // bits 5-0
	//uint8_t dst_mode = (instruction >> 3) & 07; // bits 3-5
    int16_t full_value = 0;
	uint8_t value;
	log(LOG_INST, "MOVB function called\n");
    uint16_t src_addr, dst_addr;

    value = operand_value_read_byte(src, &src_addr);
    operand_value_read_byte(dst, &dst_addr);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else { // destination is register  The MOVB to a register (unique among byte instructions) extends the most significant bit of the low order byte (sign extension)
		//extend sign out
		if ((value & 0200) >> 7 == 1)
			reg[dst] = 0177400 | value;
		else
			reg[dst] = value;
	}

/*
    N: set if (src) <0; cleared otherwise
    Z: set if (src) = 0; cleared otherwise
    V: cleared
    C: not affected 
*/
    psw.negative = ((int8_t)value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

    return 0;
}


int op_cmp(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
	uint32_t value;
	log(LOG_INST, "CMP function called\n");

    uint16_t src_addr, dst_addr;

    int16_t src_val = operand_value_read_word(src, &src_addr);
    int16_t dst_val = operand_value_read_word(dst, &dst_addr);

	log(LOG_INFO,"Source value is %d, Dest value is %d\n", src_val, dst_val);
	
	value = src_val - dst_val;
	
	log(LOG_INFO,"result is %d\n", value);
    
	/*
	N: set if result <0; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: set if there was arithmetic overflow; that is, operands were
	of opposite signs and the sign of the destination was the
	same as the sign of the result; cleared otherwise
	C: cleared if there was a carry from the most significant bit of
	the result; set otherwise 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = (((src_val < 0) && (dst_val < 0) && (value >=0)) || 
					((src_val >= 0) && (dst_val >= 0) && (value < 0)));
	psw.carry = (value >> 16 & 1 == 1);
	
	return 0;
}

int op_cmpb(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
	uint16_t value;
	log(LOG_INST, "CMPB function called\n");
	
    uint16_t src_addr, dst_addr;

    int8_t src_val = operand_value_read_byte(src, &src_addr);
    int8_t dst_val = operand_value_read_byte(dst, &dst_addr);

	log(LOG_INFO,"Source value is %d, Dest value is %d\n", src_val, dst_val);
	
	value = src_val - dst_val;
	
	log(LOG_INFO,"result is %d\n", value);
    
    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register

	/*
	N: set if result <0; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: set if there was arithmetic overflow; that is, operands were
	of opposite signs and the sign of the destination was the
	same as the sign of the result; cleared otherwise
	C: cleared if there was a carry from the most significant bit of
	the result; set otherwise 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = (((src_val < 0) && (dst_val < 0) && (value >=0)) || 
					((src_val >= 0) && (dst_val >= 0) && (value < 0)));
	psw.carry = (value >> 8 & 1 == 1);
	
	return 0;
}


int op_add(uint16_t instruction)
{
    uint8_t src = (instruction >> 6) & 077; // bits 6-8
    uint8_t dst = instruction & 077; // bits 0-5
    //uint8_t dst_reg = instruction & 07; // bits 0-2
    //uint8_t dst_mode = (instruction >> 3)& 07; // bits 0-2
    int32_t value;
    uint16_t src_addr, dst_addr;
    log(LOG_INST, "ADD function called\n");

    int16_t src_val = operand_value_read_word(src, &src_addr);
    int16_t dst_val = operand_value_read_word(dst, &dst_addr);
    value = src_val + dst_val;
	log(LOG_INFO, "value = %o src_val = %o dst_val = %o\n", value, src_val, dst_val)

	if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
	else
		reg[dst] = value;

	
	/*
	N: set if result <0; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: set if there was arithmetic overflow as a result of the oper·
	ation; that is both operands were of the same sign and the
	result was of the opposite sign; cleared otherwise
	C: set if there was a carry from the most significant bit of the
	result; cleared otherwise 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = (((src_val < 0) && (dst_val < 0) && (value >=0)) || 
					((src_val >= 0) && (dst_val >= 0) && (value < 0)));

    psw.carry = (value >> 17 & 1 == 1);
    return 0;
}


int op_sub(uint16_t instruction)
{
    uint8_t src = (instruction >> 6) & 077; // bits 6-8
    uint8_t dst = instruction & 077; // bits 0-2
    uint8_t dst_reg = instruction & 07;
    int32_t value;

    log(LOG_INST, "SUB function called\n");

    uint16_t src_addr, dst_addr;

    int16_t src_val = operand_value_read_word(src, &src_addr);
    int16_t dst_val = operand_value_read_word(dst, &dst_addr);

    value = dst_val - src_val;

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;
    
	/*
	N: set if result <0; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: set if there was arithmetic overflow as a result of the oper·
	ation; that is both operands were of the same sign and the
	result was of the opposite sign; cleared otherwise
	C: set if there was a carry from the most significant bit of the
	result; cleared otherwise 
	*/
	
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = (((src_val < 0) && (dst_val < 0) && (value >=0)) || 
					((src_val >= 0) && (dst_val >= 0) && (value < 0)));

    psw.carry = !(value >> 17 & 1 == 1);
    return 0;
}


//logical
int op_bit(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    uint8_t dst_reg = instruction & 07;
    uint8_t dst_mode = instruction & 070;
    
    int16_t value;
    
    log(LOG_INST, "BIT function called\n");

	
    uint16_t src_addr, dst_addr;

    int16_t src_val = operand_value_read_word(src, &src_addr);
    int16_t dst_val = operand_value_read_word(dst, &dst_addr);
    
    //Read values for source and destination
    log(LOG_INFO,"Source value is %d, Dest value is %d\n", src_val, dst_val);
    
    value = src_val & dst_val;

	log(LOG_INFO,"result is %o\n", value);

	/*  N: set if high-order bit of result set: cleared otherwise
		Z: set if result = 0; cleared otherwise
		V: cleared
		C: not affected 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

	return 0;
}


int op_bitb(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    //uint8_t dst_reg = instruction & 07;
    int16_t value;
    
    log(LOG_INST, "BITB function called\n");
    
    uint16_t src_addr, dst_addr;

    int8_t src_val = operand_value_read_byte(src, &src_addr);
    int8_t dst_val = operand_value_read_byte(dst, &dst_addr);

    log(LOG_INFO,"Source value is %o, Dest value is %o\n", src_val, dst_val);
    
    value = src_val & dst_val;
    
	log(LOG_INFO,"Result is %o\n", value);

	/*
	N: set if high-order bit of result set: cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: cleared
	C: not affected 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

	return 0;
}


int op_bic(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    //uint8_t dst_reg = instruction & 07;
    int16_t value;
    
    log(LOG_INST, "BIC function called\n");

    uint16_t src_addr, dst_addr;

    int16_t src_val = operand_value_read_word(src, &src_addr);
    int16_t dst_val = operand_value_read_word(dst, &dst_addr);	

    log(LOG_INFO,"Source value is %o, Dest value is %o\n", src_val, dst_val);
    
    value = ~(src_val) & dst_val;
	log(LOG_INFO,"result is %o\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;


	/*
	N: set if high order bit of result set; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: cleared
	C: not affected 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

	return 0;
}


int op_bicb(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    uint8_t dst_reg = instruction & 07;
    int16_t value;
    
    log(LOG_INST, "BIC function called\n");

    uint16_t src_addr, dst_addr;

    int8_t src_val = operand_value_read_byte(src, &src_addr);
    int8_t dst_val = operand_value_read_byte(dst, &dst_addr);

    log(LOG_INFO,"Source value is %o, Dest value is %o\n", src_val, dst_val);
    
    value = ~(src_val) & dst_val;
	log(LOG_INFO,"result is %o\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register


	/*
	N: set if high order bit of result set; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: cleared
	C: not affected 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

	return 0;
}


int op_bis(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    uint8_t dst_reg = instruction & 07;
    int16_t value;
    
    log(LOG_INST, "BIS function called\n");

    uint16_t src_addr, dst_addr;

    int16_t src_val = operand_value_read_word(src, &src_addr);
    int16_t dst_val = operand_value_read_word(dst, &dst_addr);

    log(LOG_INFO,"Source value is %d, Dest value is %d\n", src_val, dst_val);
    
    value = src_val | dst_val;
	log(LOG_INFO,"Result is %o\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;	
	/*
	N: set if high-order bit of result set. cleared otherwise
	Z: set if result = 0: cleared otherwise
	V: cleared
	C: not affected 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

	return 0;
}


int op_bisb(uint16_t instruction)
{
	uint8_t src = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    uint8_t dst_reg = instruction & 07;
    int16_t value;
    
    log(LOG_INST, "BIS function called\n");
    uint16_t src_addr, dst_addr;

    int8_t src_val = operand_value_read_byte(src, &src_addr);
    int8_t dst_val = operand_value_read_byte(dst, &dst_addr);

    log(LOG_INFO,"Source value is %d, Dest value is %d\n", src_val, dst_val);
    
    value = src_val | dst_val;
	log(LOG_INFO,"result is %o\n", value);

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_byte(dst_addr, value);
    else
		reg[dst] = (reg[dst] & 0177400) | value; // only modify the low byte of register
	/*
	N: set if high-order bit of result set. cleared otherwise
	Z: set if result = 0: cleared otherwise
	V: cleared
	C: not affected 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;

	return 0;
}


//Register
/*
The contents of the destination register and source taken as
two's complement integers are multiplied and stored in the
destination register and the succeeding register (if R is even).
If R is odd only the low order product is stored. 
 */
int op_mul(uint16_t instruction)
{
    uint8_t dst_reg = (instruction >> 6) & 07; // bits 6-8
    //uint8_t src_reg = instruction & 07; // bits 0-2
    //uint8_t dst = (instruction >> 6) & 077; // bits 6-11
    uint8_t src = (instruction) & 077; // bits 0-5
    int32_t value;
    uint16_t src_value;
    
    log(LOG_INST, "MUL function called\n");
       
	//call read/write operand with byte = 0 since there are no byte modes

    uint16_t src_addr;
    int16_t src_val = operand_value_read_word(src, &src_addr);
	int16_t reg_val = reg[dst_reg];

    log(LOG_INFO,"Source value is %o, reg[%o] is %o\n", src_val, dst_reg, reg_val);
    value = reg_val * src_val;
	log(LOG_INFO,"Result is %d\n", value);
	
	if(dst_reg % 2)
	{
		log(LOG_INFO,"Odd destination register %o\n", dst_reg);
		reg[dst_reg] = value;
	}
	else
	{
		log(LOG_INFO,"Even destination register %o\n", dst_reg);
		reg[dst_reg] = value;
		reg[dst_reg|1] = value >> 16;
    }
	/*
	N: set if product is <0: cleared otherwise
	Z: set if product is 0; cleared otherwise
	V: cleared
	C: set if the result is less than-2^15 or greater than or equal to
	2^15-1: 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;
	if((value < -2^15) || (value >= 2^15 -1))
		psw.carry = 0;
	
    return 0;
}

/*
The 32-bit two's complement integer in R and Rv1 is divided
by the source operand. The quotient is left in R; the remainder
in Rv1. Division will be performed so that the remainder
is of the same sign as the dividend. R must be even. 
 */ 
int op_div(uint16_t instruction){
    uint8_t dst_reg = (instruction >> 6) & 07; // bits 6-8
    //uint8_t src_reg = instruction & 07; // bits 0-2
    //uint8_t dst = (instruction >> 6) & 077; // bits 6-11
    uint8_t src = (instruction) & 077; // bits 0-5
    int32_t value, rem;
    uint16_t src_addr;
    
    log(LOG_INST, "DIV function called\n");
    if(dst_reg % 2)
	{
		log(LOG_INFO,"Odd destination register '%o' not allowed\n", dst_reg);
		return 0;
	}

	//Value to be divided is held in two registers
    int16_t reg_val1 = reg[dst_reg];
    int16_t reg_val2 = reg[dst_reg|1];
    int32_t comb_val = (reg_val1 << 16) + reg_val2;
    int16_t src_val = operand_value_read_word(src, &src_addr);
    
    log(LOG_INFO,"Source value is %o, combined register value is %o\n", src_val, comb_val);
	
	if(src_val == 0) {
		psw.carry = 1;
		psw.overflow = 1;
		log(LOG_INFO,"Divide by 0 attempted! Cannot complete instruction!\n");
		return 0;
	}
	if(abs(comb_val) > abs(src_val)) {
		log(LOG_INFO,"Quotient will exceed 15 bits! Abort!\n");
		psw.overflow = 1;
		return 0;
	}

    value = comb_val / src_val;
	rem = comb_val % src_val;

	log(LOG_INFO,"Quotient is %d\n", value);
	log(LOG_INFO,"Remainder is %d\n", rem);

	reg[dst_reg] = value;
	reg[dst_reg|1] = rem;

	/*
	N: set if quotient <0; cleared otherwise
	Z: set if quotient = 0; cleared otherwise
	V: set if source = 0 or if the absolute value of the register is
	larger than the absolute value of the source. (In this case the
	instruction is aborted because the quotient would exceed 15
	bits.)
	C: set if divide 0 attempted; cleared otherwise 
	*/
    psw.negative = (value < 0);
    psw.zero = (value == 0);
	psw.carry = 0;
	
    return 0;
}
/*
The contents of the register are shifted right or left the number
of times specified by the shift count. The shift count is
taken as the low order 6 bits of the source operand. This
number ranges from -32 to + 31. Negative is a a right shift
and positive is a left shift. 
*/
int op_ash(uint16_t instruction){
	uint8_t dst_reg = (instruction >> 6) & 07; // bits 6-8
    //uint8_t src_reg = instruction & 07; // bits 0-2
    //uint8_t dst = (instruction >> 6) & 077; // bits 6-11
    uint8_t src = (instruction) & 077; // bits 0-5
    uint8_t carry;
    int16_t value;
    uint8_t shift_val, shift_sign;
    int8_t converted_shift;

    uint16_t src_addr;
    int16_t src_val = operand_value_read_word(src, &src_addr);
    
    //uint16_t src_val = operand_value_read_word(src); //number of bits to shift
    int16_t dst_val = reg[dst_reg]; //register val to be shifted
    log(LOG_INST, "ASH function called\n");
    //log(LOG_INFO, "Source contains %o\n", src_val);
    shift_val = src_val & 077; //lower 6 bits of src value for shift
    shift_sign = (shift_val >> 5);
    
    if(shift_sign){
		shift_val = shift_val | 0300;
	}
    converted_shift = shift_val;
    //shift_val = shift_val & 037;
    log(LOG_INFO, "Shifting R%d contents(%o) by %d\n", dst_reg, dst_val, converted_shift);
    
    if(shift_sign){
		//shift right
		log(LOG_INFO, "Shifting Right\n");
		carry = (dst_val >> abs(converted_shift) - 1) & 01;
		value = dst_val >> abs(converted_shift);
	}
	else{
		//shift left
		log(LOG_INFO, "Shifting left\n");
		carry = ((dst_val << converted_shift - 1) >> 15) & 01;
		value = dst_val << converted_shift;
		
	}
	log(LOG_INFO, "new value is %o\n", value);
    reg[dst_reg] = value;
    /*
	N: set if result <0; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: set if sign of register changed during shift; cleared otherwise
	C: loaded from last bit shifted out of register 
	*/
	
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = (value << 15 != dst_val << 15);
	psw.carry = carry;
	return 0;
}

/*

The double word is shifted NN places to the
right or left, where NN =low order six bits or source 

The contents of the register and the register ORed with one
are treated as one 32 bit word, R + 1 (bits 0-15) and R (bits
16-31) are shifted right or left the number of times specified
by the shift count The shift count is taken as the low order 6
bits of the source operand. This number ranges from -32 to
+ 31. Negative is a right shift and positive is a left shift
When the register chosen is an odd number the register
and the register OR'ed with one are the same. In this case the
right shift becomes a rotate (for up to a shift of 16). The 16
bit word is rotated right the number of bits specified by the
shift count 
 */
int op_ashc(uint16_t instruction){
	uint8_t dst_reg = (instruction >> 6) & 07; // bits 6-8
    //uint8_t src_reg = instruction & 07; // bits 0-2
    //uint8_t dst = (instruction >> 6) & 077; // bits 6-11
    uint8_t shift_val = (instruction) & 077; // bits 0-5
    uint8_t carry;
    int32_t value;
    uint8_t shift_sign;
    int8_t converted_shift;

    //uint16_t src_val = operand_value_read_word(src); //number of bits to shift
    uint16_t dst_val1 = reg[dst_reg];
    uint16_t dst_val2 = reg[dst_reg|1];
    int32_t dst_val = (dst_val1 << 16) | dst_val2; //register val to be shifted
    log(LOG_INST, "ASHC function called\n");

    shift_sign = (shift_val >> 5);
    
    if(shift_sign){
		shift_val = shift_val | 0300;
	}
    converted_shift = shift_val;
    //shift_val = shift_val & 037;
    log(LOG_INFO, "Shifting R%d contents(%o) by %d\n", dst_reg, dst_val, converted_shift);
    
    if(shift_sign){
		//shift right
		log(LOG_INFO, "Shifting Right\n");
		carry = (dst_val >> abs(converted_shift) - 1) & 01;
		value = dst_val >> abs(converted_shift);
	}
	else{
		//shift left
		log(LOG_INFO, "Shifting left\n");
		carry = ((dst_val << converted_shift - 1) >> 31) & 01;
		value = dst_val << converted_shift;
		
	}
	log(LOG_INFO, "new value is %o\n", value);
	reg[dst_reg] = value >> 16;
    reg[dst_reg|1] = value & 0177777;

    /*
	N: set if result <0; cleared otherwise
	Z: set if result = 0; cleared otherwise
	V: set if sign of register changed during shift; cleared otherwise
	C: loaded from last bit shifted out of register 
	*/
	
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = (value << 31 != dst_val << 31);
	psw.carry = carry;
	return 0;
}


/*
The exclusive OR of the register and destination operand is
stored in the destination address. Contents of register are
unaffected. Assembler format is: XOR R.D 
*/
int op_xor(uint16_t instruction){
	uint8_t reg_num = (instruction >> 6) & 07; // bits 6-8
    //uint8_t dst_reg = instruction & 07; // bits 0-2
    //uint8_t dst = (instruction >> 6) & 077; // bits 6-11
    uint8_t dst = (instruction) & 077; // bits 0-5
    
    int16_t value;
    uint16_t dst_addr;

    value = operand_value_read_word(dst, &dst_addr);

    log(LOG_INST, "XOR function called\n");
    value = reg[reg_num] ^ value;

    if ( dst_addr != 0 ) // dst was a deferred address
        data_write_word(dst_addr, value);
    else
        reg[dst] = value;

    /*
	N: set if the result <0: cleared otherwise
	Z set if result = 0: cleared otherwise
	V: cleared
	C: unaffected 
    */
    psw.negative = (value < 0);
    psw.zero = (value == 0);
    psw.overflow = 0;
	return 0;
    
}

//BRANCHES
int op_br(uint16_t instruction)
{
	//Unconditional branch
    log(LOG_INST, "BRANCH function called\n");

    int offset = instruction - 0400;
    printf("offset is %o\n", offset);
    reg[7] += (2* offset);
    log(LOG_DEBUG, "new number! %o\n", reg[7]);

    return 0;
}


int op_bne(uint16_t instruction)
{
	//Branches if Z bit is clear
    log(LOG_INST, "Branch Not Equal function called\n");
	if(psw.zero == 0){
		int offset = instruction - 01000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
    return 0;
}


int op_beq(uint16_t instruction)
{
	//Branches if Z bit set
    log(LOG_INST, "Branch Equals (Zero) function called\n");
	if(psw.zero == 1){
		int offset = instruction - 01400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
    return 0;
}


int op_bpl(uint16_t instruction)
{
	//Branches if N bit clear
    log(LOG_INST, "Branch Plus function called\n");
	if(psw.negative == 0){
		int offset = instruction - 0100000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}


int op_bmi(uint16_t instruction)
{
	//Branches if N is set
    log(LOG_INST, "Branch Minus function called\n");
	if(psw.negative == 1){
		int offset = instruction - 0100400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}


int op_bvc(uint16_t instruction)
{
	//Branches if V is clear
    log(LOG_INST, "Branch Overflow Cleared called\n");
	if(psw.overflow == 0){
		int offset = instruction - 0102000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}


int op_bvs(uint16_t instruction)
{
	//Branches if V is set
    log(LOG_INST, "Branch Overflow Set called\n");
	if(psw.overflow == 1){
		int offset = instruction - 0102400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
int op_bcc(uint16_t instruction)
{
	//Branches if C is clear
    log(LOG_INST, "Branch Carry Cleared called\n");
	if(psw.carry == 0){
		int offset = instruction - 0103000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
int op_bcs(uint16_t instruction)
{
	//Branches if C is set
    log(LOG_INST, "Branch Carry Set called\n");
	if(psw.carry == 1){
		int offset = instruction - 0103400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
//Signed conditional branches
int op_bge(uint16_t instruction)
{
	//Branches if N and V are both set or if they are both clear
    log(LOG_INST, "Branch if Greater or equal called\n");
	if(psw.negative == psw.overflow){
		int offset = instruction - 02000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
int op_blt(uint16_t instruction)
{
	//Branches if N xor V 1
    log(LOG_INST, "Branch if Less Than\n");
	if(psw.negative ^ psw.overflow == 1){
		int offset = instruction - 02400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
int op_bgt(uint16_t instruction)
{
	//Branches if Z xor (V xor N) is Zero
    log(LOG_INST, "Branch if Greater Than called\n");
	if(psw.zero^(psw.negative ^ psw.overflow) == 0){
		int offset = instruction - 03000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
int op_ble(uint16_t instruction)
{
	//Branches if Z xor (V xor N) is One
    log(LOG_INST, "Branch if Less or equal called\n");
	if(psw.zero^(psw.negative ^ psw.overflow) == 1){
		int offset = instruction - 03400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}

//Unsigned conditional branches
int op_bhi(uint16_t instruction)
{
	//Branches if both c and z are zero
    log(LOG_INST, "Branch if higher called\n");
	if(psw.carry == 0 && psw.zero == 0){
		int offset = instruction - 0101000;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}
int op_blos(uint16_t instruction)
{
	//Branches if C or Z is set
    log(LOG_INST, "Branch if lower or same\n");
	if(psw.carry || psw.zero == 1){
		int offset = instruction - 0101400;
		log(LOG_DEBUG, "offset is! %o\n", offset);
		reg[7] += (2* offset);
		log(LOG_DEBUG, "new PC! %o\n", reg[7]);
	}
	else
		log(LOG_DEBUG, "Branch not taken!\n");
		
    return 0;
}


//Jump
int op_jmp(uint16_t instruction){
	uint8_t dst = (instruction) & 077; // bits 0-5
	uint8_t dst_reg = (instruction) & 07; // bits 0-2
    uint8_t dst_mode = (instruction >> 3) & 07; // bits 3-5
	log(LOG_INST, "Jump function called\n");
	
	//Illegal instruction if destination mode is 0 (cannot pass control
	//to a register)
	if(dst_mode == 0)
	{
		log(LOG_INFO, "Destination mode 0! Cannot pass control to register!\n");
		return 0;
	}
	
    uint16_t dst_addr;
    uint16_t dst_val = operand_value_read_word(dst, &dst_addr);
    reg[7] = (uint16_t)dst_addr;
    log(LOG_INFO, "PC value changed to: %o\n", dst_addr);
    return 0;
}



//Halt
int op_halt(uint16_t instruction)
{
    log(LOG_INST, "HALT function called\n");
    return 0;
}
