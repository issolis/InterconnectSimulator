#include "Processor.h"


Processor::Processor(InstructionList &readStack, InstructionList &writeStack, std::vector<std::thread> &workers, std::string &fileName, int id) {
    processorRead = new ProcessorRead(readStack, workers, id);
    processorWrite = new ProcessorWrite(writeStack, workers, fileName);

}