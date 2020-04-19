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

CPPFLAGS +=

CXXFLAGS +=
CXXFLAGS += $(DEBUG)

DEPFLAGS += -MT $@ -MMD -MP -MF $(DEPSDIR)/$*.Td

LDFLAGS += -Wl,-E

SRCSDIR = srcs
TESTDIR = test
OBJSDIR = objs
LOGSDIR = logs
DEPSDIR = .deps
OUTDIR = .

ERRLOG = 2> $(LOGSDIR)/$(patsubst $(OBJSDIR)%,%,$(@D))/$(shell basename $@).log
CLEANLOG = if [ ! -s $(LOGSDIR)/$(patsubst $(OBJSDIR)%,%,$(@D))/$(shell basename $@).log ]; \
		   then $(RM) $(LOGSDIR)/$(patsubst $(OBJSDIR)%,%,$(@D))/$(shell basename $@).log ; fi

MAKEDIRS = $(MKDIR) $(@D) $(patsubst $(OBJSDIR)%,$(LOGSDIR)%,$(@D)) $(patsubst $(OBJSDIR)%,$(DEPSDIR)%,$(@D))
POSTCOMP = mv -f $(DEPSDIR)/$*.Td $(DEPSDIR)/$*.d && touch $@

DEFAULT = "\033[00m"
GREEN = "\033[0;32m"
TEAL = "\033[0;36m"
RED = "\033[0;31m"

NAME = 

SRCS += $(SRCSDIR)/

SRCS += $(SRCSDIR)/exceptions/ADLException.cpp

OBJS = $(patsubst $(SRCSDIR)/%,$(OBJSDIR)/%, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@-$(MKDIR) $(OUTDIR)
	@-$(CXX) -o $(OUTDIR)/$(NAME) $(LDFLAGS) $(OBJS) \
	 $(ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $@ $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $@ $(DEFAULT)
	@-$(CLEANLOG)

$(OBJSDIR)/%.o: $(SRCSDIR)/%.cpp $(DEPSDIR)/%.d
	@-$(MAKEDIRS)
	@-$(CXX) -c $< -o $@ $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) \
	 $(ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $< $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $< $(DEFAULT)
	@-$(POSTCOMP)
	@-$(CLEANLOG)

$(DEPSDIR)/%.d: ;

$(DEPSDIR)/:
	@$(MAKEDIRS)

clean: cleandep
	@-$(RM) $(OBJSDIR)
	@-$(ECHO) $(TEAL) "Removing objects files" $(DEFAULT)

cleandep:
	@-$(RM) $(DEPSDIR)
	@-$(ECHO) $(TEAL) "Removing dependencies files" $(DEFAULT)

cleanlog:
	@-$(RM) $(LOGSDIR)
	@-$(ECHO) $(TEAL) "Removing logs files" $(DEFAULT)

distclean: clean cleanlog
	@-$(RM) $(NAME)
	@-$(ECHO) $(TEAL) "Removing binary" $(DEFAULT)

re: distclean all

.PRECIOUS: $(DEPSDIR)/%.d
.PHONY: all clean cleanlog cleandep distclean re

.SUFFIXES:
.SUFFIXES: .cpp .o

include $(patsubst $(SRCSDIR)/%,$(DEPSDIR)/%, $(SRCS:.cpp=.d))
