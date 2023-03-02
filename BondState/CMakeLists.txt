SUBWORKDIRLIST(SUBDIRS .)

foreach(child ${SUBDIRS})
    add_subdirectory(${child})
endforeach()

set(target BondState)

set(Library
        PreProcess
        ControlWork
        )

ADD_QUICK_LIBRARY(${target} . "${Library}")

CONFIGURE_TEST(${target} . "")
