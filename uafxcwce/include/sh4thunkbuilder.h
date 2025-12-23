//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
enum intRegEnum    {R0=0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,NumIntRegs};
enum floatRegEnum  {FR0=0,FR1,FR2,FR3,FR4,FR5,FR6,FR7,FR8,FR9,FR10,FR11,FR12,FR13,FR14,FR15,NumFloatRegs};
enum doubleRegEnum {DR0=0,DR2=2,DR4=4,DR6=6,DR8=8,DR10=10,DR12=12,DR14=14};

#define REG_USED   1
#define REG_UNUSED 0

//////////////////////////////////////////////////////////////////////////////
// debugging toys

#ifdef THUNKDEBUG
#define OUTPUT printf               // define to enable print output
#else 
//we'll nop the OUTPUT function.
//Since OUTPUT takes variable arguments, assign it to 0
#if _MSC_VER >= 1210
#define OUTPUT   __noop
#else
#define OUTPUT   0
#endif
#endif //THUNKDEBUG

#ifdef THUNKDEBUG
#define CHECK(p) check(p,__LINE__)  // define to enable assertion checking
#else //THUNKDEBUG
//we'll nop the CHECK function
#define CHECK(p)
#endif //THUNKDEBUG

#ifdef THUNKDEBUG
#define BUFFER_PRINT    // define to see the buffer before it is called

#ifndef OUTPUT
#define OUTPUT 
#endif

#ifndef CHECK
#define CHECK
#else
void check(int expr,unsigned line)
{
    if(!expr) OUTPUT("ERROR on line: %d\n",line);
}
#endif //CHECK
#endif //THUNKDEBUG

// we need a special type that will hold information on arg types
enum typeEnum      {IntType=0, FloatType, DoubleType};
struct DispatchArgument{
    typeEnum ty;
    unsigned size;
};
////////////////////////////////////////////////////////////////



// class to maintain register state
class AtlceRegisterState{

private:
    char intArgRegs[NumIntRegs];
    char floatArgRegs[NumFloatRegs];
    intRegEnum nextIntReg;
    floatRegEnum nextFloatReg;
    doubleRegEnum nextDoubleReg;

    //
    // Mark the next int argument register as used
    // This does not require any change to the
    // next available float or double arg reg.
    //
    _inline void useIntReg(void){
        CHECK(nextIntReg<=R7);
        CHECK(intArgRegs[nextIntReg] == REG_UNUSED);

        intArgRegs[nextIntReg] = REG_USED;
        nextIntReg = (intRegEnum)(nextIntReg+1);
    }

    //
    // Mark the next available float register as used
    // - also update the next available int and double registers
    //
    void useFloatReg(void){
        CHECK(nextFloatReg <= FR11);
        CHECK(floatArgRegs[nextFloatReg] == REG_UNUSED);

        floatArgRegs[nextFloatReg] = REG_USED;

        //
        // update the int pointer
        //
        if(nextIntReg <= R7) nextIntReg = (intRegEnum)(nextIntReg+1);

        //
        // update next available double if use of float interferes with it.
        //
        while(nextDoubleReg <= DR10 &&
              (*((short*)(&floatArgRegs[nextDoubleReg])) != REG_UNUSED))
            nextDoubleReg = (doubleRegEnum)(nextDoubleReg+2);

        //
        // update next available float
        // note that doubles can cause later registers to be used
        //
        nextFloatReg = (floatRegEnum)(nextFloatReg+1);
        while((nextFloatReg <= FR11)&&
              (floatArgRegs[nextFloatReg]==REG_USED))
            nextFloatReg = (floatRegEnum)(nextFloatReg+1);
    }

