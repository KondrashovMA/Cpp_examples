//
// Created by mk on 05.04.2022.
//

#include "State.h"

int State::get_Mark() const {
    return this->_mark;
}

std::unordered_set<int>& State::get_previous_states(){
    return this->_previous_states;
}

std::unordered_set<int>& State::get_next_states(){
    return this->_next_states;
}