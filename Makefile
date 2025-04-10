CXX           = gcc-14

SRC           = llist.cpp llistDump.cpp
SUBMODULE_SRC = Custom-asserts/Color/color_printf.cpp

BUILD_DIR     = ./build/
SRC_DIR       = ./src/
CFLAGS        = -I inc -I Custom-asserts -I Custom-asserts/Color -I Stack/Color-printf -I Debug-macros

TARGET        = Linked-list
OBJECT        = $(patsubst %.cpp, %.o, $(SRC))
BUILD_OBJ     = $(addprefix $(BUILD_DIR), $(OBJECT))

GREEN_TEXT    = \033[1;32m
YELLOW_TEXT   = \033[1;33m
DEFAULT_TEXT  = \033[0m

DED_FLAGS     = -D _DEBUG -ggdb2 -std=c++17 -O0 -Wall -Wextra -Weffc++                                     \
			    -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations                     \
				-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported                      \
				-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral           \
				-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor \
				-Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls \
				-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2        \
				-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types                \
				-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef                     \
				-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros                              \
				-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing                        \
				-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation       \
				-fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer        \
				-Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla

vpath %.o   $(BUILD_DIR)
vpath %.cpp $(SRC_DIR)

.PHONY : clean all

all : $(addprefix $(SRC_DIR), $(SRC))
	clear
	@mkdir -p build
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(TARGET))
	@printf "$(GREEN_TEXT)$(ASSEMBLY_TARGET) COMPILED$(DEFAULT_TEXT)\n"
	$(addprefix $(BUILD_DIR), $(TARGET))

clean :
	@rm -f -r $(addprefix $(BUILD_DIR), *.o) $(TARGET) *.dSYM Llist-dumps/*
	@printf  "$(YELLOW_TEXT)$(TARGET) CLEANED$(DEFAULT_TEXT)\n"

doxy :
	doxygen
