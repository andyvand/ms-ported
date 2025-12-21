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
enum intRegEnum    {ZR=0,AT,V0,V1,A0,A1,A2,A3,T0,T1,T2,T3,T4,T5,T6,T7,S0,S1,S2,S3,S4,S5,S6,S7,T8,T9,K0,K1,GP,SP,S8,RA,NumIntRegs};
enum floatRegEnum  {FV0=0,FT6,FV1,FT7,FT0,FT8,FT1,FT9,FT2,FT10,FT3,FT11,FA0,FA2,FA1,FA3,FT4,FT12,FT5,FT13,FS0,FT14,FS1,FT15,FS2,FT16,FS3,FT17,FS4,FT18,FS5,FT19,NumFloatRegs};

#define REG_USED   1
#define REG_UNUSED 0

//this def sets the size of the thunk buffer. for MIPS this is 32 bytes
#define THUNKBUFFERSIZE 32

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
class AfxRegisterState{

private:
    char intArgRegs[NumIntRegs];
    char floatArgRegs[NumFloatRegs];
    intRegEnum nextIntReg;
    floatRegEnum nextFloatReg;
    floatRegEnum nextDoubleReg;
    int LeadingIntegerArguments;

    //
    // Mark the next int argument register as used
    // This does not require any change to the
    // next available float or double arg reg.
    //
    _inline void useIntReg(void){
        CHECK(nextIntReg<=A3);
        CHECK(intArgRegs[nextIntReg] == REG_UNUSED);

        intArgRegs[nextIntReg] = REG_USED;
        nextIntReg = (intRegEnum)(nextIntReg+1);
    }

    //
    // Mark the next available float register as used
    // - also update the next available int and double registers
    //
    void useFloatReg(void){
        CHECK(nextFloatReg <= FA1);
        CHECK(floatArgRegs[nextFloatReg] == REG_UNUSED);

        floatArgRegs[nextFloatReg] = REG_USED;

        //
        // update the int pointer
        //
        if(nextIntReg <= A3) nextIntReg = (intRegEnum)(nextIntReg+1);

        //
        // update next available double if use of float interferes with it.
        //
        if (nextDoubleReg <= FA1)
            nextDoubleReg = (floatRegEnum)(nextDoubleReg+2);

        //
        // update next available float
        //
        nextFloatReg = (floatRegEnum)(nextFloatReg+2);
    }

    //
    // Mark the next available double register as used
    // - also update the next available int and float registers
    //
    void useDoubleReg(void){
        CHECK(nextDoubleReg <= FA1);
        CHECK((floatArgRegs[nextDoubleReg] == REG_UNUSED) &&
              (floatArgRegs[nextDoubleReg+1] == REG_UNUSED));

        // kill reg reg pair used by double
        *((short*)(&floatArgRegs[nextDoubleReg])) = (REG_USED | (REG_USED<<8));

        // update the int pointer
        if(nextIntReg <= A3) nextIntReg = (intRegEnum)(nextIntReg+2);
        // else if(nextIntReg <= A3) nextIntReg = (intRegEnum)(nextIntReg+1);

        // update float pointer if use of double interferes with it
        //
        if (nextFloatReg<=FA1)
            nextFloatReg = (floatRegEnum)(nextFloatReg+2);

        nextDoubleReg=(floatRegEnum)(nextDoubleReg+2);
    }


    _inline int intArgRegAvailable(void){
        return (nextIntReg <= A3);
    }

    _inline intRegEnum getAvailableIntArgReg(void){
        CHECK(intArgRegAvailable());
        return nextIntReg;
    }

    _inline int floatArgRegAvailable(void){
        return (nextFloatReg <= FA1);
    }

    _inline floatRegEnum getAvailableFloatArgReg(void){
        CHECK(floatArgRegAvailable());
        return nextFloatReg;
    }

    _inline int doubleArgRegAvailable(void){
        return (nextDoubleReg <= FA1);
    }

    _inline floatRegEnum getAvailableDoubleArgReg(void){
        CHECK(doubleArgRegAvailable());
        return nextDoubleReg;
    }

    _inline int getLeadingIntegerArguments(void){
        return LeadingIntegerArguments;
    }

    _inline void setLeadingIntegerArguments(void){
        LeadingIntegerArguments = TRUE;
    }


    _inline unsigned int opCode_LoadWord(intRegEnum dstReg,
                                         intRegEnum baseReg,
                                         unsigned offset){
        CHECK(offset % 4 == 0);
        CHECK((offset >= 0) && (offset <= 0xffff));
        OUTPUT("lw  %%%d, %d(%%%d)\n",dstReg,offset,baseReg);
        return (0x8C000000 | (dstReg << 16) | (baseReg << 21) | (offset));
    }

    _inline unsigned int opCode_LoadFloat(floatRegEnum dstReg,
                                                intRegEnum baseReg,
                                                unsigned offset ){
        CHECK(offset % 4 == 0);
        CHECK((offset >= 0) && (offset <= 0xffff));
        OUTPUT("lwc1  %%f%d, %d(%%%d)\n",dstReg,offset,baseReg);
        return (0xC4000000 | (dstReg << 16) | (baseReg << 21) | (offset));
    }