    //
    // Mark the next available double register as used
    // - also update the next available int and float registers
    //
    void useDoubleReg(void){
        CHECK(nextDoubleReg <= DR10);
        CHECK((floatArgRegs[nextDoubleReg] == REG_UNUSED) &&
              (floatArgRegs[nextDoubleReg+1] == REG_UNUSED));

        // kill reg reg pair used by double
        *((short*)(&floatArgRegs[nextDoubleReg])) = (REG_USED | (REG_USED<<8));

        // update the int pointer
        if(nextIntReg <= R6) nextIntReg = (intRegEnum)(nextIntReg+2);
        else if(nextIntReg <= R7) nextIntReg = (intRegEnum)(nextIntReg+1);

        // update float pointer if use of double interferes with it
        //
        // two fp regs were just used to pass a double, which means that
        // nextFloatReg WAS pointing to a valid register (either the
        // killed double or a single float before it).
        // If float was killed by this double write, then nextFloatReg
        // pointed to an even float register, and the next float
        // register was also killed.
        // This means that we can increment nextFloatReg by 2. That
        // register is either out of range, or untaken.
        while((nextFloatReg<=FR11)&&
              (floatArgRegs[nextFloatReg]==REG_USED))
            nextFloatReg = (floatRegEnum)(nextFloatReg+1);

        nextDoubleReg=(doubleRegEnum)(nextDoubleReg+2);
    }


    _inline int intArgRegAvailable(void){
        return (nextIntReg <= R7);
    }

    _inline intRegEnum getAvailableIntArgReg(void){
        CHECK(intArgRegAvailable());
        return nextIntReg;
    }

    _inline int floatArgRegAvailable(void){
        return (nextFloatReg <= FR11);
    }

    _inline floatRegEnum getAvailableFloatArgReg(void){
        CHECK(floatArgRegAvailable());
        return nextFloatReg;
    }

    _inline int doubleArgRegAvailable(void){
        return (nextDoubleReg <= DR10);
    }

    _inline doubleRegEnum getAvailableDoubleArgReg(void){
        CHECK(doubleArgRegAvailable());
        return nextDoubleReg;
    }

    _inline unsigned short opCode_MovlImmedDispLoad(intRegEnum dstReg,
                                          intRegEnum baseReg,
                                          unsigned disp){
        CHECK(disp % 4 == 0);
        CHECK((disp >= 0) && (disp <= 60));
        OUTPUT("mov.l  @(%d,R%d),R%d\n",disp,baseReg,dstReg);
        return (0x5000 | (dstReg << 8) | (baseReg << 4) | (disp/4));
    }

    _inline unsigned short opCode_FmovDispLoad(intRegEnum baseReg,
                                               floatRegEnum dstReg){
        OUTPUT("fmov.s @(R%d,R0), FR%d\n",baseReg,dstReg);
        return (0xf000 | (dstReg << 8) | (baseReg << 4) | 0x6);
    }

    _inline unsigned short opCode_Movi(char constant, intRegEnum dstReg){
        CHECK(constant >= -128 && constant <= 127);
        OUTPUT("mov    #%d, R%d\n",constant,dstReg);
        return (0xe000 | (dstReg << 8) | *((unsigned char*)&constant));
    }

    _inline unsigned short opCode_Mov(intRegEnum srcReg, intRegEnum dstReg){
        OUTPUT("mov    R%d, R%d\n",srcReg,dstReg);
        return (0x6000 | (dstReg << 8) | (srcReg << 4) | 0x3);
    }

    _inline unsigned short opCode_Add(intRegEnum srcReg, intRegEnum dstReg){
        OUTPUT("add    R%d, R%d\n",srcReg,dstReg);
        return (0x3000 | (dstReg << 8) | (srcReg << 4) | 0xc);
    }

    _inline unsigned short opCode_Add(char constant, intRegEnum dstReg){
        CHECK(constant >= -128 && constant <= 127);
        OUTPUT("add    #%d, R%d\n",constant,dstReg);
        return (0x7000 | (dstReg << 8) | *((unsigned char*)&constant));
    }

    _inline unsigned short opCode_Jmp(intRegEnum srcReg){
        OUTPUT("jmp    @R%d\n",srcReg);
        return (0x4000 | ((unsigned)srcReg << 8) | 0x2b);
    }

    _inline unsigned short opCode_Nop(void){
        OUTPUT("nop\n");
        return (0x9);
    }

    _inline unsigned short opCode_Bra(int instructions){
        OUTPUT("bra %d\n",(instructions-2)*2);
        return (0xa000 | (instructions-2));
    }

