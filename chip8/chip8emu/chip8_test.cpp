#pragma once
#include <iostream>
#include "chip8_unit.cpp"

// void test_
void test_fetching()
{
	CPU_8 cpu_unit;
    cpu_unit.makeClear();
    
	cpu_unit.memory[0x200]=0x13;
	cpu_unit.memory[0x201]=0x14;
    
    uint16_t expected = 0x1314;

	cpu_unit.fetch();
    
    std::cout<< "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected << (cpu_unit.fetch_opcode == expected ? " OK " : " X ") << std::endl;
    
}

void test_executing()
{
	CPU_8 cpu_unit;
    cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x01;
	cpu_unit.memory[0x201] = 0x11;

    uint16_t expected = 0x111;

	cpu_unit.fetch();

	cpu_unit.execute();

	std::cout << "opcode: " << cpu_unit.program_count << " Expected: " << expected << (cpu_unit.program_count == expected ? " OK " : " X " ) << std::endl;

}

void test_jump()
{
    CPU_8 cpu_unit;
    cpu_unit.makeClear();
    
    cpu_unit.memory[0x200] = 0x12;
	cpu_unit.memory[0x201] = 0x22;

    uint16_t expected = 0x222;

	cpu_unit.fetch();

	cpu_unit.execute();

	std::cout << "opcode: " << cpu_unit.program_count << " Expected: " << expected << (cpu_unit.program_count == expected ? " OK " : " X " ) << std::endl;

}

void test_stack_pointer()
{
    CPU_8 cpu_unit;
    cpu_unit.makeClear();

    cpu_unit.memory[0x200] = 0x21;
	cpu_unit.memory[0x201] = 0x11;

    uint16_t expected_OPCODE = 0x2111;
    uint16_t expected_PC = 0x111;
    uint16_t expected_SP = 0xEFC;
    uint16_t expected_SP_memory = 0x0202;

	cpu_unit.fetch();

	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC << (cpu_unit.program_count == expected_PC ? " OK " : " X " ) << std::endl;
	std::cout << "SP: " << cpu_unit.stack_pointer << " Expected: " << expected_SP << (cpu_unit.stack_pointer == expected_SP ? " OK " : " X " ) << std::endl;
	std::cout << "SP_mem: " << ((cpu_unit.memory[0xEFE]<<8) | cpu_unit.memory[0xEFF]) << " Expected: " << expected_SP_memory << (((cpu_unit.memory[0xEFE] << 8 )|cpu_unit.memory[0xEFF]) == expected_SP_memory ? " OK " : " X " ) << std::endl;

}

void test_return_stack_pointer()
{
    CPU_8 cpu_unit;
    cpu_unit.makeClear();
    
    cpu_unit.memory[0x200] = 0x22;
	cpu_unit.memory[0x201] = 0x22;

    cpu_unit.memory[0x222] = 0x00;
	cpu_unit.memory[0x223] = 0xEE;
    // b - before || a - after
    uint16_t expected_OPCODE_b = 0x2222;
    uint16_t expected_PC_b = 0x222;
    uint16_t expected_SP_b = 0xEFC;
    uint16_t expected_SP_memory_b = 0x0202;

    uint16_t expected_OPCODE_a = 0x00EE;
    uint16_t expected_PC_a = 0x202;
    uint16_t expected_SP_a = 0xEFE;
    
	cpu_unit.fetch();

	cpu_unit.execute();
    
    std::cout << "STAGE I" << std::endl;
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE_b << (cpu_unit.fetch_opcode == expected_OPCODE_b ? " OK " : " X " ) << std::endl;
	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_b << (cpu_unit.program_count == expected_PC_b ? " OK " : " X " ) << std::endl;
	std::cout << "SP: " << cpu_unit.stack_pointer << " Expected: " << expected_SP_b << (cpu_unit.stack_pointer == expected_SP_b ? " OK " : " X " ) << std::endl;
	std::cout << "SP_mem: " << ((cpu_unit.memory[0xEFE]<<8) | cpu_unit.memory[0xEFF]) << " Expected: " << expected_SP_memory_b << (((cpu_unit.memory[0xEFE] << 8 )|cpu_unit.memory[0xEFF]) == expected_SP_memory_b ? " OK " : " X " ) << std::endl << std::endl;
    
	cpu_unit.fetch();
	cpu_unit.execute();
    std::cout << "STAGE II" << std::endl;
    std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE_a << (cpu_unit.fetch_opcode == expected_OPCODE_a ? " OK " : " X " ) << std::endl;
	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_a << (cpu_unit.program_count == expected_PC_a ? " OK " : " X " ) << std::endl;
	std::cout << "SP: " << cpu_unit.stack_pointer << " Expected: " << expected_SP_a << (cpu_unit.stack_pointer == expected_SP_a ? " OK " : " X " ) << std::endl;
}