    _inline unsigned int opCode_LoadDouble(floatRegEnum dstReg,
                                           intRegEnum baseReg,
                                           unsigned offset ){
        CHECK(offset % 8 == 0);
        CHECK((offset >= 0) && (offset <= 0xffff));
        OUTPUT("ldc1  %%f%d, %d(%%%d)\n",dstReg,offset,baseReg);
        return (0xD4000000 | (dstReg << 16) | (baseReg << 21) | (offset));
    }


    _inline unsigned int opCode_Mov(intRegEnum dstReg, intRegEnum srcReg){
        OUTPUT("addiu    %%%d, %%%d, 0\n",dstReg,srcReg);
        return (0x24000000 | (dstReg << 16) | (srcReg << 21));
    }

    _inline unsigned int opCode_Jmp(intRegEnum srcReg){
        OUTPUT("jr    %%%d\n",srcReg);
        return (0x00000008 | (srcReg << 21)) ;
    }

    _inline unsigned int opCode_Nop(void){
        OUTPUT("nop\n");
        return (0x00000000);
    }

public:

    void createCall(struct DispatchArgument *argList, unsigned argCount, unsigned* buffer){

        unsigned int stackOffset = 0;

        unsigned * b = buffer;
        unsigned tSize,tStack;

        unsigned r0HasVal = 0;

        // thunk takes two arguments:
        // A0 - pFn, the function that we are jumping to
        // A1 - newStack, the value that we will set the stack to
        
        *b++ = opCode_Mov(T6, A0);  // mov T6, A0(addiu t6, a0, 0)
        *b++ = opCode_Mov(SP, A1);  // mov SP, A1(addiu sp,  s1, 0)

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

                // If the argument is a int64, we need 
                // to adjust the register number for alignment
                if (tSize > 4 &&
                    (intArgRegAvailable()) &&
                    ((getAvailableIntArgReg() & 1) != 0))
                {
                    useIntReg();
                    stackOffset+=4;
                }
                
                tStack = stackOffset;
    
                while((tSize > 0) &&
                      (intArgRegAvailable()))
                {
                    // lw  ArgReg, StackOffset(SP)
                    *b++ = opCode_LoadWord (getAvailableIntArgReg(),
                                            SP, tStack); 
                    useIntReg();
                    tSize-=4;
                    tStack+=4;
                }
                stackOffset+=(((argList->size*4)+3)/4);
                setLeadingIntegerArguments();
                break;

            case FloatType:
                CHECK(argList->size==4);
                if (getLeadingIntegerArguments())
                {
                    if (intArgRegAvailable())
                    {
                        // load float arg into int arg reg
                        *b++ = opCode_LoadWord (getAvailableIntArgReg(),
                                                SP, stackOffset);
                        useIntReg();
                    }
                }
                else 
                if(floatArgRegAvailable())
                {
                    // load float arg into float arg reg

                    // lwc1  floatArgReg,  StackOffset(SP)
                    *b++ = opCode_LoadFloat(getAvailableFloatArgReg(),
                                                SP, stackOffset); 
                    useFloatReg();
                }
                stackOffset+=4;
                break;

            case DoubleType:
                CHECK(argList->size==8);
                if (getLeadingIntegerArguments())
                {
                    // For a double argument, we need
                    // to adjust the register number for alignment
                    if ((intArgRegAvailable()) &&
                        ((getAvailableIntArgReg() & 1) != 0))
                    {
                        useIntReg();
                        stackOffset+=4;
                    }

                    if (intArgRegAvailable())
                    {
                        // load double arg into 2 int arg regs
                        *b++ = opCode_LoadWord (getAvailableIntArgReg(),
                                                SP, stackOffset);
                        useIntReg();
                        *b++ = opCode_LoadWord (getAvailableIntArgReg(),
                                                SP, stackOffset+4);
                        useIntReg();
                       
                    }
                }
                else
                if(doubleArgRegAvailable())
                {

                    // load double arg into double register

                    // ldc1  doubleArgReg,  StackOffset(SP)
                    *b++ = opCode_LoadDouble(getAvailableDoubleArgReg(),
                                             SP, stackOffset);
                    useDoubleReg();
                }
                stackOffset+=8;
                break;
            }
            argList--;
        }

        // end of thunk, jump to selected function
        *b++ = opCode_Jmp(T6);
        *b++ = opCode_Nop();  // Delay slot, is this necessary?
        *b++ = 0;
    }

    void reset(void){
        nextIntReg = A0;
        nextFloatReg = FA0;
        nextDoubleReg = FA0;
        LeadingIntegerArguments = FALSE;

        int i;
        for(i=0;i<NumIntRegs;i++)
            intArgRegs[i] = REG_UNUSED;

        for(i=0;i<NumFloatRegs;i++)
            floatArgRegs[i] = REG_UNUSED;
    }

    AfxRegisterState(void){
        reset();
    };

};

