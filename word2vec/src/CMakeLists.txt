
include_directories(
	include
)

add_executable(word2vec main.cpp)
target_link_libraries(word2vec ${LIBS})