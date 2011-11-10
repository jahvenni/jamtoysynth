include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(jamtoysynth
    main.cpp
    screen.cpp
    keys.cpp
    touchlistener.cpp
    simulator.cpp
    synth.c
    instrument.c
    audio_output.c
    )
