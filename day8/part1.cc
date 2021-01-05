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
    MachineState machine_state = run_program(program);
    std::cout << machine_state << std::endl;

    return 0;
}