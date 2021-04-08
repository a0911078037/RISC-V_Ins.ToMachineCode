# RISC-V_Ins.ToMachineCode
read risc-v instruction and compile to machine code support to I,R,S,SB type

#HOW TO USE 

Input risc-v inst and read until EOF

then output the machine code.

#PROCESS DESCRIPTION

When input all the ins.

It will call the function (find_ins)

In the function,it will search the inst ex.add,addi etc..

And call the class when found the type of Ins.

Inside the class,read the all rs1,rs2,rd etc,and find the offset(if is SB type)

translate to Binary when assble the code.

