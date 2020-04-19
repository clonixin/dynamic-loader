#################################################
#                                               #
#  C++ Makefile                                 #
#  Made by Phantomas <phantomas@phantomas.xyz>  #
#                                               #
#################################################

CXX = g++
ECHO = echo -e 
MKDIR = mkdir -p 
RM = rm -rf
SHELL = /bin/sh

CPPFLAGS += -I srcs

TEST_CPPFLAGS += -I tests/srcs

CXXFLAGS += -std=c++17 -fpic
CXXFLAGS += $(DEBUG)

TEST_CXXFLAGS += -fprofile-arcs -ftest-coverage --coverage -fno-builtin -O0

DEPFLAGS += -MT $@ -MMD -MP -MF $(DEPSDIR)/$*.Td
TEST_DEPFLAGS += -MT $@ -MMD -MP -MF $(TEST_DEPSDIR)/$*.Td

LDFLAGS += -Wl,-E -shared

TEST_LDFLAGS += -lcriterion --coverage

SRCSDIR = srcs
OBJSDIR = objs
TESTDIR = tests
LOGSDIR = logs
DEPSDIR = .deps
OUTDIR = lib

TEST_SRCSDIR = $(TESTDIR)/$(SRCSDIR)
TEST_OBJSDIR = $(TESTDIR)/$(OBJSDIR)
TEST_LOGSDIR = $(TESTDIR)/$(LOGSDIR)
TEST_DEPSDIR = $(TESTDIR)/$(DEPSDIR)
TEST_OUTDIR = $(OUTDIR)

ERRLOG = 2> $(patsubst $(OBJSDIR)/%,$(LOGSDIR)/%,$(@D))/$(shell basename $@).log
CLEANLOG = if [ ! -s $(patsubst $(OBJSDIR)/%,$(LOGSDIR)/%,$(@D))/$(shell basename $@).log ]; \
		   then $(RM) $(patsubst $(OBJSDIR)/%, $(LOGSDIR)/%,$(@D))/$(shell basename $@).log ; fi

POSTCOMP = mv -f $(DEPSDIR)/$*.Td $(DEPSDIR)/$*.d && touch $@

TEST_ERRLOG = 2> $(patsubst $(TEST_OBJSDIR)/%,$(TEST_LOGSDIR)/%,$(@D))/$(shell basename $@).log
TEST_CLEANLOG = if [ ! -s $(patsubst $(TEST_OBJSDIR)/%,$(TEST_LOGSDIR)/%,$(@D))/$(shell basename $@).log ]; \
		   then $(RM) $(patsubst $(TEST_OBJSDIR)/%, $(TEST_LOGSDIR)/%,$(@D))/$(shell basename $@).log ; fi

TEST_POSTCOMP = mv -f $(TEST_DEPSDIR)/$*.Td $(TEST_DEPSDIR)/$*.d && touch $@

DEFAULT = "\033[00m"
GREEN = "\033[0;32m"
TEAL = "\033[0;36m"
RED = "\033[0;31m"

NAME = dynamicloader.so

TEST_NAME = test_dynamicloader

SRCS += $(SRCSDIR)/exceptions/ADLException.cpp

OBJS = $(patsubst $(SRCSDIR)/%,$(OBJSDIR)/%, $(SRCS:.cpp=.o))

TEST_SRCS += $(TEST_SRCSDIR)/BasicLoader/test_BasicLoader.cpp
TEST_SRCS += $(TEST_SRCSDIR)/resources/Singleton.cpp
TEST_SRCS += $(TEST_SRCSDIR)/resources/mocks/backends/MockBackend.cpp