void test_return_stack_pointer_incorrect()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();
    
	cpu_unit.memory[0x200] = 0x00;
	cpu_unit.memory[0x201] = 0xEE;

	cpu_unit.fetch();

	cpu_unit.execute();
    
}

void test_cond_eq()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x31;
	cpu_unit.memory[0x201] = 0x11;

	uint16_t expected_OPCODE = 0x3111;
	uint16_t expected_PC_fail = 0x202;
	uint16_t expected_PC_pass = 0x204;

	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC_fail: " << cpu_unit.program_count << " Expected: " << expected_PC_fail << (cpu_unit.program_count == expected_PC_fail ? " OK " : " X " ) << std::endl;
	
	cpu_unit.program_count = 0x200;
	cpu_unit.reg_V[0x01] = 0x11;
	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "PC_pass: " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;

};

void test_cond_not_eq()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x41;
	cpu_unit.memory[0x201] = 0x11;

	uint16_t expected_OPCODE = 0x4111;
	uint16_t expected_PC_fail = 0x202;
	uint16_t expected_PC_pass = 0x204;

	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC_pass: " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
	cpu_unit.program_count = 0x200;
	cpu_unit.reg_V[0x01] = 0x11;
	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "PC_fail: " << cpu_unit.program_count << " Expected: " << expected_PC_fail << (cpu_unit.program_count == expected_PC_fail ? " OK " : " X " ) << std::endl;

};

void test_cond_xy_not_eq()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x91;
	cpu_unit.memory[0x201] = 0x20;

	uint16_t expected_OPCODE = 0x9120;
	uint16_t expected_PC_fail = 0x202;
	uint16_t expected_PC_pass = 0x204;

	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC_fail: " << cpu_unit.program_count << " Expected: " << expected_PC_fail << (cpu_unit.program_count == expected_PC_fail ? " OK " : " X " ) << std::endl;
	
	cpu_unit.program_count = 0x200;
	cpu_unit.reg_V[0x01] = 0x11;
	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "PC_pass: " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;

};

void test_cond_xy_eq()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x51;
	cpu_unit.memory[0x201] = 0x20;

	uint16_t expected_OPCODE = 0x5120;
	uint16_t expected_PC_fail = 0x202;
	uint16_t expected_PC_pass = 0x204;

	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC_pass: " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
	cpu_unit.program_count = 0x200;
	cpu_unit.reg_V[0x01] = 0x11;
	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "PC_fail: " << cpu_unit.program_count << " Expected: " << expected_PC_fail << (cpu_unit.program_count == expected_PC_fail ? " OK " : " X " ) << std::endl;

};

void test_set_x()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x61;
	cpu_unit.memory[0x201] = 0x20;

	uint16_t expected_OPCODE = 0x6120;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_VX_pass = 0x20;

	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X: " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;
	
};

void test_add_x()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x72;
	cpu_unit.memory[0x201] = 0x20;

	uint16_t expected_OPCODE = 0x7220;
	uint16_t expected_V_X = 0x02;
	uint16_t expected_VX_pass = 0x20;

	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X: " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;
	
};

void test_set_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x20;

	uint16_t expected_OPCODE = 0x8120;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0x20;

	cpu_unit.reg_V[expected_V_Y] = expected_VX_pass;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;
	
};

void test_or_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x21;

	uint16_t expected_OPCODE = 0x8121;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0xA5;

	cpu_unit.reg_V[expected_V_Y] = expected_VX_pass;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;
	
};

