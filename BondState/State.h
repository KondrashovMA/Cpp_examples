//
// Created by mk on 05.04.2022.
//

#ifndef PROJECT0_RZA_STATE_H
#define PROJECT0_RZA_STATE_H

#include <functional>
#include <unordered_set>
#include <list>
#include <utility>


class State {
private:

    /*!
    * Номер текущего состояния
    */
    int _mark;

    /*!
    * Номера прошлых состояний
    */
    std::unordered_set<int> _previous_states;

    /*!
    * Номера следующих состояний
    */
    std::unordered_set<int> _next_states;

public:

    /*!
    * Получить номер текущего состояния
    */
    int get_Mark() const;

    /*!
    * Получить номера предыдущих состояний
    */
    std::unordered_set<int>& get_previous_states();

    std::unordered_set<int>& get_next_states();

    /*!
    * Функция текущего состояния
    */
    std::function<void()> state_function;

    /*!
     * Конструктор состояния
     * @param mark
     * @param state_function
     * @param previous_states
     * @param next_states
     */
    template <typename T, typename TContainer>
    State(const T& mark, std::function<void()> state_function,
          const TContainer& previous_states,
          const TContainer& next_states):
          state_function(std::move(state_function)){

        this->_mark = static_cast<int>(mark);

        for (auto& it : previous_states){
            this->_previous_states.template emplace(static_cast<int>(it));
        }

        for (auto& it : next_states){
            this->_next_states.template emplace(static_cast<int>(it));
        }
    };
};


#endif //PROJECT0_RZA_STATE_H
