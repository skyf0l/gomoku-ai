TARGET = pbrain-gomoku-ai pbrain-gomoku-ai.exe
NAME = pbrain-gomoku-ai

# MAKEFLAGS += --silent

MAX_JOBS = $(shell nproc)

SRC = ./src/main.cpp \
	./src/random_brain.cpp \
	./src/core/protocol.cpp \
	./src/core/brain_core.cpp \
	./src/ppay/ppay_brain.cpp \
	./src/ppay/solver.cpp

OBJ = $(SRC:.cpp=.o)
CXXFLAGS = -Iinclude

all: build

build:
	cmake -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release --parallel $(MAX_JOBS)
.PHONY: build

debug:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --config Debug --parallel $(MAX_JOBS)

tests_run:
	cd tests && cmake -B build
	cd tests && cmake --build build --parallel $(MAX_JOBS)
	cd tests/build && ctest || true

clean:
	rm -rf $(OBJ)
	rm -rf build
	rm -rf tests/build
	rm -rf *.log
	rm -rf vgcore.*

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re