void test_and_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x22;

	uint16_t expected_OPCODE = 0x8122;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0x24;

	cpu_unit.reg_V[expected_V_X] = 0x7E;
	cpu_unit.reg_V[expected_V_Y] = 0xA5;
	// 10100101
	// 01111110
	// 00100100
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;
	
};

void test_xor_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x23;

	uint16_t expected_OPCODE = 0x8123;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0xC3; 

	cpu_unit.reg_V[expected_V_X] = 0xA5;
	cpu_unit.reg_V[expected_V_Y] = 0x66;
	// 10100101
	// 01100110
	// 11000011
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;
	
};

void test_add_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x24;

	cpu_unit.memory[0x202] = 0x81;
	cpu_unit.memory[0x203] = 0x24;

	// add without carry
	uint16_t expected_OPCODE = 0x8124;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0xF5; 

	cpu_unit.reg_V[expected_V_X] = 0x05;
	cpu_unit.reg_V[expected_V_Y] = 0xF0;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 0 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 0) ? " OK " : " X " ) << std::endl;

	cpu_unit.fetch();
	cpu_unit.execute();
	
	expected_VX_pass = 0xE5; 

	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 1 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 1) ? " OK " : " X " ) << std::endl;
	
};

void test_sub_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x25;

	cpu_unit.memory[0x202] = 0x81;
	cpu_unit.memory[0x203] = 0x25;

	// add without carry
	uint16_t expected_OPCODE = 0x8125;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0x15; 

	cpu_unit.reg_V[expected_V_X] = 0x05;
	cpu_unit.reg_V[expected_V_Y] = 0xF0;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 0 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 0) ? " OK " : " X " ) << std::endl;

	cpu_unit.reg_V[expected_V_Y] = 0x05;
	cpu_unit.fetch();
	cpu_unit.execute();
	
	expected_VX_pass = 0x10; 

	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 1) ? " OK " : " X " ) << std::endl;
	
};

void test_shr_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x26;

	cpu_unit.memory[0x202] = 0x81;
	cpu_unit.memory[0x203] = 0x26;


	// add without carry
	uint16_t expected_OPCODE = 0x8126;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_VX_pass = 0x7F; 

	cpu_unit.reg_V[expected_V_X] = 0xFE;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 0 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 0) ? " OK " : " X " ) << std::endl;

	cpu_unit.reg_V[expected_V_X] = 0xFF;
	cpu_unit.fetch();
	cpu_unit.execute();

	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 1 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 1) ? " OK " : " X " ) << std::endl;
	
};

void test_nsub_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x27;

	cpu_unit.memory[0x202] = 0x81;
	cpu_unit.memory[0x203] = 0x27;


	// add without carry
	uint16_t expected_OPCODE = 0x8127;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_V_Y = 0x02;
	uint16_t expected_VX_pass = 0x10; 

	cpu_unit.reg_V[expected_V_X] = 0xFE;
	cpu_unit.reg_V[expected_V_Y] = 0x0E;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 0 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 0) ? " OK " : " X " ) << std::endl;

	cpu_unit.reg_V[expected_V_X] = 0xDF;
	cpu_unit.reg_V[expected_V_Y] = 0xEF;
	cpu_unit.fetch();
	cpu_unit.execute();

	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 1 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 1) ? " OK " : " X " ) << std::endl;
	
};

void test_shl_xy()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x81;
	cpu_unit.memory[0x201] = 0x2E;

	cpu_unit.memory[0x202] = 0x81;
	cpu_unit.memory[0x203] = 0x2E;


	// add without carry
	uint16_t expected_OPCODE = 0x812E;
	uint16_t expected_V_X = 0x01;
	uint16_t expected_VX_pass = 0xFE; 

	cpu_unit.reg_V[expected_V_X] = 0x7F;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 0 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 0) ? " OK " : " X " ) << std::endl;

	cpu_unit.reg_V[expected_V_X] = 0xFF;
	cpu_unit.fetch();
	cpu_unit.execute();

	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "V_X + carry flag 1 : " << uint16_t(cpu_unit.reg_V[expected_V_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_V_X] == expected_VX_pass && (cpu_unit.reg_V[0xF] == 1) ? " OK " : " X " ) << std::endl;
	
};

