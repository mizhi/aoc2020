#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

struct MachineState {
    MachineState(int ip = 0, int accum = 0): ip(ip), accum(accum) { }

    int ip = 0;
    int accum = 0;

    friend std::ostream& operator<<(std::ostream& os, const MachineState& ms);
};

std::ostream& operator<<(std::ostream& os, const MachineState& ms) {
    os << "ip: " << ms.ip << ", accum: " << ms.accum;
    return os;
}

struct Instruction {
    std::string command;
    int parameter;

    MachineState apply(const MachineState& ms) const {
        if (this->command == "jmp") {
            return MachineState { ms.ip + this->parameter, ms.accum };
        } else if (this->command == "acc") {
            return MachineState { ms.ip + 1, ms.accum + this->parameter };
        }

        return MachineState { ms.ip + 1, ms.accum };
    }

    friend std::ostream& operator<<(std::ostream& os, const Instruction& i);
};

std::ostream& operator<<(std::ostream& os, const Instruction& i) {
    os << i.command << " " << i.parameter;
    return os;
}

std::vector<Instruction> read_program(const std::string input_filename) {
    std::vector<Instruction> program;

    std::ifstream input_file(input_filename, std::ifstream::in);

    while (input_file.good()) {
        std::string command;
        std::string parameter_string;
        input_file >> command >> parameter_string;

        if (command == "" || parameter_string == "") continue;

        int parameter = std::stoi(parameter_string);
        program.push_back(Instruction {command, parameter});
    }

    return program;
}

MachineState run_program(const std::vector<Instruction> program) {
    MachineState current_state{0, 0};
    std::vector<bool> seen_instructions(program.size(), false);

    while (current_state.ip >= 0 && current_state.ip < program.size() && !seen_instructions[current_state.ip]) {
        MachineState new_state = program[current_state.ip].apply(current_state);
        seen_instructions[current_state.ip] = true;
        current_state = new_state;
    }

    return current_state;
}

int main(int argc, char** argv) {
    std::vector<Instruction> program = read_program("input.txt");

    // we're going to find the instruction to flip by going through each instruction
    // in the program. For jmp and nop instructions, we'll produce a copy of the program
    // that is the same except for that one instruction. We skip acc instructions.
    // Then we run the copy of that program. If we ever end with a machine state
    // where the instruction pointer (ip) is one past the end of the program, then
    // we know that we've found the fix.
    for (int i = 0; i < program.size(); i++) {
        Instruction current_instruction = program[i];

        if (current_instruction.command == "acc") continue;

        std::vector<Instruction> program_copy(program);

        if (current_instruction.command == "nop") {
            program_copy[i] = Instruction{"jmp", current_instruction.parameter };
        } else {
            program_copy[i] = Instruction{"nop", current_instruction.parameter };
        }

        MachineState final_state = run_program(program_copy);

        if (final_state.ip == program.size()) {
            // we could end the program here, but to be safe, we'll run until we have swapped all possible instructions
            // to see if there is more than one possible answer.
            std::cout << "Accumulator at end: " << final_state.accum << std::endl;
        }
    }

    return 0;
}