TEST_OBJS = $(patsubst $(TEST_SRCSDIR)/%, $(TEST_OBJSDIR)/%, $(TEST_SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@-$(MKDIR) $(OUTDIR)
	@-$(CXX) -o $(OUTDIR)/$(NAME) $^ $(LDFLAGS) \
	 $(ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $@ $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $@ $(DEFAULT)
	@-$(CLEANLOG)

$(OBJSDIR)/%.o: OBJSPATH = $(OBJSDIR)/$(*D)/
$(OBJSDIR)/%.o: LOGSPATH = $(LOGSDIR)/$(*D)/
$(OBJSDIR)/%.o: DEPSPATH = $(DEPSDIR)/$(*D)/
$(OBJSDIR)/%.o: $(SRCSDIR)/%.cpp $(DEPSDIR)/%.d
	@-$(MKDIR) $(OBJSPATH)
	@-$(MKDIR) $(LOGSPATH)
	@-$(MKDIR) $(DEPSPATH)
	@-$(RM) $@
	@-$(CXX) -c $< -o $@ $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) \
	 $(ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $< $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $< $(DEFAULT)
	@-$(POSTCOMP)
	@-$(CLEANLOG)

$(DEPSDIR)/%.d: ;

tags:
	@ctags -R $(SRCSDIR)

clean: cleandep
	@-$(RM) $(OBJSDIR)
	@-$(ECHO) $(TEAL) "Removing objects files" $(DEFAULT)

cleandep:
	@-$(RM) $(DEPSDIR)
	@-$(ECHO) $(TEAL) "Removing dependencies files" $(DEFAULT)

cleanlog:
	@-$(RM) $(LOGSDIR)
	@-$(ECHO) $(TEAL) "Removing logs files" $(DEFAULT)

test_clean: test_cleandep
	@-$(RM) $(TEST_OBJSDIR)
	@-$(ECHO) $(TEAL) "Removing tests objects files" $(DEFAULT)

test_cleandep:
	@-$(RM) $(TEST_DEPSDIR)
	@-$(ECHO) $(TEAL) "Removing tests dependencies files" $(DEFAULT)

test_cleanlog:
	@-$(RM) $(TEST_LOGSDIR)
	@-$(ECHO) $(TEAL) "Removing tests logs files" $(DEFAULT)

test_distclean: test_clean test_cleanlog
	@-$(RM) $(TEST_NAME)
	@-$(ECHO) $(TEAL) "Removing tests binary" $(DEFAULT)

distclean: clean cleanlog test_distclean
	@-$(RM) $(NAME)
	@-$(ECHO) $(TEAL) "Removing binary" $(DEFAULT)

re: distclean all

.PRECIOUS: $(DEPSDIR)/%.d
.PHONY: all clean cleanlog cleandep distclean re tags
include $(patsubst $(SRCSDIR)/%,$(DEPSDIR)/%, $(SRCS:.cpp=.d))

test: $(TEST_NAME)
	@find . -name "*.gcda" -delete
	@$(TEST_OUTDIR)/$(TEST_NAME) --verbose
	@gcovr --exclude=$(TESTDIR)
	@gcovr --exclude=$(TESTDIR) -b --exclude-throw-branches

$(TEST_NAME): CPPFLAGS += $(TEST_CPPFLAGS)
$(TEST_NAME): CXXFLAGS += $(TEST_CXXFLAGS)
$(TEST_NAME): LDFLAGS  += $(TEST_LDFLAGS)
$(TEST_NAME): $(OBJS) $(TEST_OBJS)
	@-$(MKDIR) $(TEST_OUTDIR)
	@-$(CXX) -o $(TEST_OUTDIR)/$(TEST_NAME) $^ $(LDFLAGS) \
	 $(TEST_ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $@ $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $@ $(DEFAULT)
	@-$(TEST_CLEANLOG)

$(TEST_OBJSDIR)/%.o: OBJSPATH = $(TESTDIR)/$(OBJSDIR)/$(*D)/
$(TEST_OBJSDIR)/%.o: LOGSPATH = $(TESTDIR)/$(LOGSDIR)/$(*D)/
$(TEST_OBJSDIR)/%.o: DEPSPATH = $(TESTDIR)/$(DEPSDIR)/$(*D)/
$(TEST_OBJSDIR)/%.o: $(TEST_SRCSDIR)/%.cpp $(TEST_DEPSDIR)/%.d
	@-$(MKDIR) $(OBJSPATH)
	@-$(MKDIR) $(LOGSPATH)
	@-$(MKDIR) $(DEPSPATH)
	@-$(RM) $@
	@-$(CXX) -c $< -o $@ $(TEST_DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) \
	 $(TEST_ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $< $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $< $(DEFAULT)
	@-$(TEST_POSTCOMP)
	@-$(TEST_CLEANLOG)

$(TEST_DEPSDIR)/%.d: ;


.PRECIOUS: $(TEST_DEPSDIR)/%.d
.PHONY: test test_clean test_cleanlog test_cleandep test_distclean

.SUFFIXES:
.SUFFIXES: .cpp .o

include $(patsubst $(TEST_SRCSDIR)/%,$(TEST_DEPSDIR)/%, $(TEST_SRCS:.cpp=.d))