void test_set_I()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xA1;
	cpu_unit.memory[0x201] = 0x2E;


	// add without carry
	uint16_t expected_OPCODE = 0xA12E;
	uint16_t expected_I_pass = 0x12E; 
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "I : " << uint16_t(cpu_unit.reg_I) << " Expected: " << expected_I_pass << (cpu_unit.reg_I == expected_I_pass ? " OK " : " X " ) << std::endl;
	
};

void test_jump_to_NNNplusV()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xB1;
	cpu_unit.memory[0x201] = 0x2E;


	// add without carry
	cpu_unit.reg_V[0] = 1;
	uint16_t expected_OPCODE = 0xB12E;
	uint16_t expected_PC_pass = 0x12F; 
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC : " << uint16_t(cpu_unit.program_count) << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
};

void test_rand()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xC1;
	cpu_unit.memory[0x201] = 0x2E;


	uint16_t expected_OPCODE = 0xC12E;
	uint16_t expected_X = 0x01;
	uint16_t expected_X_fail = 0x2E;
	
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "VX : " << uint16_t(cpu_unit.reg_V[expected_X]) << " NOT Expected: " << expected_X_fail << (cpu_unit.reg_V[expected_X] != expected_X_fail ? " OK " : " X " ) << std::endl;
	
};

void test_draw()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	// 0x6101
	cpu_unit.memory[0x200] = 0x61;
	cpu_unit.memory[0x201] = 0x01;
	// 0x6202
	cpu_unit.memory[0x202] = 0x62;
	cpu_unit.memory[0x203] = 0x02;
	// 0xA101
	cpu_unit.memory[0x204] = 0xA1;
	cpu_unit.memory[0x205] = 0x01;
	// NNN 
	cpu_unit.memory[0x101] = 0x55;
	cpu_unit.memory[0x102] = 0xAA;
	cpu_unit.memory[0x103] = 0x61;
	// 0xD122
	cpu_unit.memory[0x206] = 0xD1;
	cpu_unit.memory[0x207] = 0x22;
	// checks here - no bit flip
	// 0xA103
	cpu_unit.memory[0x208] = 0xA1;
	cpu_unit.memory[0x209] = 0x03;
	// 0xD121
	cpu_unit.memory[0x20A] = 0xD1;
	cpu_unit.memory[0x20B] = 0x21;
	// checks here - bit flip
	// order - a,b,c...
	uint16_t expected_OPCODE_a = 0xD122;
	uint16_t expected_OPCODE_b = 0xD121;
	uint16_t expected_XY_addr_a = 0xF00 + (0x01 + (0x02 * 8));
	uint16_t expected_XY_addr_b = 0xF00 + (0x01 + (0x03 * 8));
	uint16_t expected_a = 0x55;
	uint16_t expected_b = 0xAA;
	uint16_t expected_c = 0x34;

	cpu_unit.fetch();
	cpu_unit.execute();
    	cpu_unit.fetch();
	cpu_unit.execute();
 	cpu_unit.fetch();
	cpu_unit.execute();
 	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE_a << (cpu_unit.fetch_opcode == expected_OPCODE_a ? " OK " : " X " ) << std::endl;
	std::cout << "MEMORY[0xF11:0xF19] : " << uint16_t(cpu_unit.memory[expected_XY_addr_a]) << " " << uint16_t(cpu_unit.memory[expected_XY_addr_b]) << " Expected: " << expected_a << (cpu_unit.memory[expected_XY_addr_a] == expected_a ? " [OK] " : " [X] " ) << expected_b << (cpu_unit.memory[expected_XY_addr_b] == expected_b ? " [OK] " : " [X] " ) << " BIT FLIP: NO" << (cpu_unit.reg_V[0xF] == 0 ? " [OK] " : " [X] " ) << std::endl;

	cpu_unit.fetch();
	cpu_unit.execute();
 	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE_b << (cpu_unit.fetch_opcode == expected_OPCODE_b ? " OK " : " X " ) << std::endl;
	std::cout << "MEMORY[0xF11] : " << uint16_t(cpu_unit.memory[expected_XY_addr_a]) << " Expected: " << expected_c << (cpu_unit.memory[expected_XY_addr_a] == expected_c ? " [OK] " : " [X] " ) << " BIT FLIP: YES" << (cpu_unit.reg_V[0xF] == 1 ? " [OK] " : " [X] " ) << std::endl;

};

