CC      = gcc
LD      = gcc
CFLAGS  = -O2 -Wall -Wextra -g
CFLAGS += -Wno-unused-parameter -Wno-unused-function -Wno-unused-result
INCLDS  = -I $(INC_DIR)
BIN_DIR = bin
BLD_DIR = build
DOC_DIR = docs
INC_DIR = includes
OUT_DIR = out
SRC_DIR = src
TST_DIR = scripts
TARGETS = cl sv

.DEFAULT_GOAL = all

.PHONY: $(TARGETS) all check checkdirs clean doc fmt lint test

all: checkdirs $(TARGETS)

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(INCLDS) $(CFLAGS) $< -o $@

cl: $(BLD_DIR)/parsed_line.o
	$(CC) $(INCLDS) $(CFLAGS) -o $(BIN_DIR)/$@ $(SRC_DIR)/client.c $^

sv: $(BLD_DIR)/server_funcs.o
	$(CC) $(INCLDS) $(CFLAGS) -o $(BIN_DIR)/$@ $(SRC_DIR)/server.c $^

start_sv: all
	./$(BIN_DIR)/sv

start_cl: all
	./$(BIN_DIR)/cl

stop:
	kill -s SIGTERM $(shell pidof sv)

fmt:
	@echo "C and Headers files:"
	@-clang-format -style="{BasedOnStyle: Google, IndentWidth: 4}" -verbose -i \
		$(SRC_DIR)/* $(INC_DIR)/*
	@echo ""
	@echo "Shell files:"
	@shfmt -l -w -i 2 .

lint:
	@splint -retvalint -hints -I $(INC_DIR) \
		$(SRC_DIR)/*

check: all
	@echo "Write something to check!"

doc:
	@doxygen $(DOC_DIR)/Doxyfile

checkdirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(DOC_DIR)
	@mkdir -p $(OUT_DIR)