    _inline unsigned short opCode_DataW(unsigned short data){
        OUTPUT("data.w 0x%x\n",data);
        return data;
    }

    _inline unsigned short opCode_PCMovW(unsigned char instructions,
                                         intRegEnum dstReg){
        OUTPUT("mov.w  @(%d,pc),r0\n",instructions -2);
        return (0x9000 | dstReg<<8 | instructions-2);
    }

    // sets R0 to requested constant
    //
    // emits following instructions:
    //   mov.w @(2,pc),r0
    //   bra   +2
    //   nop
    //   .dataw immediate
    //
    unsigned short* setR0(unsigned newValue, unsigned short* b){
        *b++ = opCode_PCMovW(3,R0);
        *b++ = opCode_Bra(3);        // bra + 2
        *b++ = opCode_Nop();         // nop
        *b++ = opCode_DataW(newValue&0xffff);
        return b;
    }

public:

    void createCall(struct DispatchArgument *argList, unsigned argCount, unsigned* buffer){

        unsigned int stackOffset = 0;

        unsigned short* b = (unsigned short*)buffer;
        unsigned tSize,tStack;

        unsigned r0HasVal = 0;

        // thunk takes two arguments:
        // R4 - pFn, the function that we are jumping to
        // R5 - newStack, the value that we will set the stack to
        
        *b++ = opCode_Mov(R4, R2);    // mov R4, R2
        *b++ = opCode_Mov(R5, R15);   // mov R5, SP

        // Argument list is in reverse order,
        // process argument list from back to
        // front.
        argList = &argList[argCount-1];
        while(argCount-- > 0)
        {
            CHECK(stackOffset%4==0);
            switch(argList->ty)
            {
            case IntType:                
                tSize = argList->size;
                tStack = stackOffset;
                while((tSize > 0) &&
                      (intArgRegAvailable()))
                {
                    // Note:
                    //   immediate displacement can never be larger than
                    //   16 so disp will always fit in immediate field

                    // mov.l @(R15,stackOffset),ArgReg
                    *b++ = opCode_MovlImmedDispLoad(getAvailableIntArgReg(),
                                                    R15,
                                                    tStack); 
                    useIntReg();
                    tSize-=4;
                    tStack+=4;
                }
                stackOffset+=(((argList->size*4)+3)/4);
                break;

            case FloatType:
                CHECK(argList->size==4);
                if(floatArgRegAvailable())
                {
                    // set up R0 for use with fmov.s
                    b = setR0(stackOffset,b);

                    // load float arg into float arg reg

                    // fmov.s (R0, R15), floatArgReg
                    *b++ = opCode_FmovDispLoad(R15,getAvailableFloatArgReg()); 
                    useFloatReg();
                }
                stackOffset+=4;
                break;

            case DoubleType:
                CHECK(argList->size==8);
                if(doubleArgRegAvailable())
                {
                    b = setR0(stackOffset,b);

                    // load double arg into double register

                    // fmov.s @(R0, R15), floatArgReg+1
                    // add    #4, R0
                    // fmov.s @(R0, R15), floatArgReg
                    *b++ = opCode_FmovDispLoad(R15,(floatRegEnum)(getAvailableDoubleArgReg()+1));
                    *b++ = opCode_Add(4,R0);
                    *b++ = opCode_FmovDispLoad(R15,(floatRegEnum)getAvailableDoubleArgReg());
                    useDoubleReg();
                }
                stackOffset+=8;
                break;
            }
            argList--;
        }

        // end of thunk, jump to selected function
        *b++ = opCode_Jmp(R2);
        *b++ = opCode_Nop();
        *b++ = 0;
    }

    void reset(void){
        nextIntReg = R4;
        nextFloatReg = FR4;
        nextDoubleReg = DR4;

        int i;
        for(i=0;i<NumIntRegs;i++)
            intArgRegs[i] = REG_UNUSED;

        for(i=0;i<NumFloatRegs;i++)
            floatArgRegs[i] = REG_UNUSED;
    }

    AtlceRegisterState(void){
        reset();
    };

};