void test_key_press()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x61;
	cpu_unit.memory[0x201] = 0x01;
	cpu_unit.memory[0x202] = 0xE1;
	cpu_unit.memory[0x203] = 0x9E;
	cpu_unit.memory[0x204] = 0xE1;
	cpu_unit.memory[0x205] = 0x9E;

	uint16_t expected_OPCODE = 0xE19E;
	uint16_t expected_X = 0x01;
	uint16_t expected_PC_pass = 0x206;
	
	cpu_unit.keys[expected_X] = true;

	cpu_unit.fetch();
	cpu_unit.execute();
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "KEY PRESS: " << cpu_unit.keys[expected_X] << " PC : " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
	cpu_unit.program_count = 0x204;
	expected_PC_pass = 0x206;
	cpu_unit.keys[expected_X] = false;

	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "KEY PRESS: " << cpu_unit.keys[expected_X] << " PC : " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
};

void test_key_not_press()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0x61;
	cpu_unit.memory[0x201] = 0x01;
	cpu_unit.memory[0x202] = 0xE1;
	cpu_unit.memory[0x203] = 0xA1;
	cpu_unit.memory[0x204] = 0xE1;
	cpu_unit.memory[0x205] = 0xA1;

	uint16_t expected_OPCODE = 0xE1A1;
	uint16_t expected_X = 0x01;
	uint16_t expected_PC_pass = 0x206;
	
	cpu_unit.keys[expected_X] = false;

	cpu_unit.fetch();
	cpu_unit.execute();
	cpu_unit.fetch();
	cpu_unit.execute();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "KEY PRESS: " << cpu_unit.keys[expected_X] << " PC : " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
	cpu_unit.program_count = 0x204;
	expected_PC_pass = 0x206;
	cpu_unit.keys[expected_X] = true;

	cpu_unit.fetch();
	cpu_unit.execute();
 
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "KEY PRESS: " << cpu_unit.keys[expected_X] << " PC : " << cpu_unit.program_count << " Expected: " << expected_PC_pass << (cpu_unit.program_count == expected_PC_pass ? " OK " : " X " ) << std::endl;
	
};

void test_delay_timer()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x07;
	cpu_unit.memory[0x202] = 0xF1;
	cpu_unit.memory[0x203] = 0x07;

	uint16_t expected_OPCODE = 0xF107;
	uint16_t expected_X = 0x01;
	uint16_t expected_VX_pass = 0x2F;
	
	cpu_unit.timer_delay = expected_VX_pass;

	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "VX: " << uint16_t(cpu_unit.reg_V[expected_X]) << " Expected: " << expected_VX_pass << (cpu_unit.reg_V[expected_X] == expected_VX_pass ? " OK " : " X " ) << std::endl;

	cpu_unit.run();
	
	std::cout << "VX: " << uint16_t(cpu_unit.reg_V[expected_X]) << " Expected: " << expected_VX_pass-1 << (cpu_unit.reg_V[expected_X] == expected_VX_pass - 1 ? " OK " : " X " ) << std::endl;
};

void test_get_key()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x0A;
	cpu_unit.memory[0x202] = 0xF1;
	cpu_unit.memory[0x203] = 0x0A;
	
	uint16_t expected_OPCODE = 0xF10A;
	uint16_t expected_X = 0x01;
	uint16_t expected_key = 0x05;
	uint16_t expected_PC_pass_a = 0x200;
	uint16_t expected_PC_pass_b = 0x202;
	
	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_pass_a << (cpu_unit.program_count == expected_PC_pass_a ? " OK[WAIT FOR KEY] " : " X " ) << std::endl;

	cpu_unit.run();

	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_pass_a << (cpu_unit.program_count == expected_PC_pass_a ? " OK[WAIT FOR KEY] " : " X " ) << std::endl;
	
	cpu_unit.run();

	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_pass_a << (cpu_unit.program_count == expected_PC_pass_a ? " OK[WAIT FOR KEY] " : " X " ) << std::endl;
	
	cpu_unit.run();

	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_pass_a << (cpu_unit.program_count == expected_PC_pass_a ? " OK[WAIT FOR KEY] " : " X " ) << std::endl;
	
	cpu_unit.keys[expected_key] = true;
	cpu_unit.run();

	std::cout << "PC: " << cpu_unit.program_count << " Expected: " << expected_PC_pass_b << " VX: " << uint16_t(cpu_unit.reg_V[expected_X]) << (cpu_unit.program_count == expected_PC_pass_b ? " OK[KEY GET] " : " X " ) << std::endl;

};

