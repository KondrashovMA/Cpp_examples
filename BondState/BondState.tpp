//
// Created by mk on 05.04.2022.
//

inline bool BondState::_checkStates(){
    for(auto& state: this->_states){
        // рассматриваем номер данного состояние
        auto current_state_mark = state.second->get_Mark();
        std::unordered_set<int> previous_numbers = state.second->get_previous_states(); // список всех возможных предыдущих состояний для данного
        std::unordered_set<int> next_numbers = state.second->get_next_states(); // список всех возможных будущих состояний для данного

        for (auto& previous_number : previous_numbers){
            try{
                const auto& item = this->_states.at(previous_number); // получаем предыдущее состояние для выбранного состояния
                if(!item->get_next_states().contains(current_state_mark)){ // если текущее состояние есть в списке будущих состояний для прошлого состояния - то всё нормально, иначе ошибка
                    POSITION_EXCEPTION(
                            BondState,
                            checkFrames,
                            "Error in filling in next states",
                            LOGGER::TypeMessage::WARNING
                    )
                    return false;
                }
            }catch(std::exception& e){
                POSITION_EXCEPTION(
                        BondState,
                        checkFrames,
                        "Error in filling in next states",
                        LOGGER::TypeMessage::WARNING
                )
                return false;
            }
        }

        for (auto& next_number : next_numbers){
            try{
                const auto& item = this->_states.at(next_number);  // получаем будущее состояние для выбранного состояния
                if(!item->get_previous_states().contains(current_state_mark)){ // если текущее состояние есть в списке прошлых состояний для будущих состояний - то всё нормально, иначе ошибка
                    POSITION_EXCEPTION(
                            BondState,
                            checkFrames,
                            "Error in filling in previous states",
                            LOGGER::TypeMessage::WARNING
                    )
                    return false;
                }
            }catch(std::exception& e){
                POSITION_EXCEPTION(
                        BondState,
                        checkFrames,
                        "Error in filling in previous states",
                        LOGGER::TypeMessage::WARNING
                )
                return false;
            }
        }
    }
    return true;
}