//
// Created by mk on 05.04.2022.
//

#ifndef PROJECT0_RZA_BONDSTATE_H
#define PROJECT0_RZA_BONDSTATE_H

#include <unordered_set>
#include <functional>
#include <list>
#include <utility>
#include <iostream>
#include <exception>

#include <Exception.h>

#include "State.h"

//TODO перейти на хэши в качестве ключей

typedef int StateID;

class BondState {

private:
    /*!
     * Массив всех возможных состояний
     */
    std::unordered_map<StateID, std::shared_ptr<State>> _states;

    /*!
    * Проверка отсутствия неправильных переходов между состояниями
    */
    inline bool _checkStates();

    /*!
     * Текущее активное состояние
     */
    std::shared_ptr<State> _current_state;

public:

    /*!
     * Инициализация, включаящая в себя проверку на правильность связей
     * и установку начального состояния
     */
    void checkStates(){
        if (!this->_checkStates()){
            POSITION_EXCEPTION(
                    BondState,
                    changeState,
                    "Error in relations between states",
                    LOGGER::TypeMessage::ERROR
            )
        }
    }

    /*!
     * Добавить новое состояние в список возможных состояний
     */
    void add_States_Frame(const State& state){

        if (auto p = _states.try_emplace(state.get_Mark(), std::make_shared<State>(state)); !p.second){
            POSITION_EXCEPTION(
                    BondState,
                    changeState,
                    "The state has already been added",
                    LOGGER::TypeMessage::WARNING
            )
        }
    }

    /*!
     * Вызов текущей функции состояние
     */
    void currentFunction(){
        _current_state->state_function();
    }

    /*!
     * Функция смены состояния на некоторое следующее
     * с проверкой возможности такой смены
     */
    template <typename T>
    void changeState(const T& id) {
        auto& nextState = this->_current_state->get_next_states();
        boolean_T successFlag = false;
        // проверяем, что для текущего состояние следующее состояние содержится в списке следующих состояний
        // а для следующего состояние текущее состоние находится в списке предыдущих

        if (nextState.contains(static_cast<int>(id))){
            auto& previous_states = this->_states.at(static_cast<int>(id))->get_previous_states();
            if(previous_states.contains(this->_current_state->get_Mark())){
                successFlag = true;
            }
        }
        if(successFlag){
            successFlag = false;
            this->_current_state = this->_states.at(static_cast<int>(id));
        }else{
            std::string error_message{"Cannot switch from state number "};
            error_message = error_message +
                            std::to_string(this->_current_state->get_Mark()) + " to state number " +
                            std::to_string(static_cast<int>(id));
            POSITION_EXCEPTION(
                    BondState,
                    changeState,
                    error_message,
                    LOGGER::TypeMessage::ERROR
            )
        }
    }

    /*!
     * Конструктор по умолчанию
     * требует задания начального состояния
     */
    BondState(const State& start_state){
        this->_states.emplace(start_state.get_Mark(), std::make_shared<State>(start_state));
        this->_current_state = this->_states.at(start_state.get_Mark());
    }
};

#include "BondState.tpp"
#endif