void test_set_delay_timer()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x15;
	cpu_unit.memory[0x202] = 0x00;
	cpu_unit.memory[0x203] = 0x00;
	
	uint16_t expected_OPCODE = 0xF115;
	uint16_t expected_X = 0x01;
	uint16_t expected_time = 0x05;
	uint16_t expected_timer_pass_a = 0x04;
	uint16_t expected_timer_pass_b = 0x03;
	cpu_unit.reg_V[expected_X] = expected_time;
	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "DT: " << uint16_t(cpu_unit.timer_delay) << " Expected: " << expected_timer_pass_a << (cpu_unit.timer_delay == expected_timer_pass_a ? " OK " : " X " ) << std::endl;

	cpu_unit.run();

	std::cout << "DT: " << uint16_t(cpu_unit.timer_delay) << " Expected: " << expected_timer_pass_b << (cpu_unit.timer_delay == expected_timer_pass_b ? " OK " : " X " ) << std::endl;

};

void test_set_sound_timer()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x18;
	cpu_unit.memory[0x202] = 0x00;
	cpu_unit.memory[0x203] = 0x00;
	
	uint16_t expected_OPCODE = 0xF118;
	uint16_t expected_X = 0x01;
	uint16_t expected_time = 0x05;
	uint16_t expected_timer_pass_a = 0x04;
	uint16_t expected_timer_pass_b = 0x03;
	cpu_unit.reg_V[expected_X] = expected_time;
	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "ST: " << uint16_t(cpu_unit.timer_sound) << " Expected: " << expected_timer_pass_a << (cpu_unit.timer_sound == expected_timer_pass_a ? " OK " : " X " ) << std::endl;

	cpu_unit.run();

	std::cout << "ST: " << uint16_t(cpu_unit.timer_sound) << " Expected: " << expected_timer_pass_b << (cpu_unit.timer_sound == expected_timer_pass_b ? " OK " : " X " ) << std::endl;

};

void test_add_VXtoI()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x1E;
	
	uint16_t expected_OPCODE = 0xF11E;
	uint16_t expected_X = 0x01;
	uint16_t expected_I = 0x05;
	cpu_unit.reg_V[expected_X] = expected_I;
	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "I: " << cpu_unit.reg_I << " Expected: " << expected_I << (cpu_unit.reg_I == expected_I ? " OK " : " X " ) << std::endl;

};

void test_set_ItoFontSprite()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x29;
	cpu_unit.memory[0x202] = 0xF1;
	cpu_unit.memory[0x203] = 0x29;
	
	uint16_t expected_OPCODE = 0xF129;
	uint16_t expected_X = 0x01;
	uint16_t expected_font = 0x05;
	cpu_unit.reg_V[expected_X] = expected_font;
	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "I: " << cpu_unit.reg_I << " Expected: " << expected_font*5 << (cpu_unit.reg_I == expected_font*5 ? " OK " : " X " ) << std::endl;

	expected_font = 0x0A;
	cpu_unit.reg_V[expected_X] = expected_font;
	cpu_unit.run();
	
	std::cout << "I: " << cpu_unit.reg_I << " Expected: " << expected_font*5 << (cpu_unit.reg_I == expected_font*5 ? " OK " : " X " ) << std::endl;

};

