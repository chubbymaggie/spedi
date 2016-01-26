//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2015-2016 University of Kaiserslautern.

#include "MaximalBlock.h"
#include <cassert>

namespace disasm {

size_t
MaximalBlock::getBasicBlockMemSize(const unsigned int bb_id) const {
    assert(bb_id <= m_bblocks.size()
               && "Invalid Basic Block Id!!");
    return m_bblocks[bb_id].size();
}

bool MaximalBlock::isValid() const {
    if (m_bblocks.size() == 0)
        return false;

    for (const BasicBlock &block: m_bblocks)
        if (!block.isValid())
            return false;
    return true;
}

const std::vector<const MCInstSmall *>
MaximalBlock::getInstructionsOf(BasicBlock &bblock) {
    std::vector<const MCInstSmall *> result;

    auto current = bblock.startAddr();
    for (auto iter = m_insts.cbegin(); iter < m_insts.cend(); ++iter) {
        if ((*iter).addr() == current) {
            result.push_back(&(*iter));
            current += (*iter).size();
        }
    }
    return result;
}

const std::vector<addr_t> &
MaximalBlock::getInstructionAddressesOf(const BasicBlock &bblock) const {
    return bblock.m_inst_addrs;
}

addr_t MaximalBlock::addrOfFirstInst() const {
    return m_insts.front().addr();
}

addr_t MaximalBlock::addrOfLastInst() const {
    return m_insts.back().addr();
}

const BasicBlock &
MaximalBlock::getBasicBlockById(const unsigned int bb_id) const {
    return m_bblocks[bb_id];
}

size_t
MaximalBlock::getBasicBlocksCount() const {
    return m_bblocks.size();
}

const std::vector<BasicBlock> &
MaximalBlock::getBasicBlocks() const {
    return m_bblocks;
}

MaximalBlock::MaximalBlock(unsigned id, const BranchData &branch) :
    m_id{id},
    m_branch{branch} {
}

unsigned MaximalBlock::getId() const {
    return m_id;
}

unsigned MaximalBlock::instructionsCount() const {
    return static_cast<unsigned> (m_insts.size());
}


bool MaximalBlock::isWithinAddressSpace(addr_t addr) const {
    return addrOfFirstInst() <= addr
        && addr < endAddr();
}

const std::vector<MCInstSmall> &MaximalBlock::getAllInstructions() const {
    return m_insts;
}
const BranchData &MaximalBlock::getBranch() const {
    return m_branch;
}

addr_t MaximalBlock::endAddr() const {
    return (m_end_addr);
}

bool MaximalBlock::startOverlapsWith(const MaximalBlock &prev_block) const {
    return addrOfFirstInst() < prev_block.endAddr();
}

bool MaximalBlock::startOverlapsWith(const MaximalBlock *prev_block) const {
    return addrOfFirstInst() < prev_block->endAddr();;
}

bool MaximalBlock::coversAddressSpaceOf(const MaximalBlock &block) const {
    return addrOfFirstInst() < block.addrOfFirstInst()
        && endAddr() > block.endAddr();
}

bool MaximalBlock::coversAddressSpaceOf(const MaximalBlock *block) const {
    return addrOfFirstInst() < block->addrOfFirstInst()
        && endAddr() > block->endAddr();
}

bool MaximalBlock::isAddressOfInstruction(const addr_t inst_addr) const {
    if (inst_addr < addrOfFirstInst() || inst_addr > addrOfLastInst()) {
        return false;
    }
    for (auto it = m_insts.cbegin(); it < m_insts.cend(); ++it) {
        if ((*it).addr() == inst_addr) {
            return true;
        }
    }
    return false;
}
}
