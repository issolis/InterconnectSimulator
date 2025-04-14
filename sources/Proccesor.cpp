#include "Proccesor.h"
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

enum class state {
    READ, 
    EXECUTE, 
    VALIDATE, 
    COMMIT, 
    RETRY
}; 

struct thread_context {
    state current_state {state::READ};
    int start_size {0};
    bool committed {false};
};
Proccesor::Proccesor(InstructionList &stack, std::vector<std::thread>& workers) {
    this->stack = &stack;
    this->workers = &workers; 
}

void Proccesor::processorThreadFunction(std::string instr) {
    thread_context ctx;

    while (!ctx.committed) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = stack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (stack->size.load() == ctx.start_size) {
                    stack->executeStackOperation(1, instr);
                    ctx.current_state = state::COMMIT;
                } else {
                    ctx.current_state = state::RETRY;
                }
                break;
            case state::COMMIT:
                ctx.committed = true;
                break;
            case state::RETRY:
                ctx.current_state = state::READ;
                break;
        }
    }
}

void Proccesor::processorThread(std::string instr) {
    workers->emplace_back(&Proccesor::processorThreadFunction, this, instr);
}