void test_set_BCDofVX()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF1;
	cpu_unit.memory[0x201] = 0x33;
	cpu_unit.memory[0x202] = 0xF1;
	cpu_unit.memory[0x203] = 0x33;
	
	uint16_t expected_OPCODE = 0xF133;
	uint16_t expected_X = 0x01;
	uint16_t expected_nr = 0x6F;
	uint16_t expected_Hnr = 0x01;
	uint16_t expected_Tnr = 0x01;
	uint16_t expected_Onr = 0x01;
	cpu_unit.reg_V[expected_X] = expected_nr;
	cpu_unit.reg_I = 0x50;
	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "MEM[I/I+1/I+2]: " << uint16_t(cpu_unit.memory[cpu_unit.reg_I]) << uint16_t(cpu_unit.memory[cpu_unit.reg_I+1]) << uint16_t(cpu_unit.memory[cpu_unit.reg_I+2]) << " Expected: " << expected_nr << ((cpu_unit.memory[cpu_unit.reg_I] == expected_Hnr) && (cpu_unit.memory[cpu_unit.reg_I+1] == expected_Tnr) && (cpu_unit.memory[cpu_unit.reg_I+2] == expected_Onr) ? " OK " : " X " ) << std::endl;

	expected_nr = 0xFD;
	expected_Hnr = 0x02;
	expected_Tnr = 0x05;
	expected_Onr = 0x03;
	cpu_unit.reg_V[expected_X] = expected_nr;
	cpu_unit.run();
	
	std::cout << "MEM[I/I+1/I+2]: " << uint16_t(cpu_unit.memory[cpu_unit.reg_I]) << uint16_t(cpu_unit.memory[cpu_unit.reg_I+1]) << uint16_t(cpu_unit.memory[cpu_unit.reg_I+2]) << " Expected: " << expected_nr << ((cpu_unit.memory[cpu_unit.reg_I] == expected_Hnr) && (cpu_unit.memory[cpu_unit.reg_I+1] == expected_Tnr) && (cpu_unit.memory[cpu_unit.reg_I+2] == expected_Onr) ? " OK " : " X " ) << std::endl;

};

void test_reg_dump()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF5;
	cpu_unit.memory[0x201] = 0x55;
	
	uint16_t expected_OPCODE = 0xF555;
	uint16_t expected_X = 0x05;
	uint16_t expected_answ = 0x06;

	uint16_t check = 0;
	for (uint16_t idx = 0; idx < expected_X + 1; idx++)
	{
		cpu_unit.reg_V[idx] = idx;
	};

	cpu_unit.reg_I = 0x100;

	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "REG DUMP FROM V[0] TO: V[" << expected_X << "](included) Expected answ: " << expected_answ;

	check = 0;

	for (uint16_t idx = 0; idx < expected_X + 1; idx++)
	{
		check += cpu_unit.memory[cpu_unit.reg_I + idx] == idx ? 1 : 0;
	};

	std::cout << (check == expected_answ ? " OK " : " X " ) << std::endl;

};

void test_reg_load()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();

	cpu_unit.memory[0x200] = 0xF5;
	cpu_unit.memory[0x201] = 0x65;
	
	uint16_t expected_OPCODE = 0xF565;
	uint16_t expected_X = 0x05;
	uint16_t expected_answ = 0x06;

	cpu_unit.reg_I = 0x100;

	uint16_t check = 0;

	for (uint16_t idx = 0; idx < expected_X + 1; idx++)
	{
		cpu_unit.memory[cpu_unit.reg_I + idx] = idx + 2;
	};

	cpu_unit.run();
    
	std::cout << "opcode: " << cpu_unit.fetch_opcode << " Expected: " << expected_OPCODE << (cpu_unit.fetch_opcode == expected_OPCODE ? " OK " : " X " ) << std::endl;
	std::cout << "REG LOAD FROM V[0] TO: V[" << expected_X << "](included) Expected answ: " << expected_answ;

	check = 0;

	for (uint16_t idx = 0; idx < expected_X + 1; idx++)
	{
		check += cpu_unit.reg_V[idx] == idx + 2 ? 1 : 0;
	};

	std::cout << (check == expected_answ ? " OK " : " X " ) << std::endl;

};

