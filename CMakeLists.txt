#set(GLISS_SQUARE_SRC  *.cpp)
#set(gliss_square_headers *.h *.hpp)

file(GLOB CoupledOscillators_src coupled_oscillators.cpp stiffness_matrix.cpp)
file(GLOB CoupledOscillators_headers coupled_oscillators.hpp)

#message(EasyBMP_headers="${gliss_square_headers}")

add_library(CoupledOscillators ${CoupledOscillators_src} ${CoupledOscillators_headers})
#add_library(gliss_square::gliss_square ALIAS gliss_square)

#target_include_directories(CoupledOscillators 
#    PRIVATE
#    include
#    ${BELA_ROOT} ${BELA_ROOT}/include 
#    ${CMAKE_SYSROOT}/usr/local/include
#)

#target_link_libraries(CoupledOscillators
#    PRIVATE
#    ${MATHNEON_LIB}
#    ${BELA_ROOT}/lib/libbelafull.a
#)