void test_fonts()
{
	CPU_8 cpu_unit;
	cpu_unit.makeClear();
	uint16_t check_wrong=0;

	uint8_t test_fonts[0x50] = {
	       	0xF0, 0x90, 0x90, 0x90, 0xF0, // font 0
		0x20, 0x60, 0x20, 0x20, 0x70, // font 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // font 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // font 3
	       	0x90, 0x90, 0xF0, 0x10, 0x10, // font 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // font 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // font 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // font 7
	       	0xF0, 0x90, 0xF0, 0x90, 0xF0, // font 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // font 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // font A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // font B
	       	0xF0, 0x80, 0x80, 0x80, 0xF0, // font C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // font D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // font E
		0xF0, 0x80, 0xF0, 0x80, 0x80, // font F
	};
	for (uint16_t idx = 0; idx < 0x50; idx++)
	{
		check_wrong += cpu_unit.memory[idx] == test_fonts[idx] ? 0 : 1;
	};

	std::cout << "FONTS TEST: " << (check_wrong == 0 ? " OK " : " X " ) << std::endl;

};





int main()
{
    std::cout<< "Tests chip8_unit!!" << std::endl;
    std::cout<< "-----------------------" << std::endl;

    std::cout<< "----FETCH--------------" << std::endl;
	test_fetching();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----EXECUTE------------" << std::endl;
	test_executing();
    std::cout<< "-----------------------" << std::endl << std::endl;
    
    std::cout<< "----JUMP---------------" << std::endl;
	test_jump();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----STACK_POINTER------" << std::endl;
    test_stack_pointer();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----RETURN SP----------" << std::endl;
    test_return_stack_pointer();
    std::cout<< "-----------------------" << std::endl << std::endl;

    // incorrect return test
    // test_return_stack_pointer_incorrect(); // test check - OK;

    std::cout<< "----COND EQ------------" << std::endl;
    test_cond_eq();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----COND NOT EQ--------" << std::endl;
    test_cond_not_eq();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----COND XY EQ---------" << std::endl;
    test_cond_xy_eq();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----COND XY NOT EQ-----" << std::endl;
    test_cond_xy_not_eq();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET V_X TO NN------" << std::endl;
	test_set_x();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----ADD NN TO V_X------" << std::endl;
	test_add_x();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET V_X TO V_Y-----" << std::endl;
	test_set_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----OR V_X & V_Y-----" << std::endl;
	test_or_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----AND V_X & V_Y-----" << std::endl;
	test_and_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----XOR V_X & V_Y-----" << std::endl;
	test_xor_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----ADD V_X & V_Y-----" << std::endl;
	test_add_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SUB V_X & V_Y-----" << std::endl;
	test_sub_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SHR V_X & V_Y-----" << std::endl;
	test_shr_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----NSUB V_X & V_Y-----" << std::endl;
	test_nsub_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SHL V_X & V_Y-----" << std::endl;
	test_shl_xy();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET I--------------" << std::endl;
	test_set_I();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----JUMP TO NNN+V0-----" << std::endl;
	test_jump_to_NNNplusV();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----RANDOM-------------" << std::endl;
	test_rand();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----DRAW---------------" << std::endl;
	test_draw();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----KEY PRESS----------" << std::endl;
	test_key_press();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----KEY NOT PRESS------" << std::endl;
	test_key_not_press();
    std::cout<< "-----------------------" << std::endl << std::endl;

// opcode of 0xF000 need to be tested

    std::cout<< "----DELAY TIMER--------" << std::endl;
	test_delay_timer();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----GET KEY------------" << std::endl;
	test_get_key();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET DELAY TIMER----" << std::endl;
	test_set_delay_timer();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET SOUND TIMER----" << std::endl;
	test_set_sound_timer();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----ADD VX TO I--------" << std::endl;
	test_add_VXtoI();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET ItoFontSprite--" << std::endl;
	test_set_ItoFontSprite();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----SET BCD OF VX------" << std::endl;
	test_set_BCDofVX();
    std::cout<< "-----------------------" << std::endl << std::endl;
// test for 0xFX55 and 0xFX65
    std::cout<< "----REG DUMP-----------" << std::endl;
	test_reg_dump();
    std::cout<< "-----------------------" << std::endl << std::endl;

    std::cout<< "----REG LOAD-----------" << std::endl;
	test_reg_load();
    std::cout<< "-----------------------" << std::endl << std::endl;


// test to check fonts TODO
    std::cout<< "----FONTS TEST----------" << std::endl;
	test_fonts();
    std::cout<< "-----------------------" << std::endl << std::endl;




    return 0;
}
