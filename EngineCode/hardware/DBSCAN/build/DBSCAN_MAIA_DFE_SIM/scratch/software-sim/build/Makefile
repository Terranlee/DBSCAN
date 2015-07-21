
##
## targets:
##   'precompile' will build all objects (standard)
##   'tests' will build all objects and tests
##

nullstring :=
space := $(nullstring) # a space at the end
MAXCOMPILERDIR_QUOTE:=$(subst $(space),\ ,$(MAXCOMPILERDIR))


CC := $(MAXCOMPILERDIR_QUOTE)/lib/gcc/bin/gcc
CXX := $(MAXCOMPILERDIR_QUOTE)/lib/gcc/bin/g++

LWIP_INCLUDES := -I$(MAXCOMPILERDIR_QUOTE)/lib/lwip_sim \
                 -I$(MAXCOMPILERDIR_QUOTE)/lib/lwip_sim/include \
                 -I$(MAXCOMPILERDIR_QUOTE)/lib/lwip_sim/include/arch \
                 -I$(MAXCOMPILERDIR_QUOTE)/lib/lwip_sim/include/ipv4

# rest of includes defined in Makefile.static_rules
# -I . needed for compilation of .max file
CXXINCLUDES += \
	-I. \
	-I$(MAXCOMPILERDIR_QUOTE)/lib/boost/include \
	-I$(MAXCOMPILERDIR_QUOTE)/lib/maxeleros-sim/include \
	$(LWIP_INCLUDES)

LWIP_LIB := $(MAXCOMPILERDIR_QUOTE)/lib/lwip_sim/lib/liblwip.a

LIBRARIES := \
	-L$(MAXCOMPILERDIR_QUOTE)/lib/boost/lib \
	-lboost_thread -lboost_system -lboost_filesystem \
	-lrt

WARNINGS := #-Wall -Wredundant-decls -Wshadow -Wextra \
            #-Wcast-align -Wunused-macros -fmessage-length=0 #-Weffc++

OBJDIR ?= objdir
BINDIR ?= bindir

PHONY_TARGETS := all clean

LOCAL_RULES_MAKEFILE ?= Makefile.local_rules

include $(LOCAL_RULES_MAKEFILE)

# WARNING: gcc-3.4.6 on redhat 4 with -Os will produce code
# that segfaults or aborts.
# -O0, -O1, -O2 are all fine.
# -D__USE_XOPEN2K8 .... needed for CentOS 6 support
# x86_64-linux-gnu paths are hacks for Ubuntu
# ALT_MAXCOMPILERSIM_* variables will never be set by MaxCompiler and dare for users
#   for forcing a certain set of flags in emergecny scenarios.
CXXFLAGS = -I/usr/include/x86_64-linux-gnu -fPIC -D__USE_XOPEN2K8 -O3 $(WARNINGS) $(CXXINCLUDES) $(CXXDEFINES) $(CXXFLAGS_OPTIONAL) $(ALT_MAXCOMPILERSIM_CXXFLAGS)
CFLAGS   = -I/usr/include/x86_64-linux-gnu -std=c99 -D__USE_XOPEN2K8 -fPIC -O3 $(WARNINGS) $(CINCLUDES) $(CFLAGS_OPTIONAL) $(ALT_MAXCOMPILERSIM_CFLAGS)
LDFLAGS  = -B/usr/lib/x86_64-linux-gnu $(LIBRARIES) -pthread $(LDFLAGS_OPTIONAL) $(ALT_MAXCOMPILERSIM_LDFLAGS)

#
# Target 'all' is recursive, so we first precompile the headers and only
# after the headers are ready, we invoke the normal cpp build.
all: $(OBJDIR) $(HDRDIR) $(HDRS)
	$(MAKE) $(TARGET_BIN)

%.exe: $(OBJS) $(PRECOMPILED_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LWIP_LIB)

%.so: $(OBJS) $(PRECOMPILED_OBJS)
	$(CXX) -fPIC -shared $(LDFLAGS) -o $@ $^ $(LWIP_LIB)


#
# Precompile a header file with a given optimization flag, for the
# -fno-implicit-templates case.
#

# O0 (O0Impl used to precompile headers for the normal kernel files)
%.h.O0Impl: %.h
	$(CXX) -c $(CXXFLAGS) -O0 -DOPTIMIZATION_LEVEL=0 -x c++-header \
		-o $(notdir $(@:.O0Impl=.gch))/O0_$(notdir $(@:.O0Impl=.gch)) $<
%.h.O0: %.h
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O0 -DOPTIMIZATION_LEVEL=0 -x c++-header \
		-o $(notdir $(@:.O0=.gch))/NoTplInst_O0_$(notdir $(@:.O0=.gch)) $<

# O3
%.h.O3: %.h
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O3 -DOPTIMIZATION_LEVEL=3 -x c++-header \
		-o $(notdir $(@:.O3=.gch))/NoTplInst_O3_$(notdir $(@:.O3=.gch)) $<

# O2
%.h.O2: %.h
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O2 -DOPTIMIZATION_LEVEL=2 -x c++-header \
		-o $(notdir $(@:.O2=.gch))/NoTplInst_O2_$(notdir $(@:.O2=.gch)) $<

# O1
%.h.O1: %.h
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O1 -DOPTIMIZATION_LEVEL=1 -x c++-header \
		-o $(notdir $(@:.O1=.gch))/NoTplInst_O1_$(notdir $(@:.O1=.gch)) $<

# Os
%.h.Os: %.h
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -Os -DOPTIMIZATION_LEVEL=s -x c++-header \
		-o $(notdir $(@:.Os=.gch))/NoTplInst_Os_$(notdir $(@:.Os=.gch)) $<


# default rule (checks that stdsimheader.h compiles..)
$(OBJDIR)/%.h.gch: %.h
	rm -rf $(OBJDIR)/$(notdir $@)
	mkdir $(OBJDIR)/$(notdir $@)
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O3 -DOPTIMIZATION_LEVEL=3 -x c++-header \
		-o $(OBJDIR)/$(notdir $@)/NoTplInst_$(notdir $@) $<


#
# compile a source file with a particular optimization/template flag
# these rules will use precompiled headers, if available (see rule above)
#

# O0
$(OBJDIR)/%.NoTplInst.O0.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O0 -DOPTIMIZATION_LEVEL=0 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

$(OBJDIR)/%.O0.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -O0 -DOPTIMIZATION_LEVEL=0 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

# O1
$(OBJDIR)/%.NoTplInst.O1.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O1 -DOPTIMIZATION_LEVEL=1 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

$(OBJDIR)/%.O1.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -O1 -DOPTIMIZATION_LEVEL=1 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

# O2
$(OBJDIR)/%.NoTplInst.O2.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O2 -DOPTIMIZATION_LEVEL=2 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

$(OBJDIR)/%.O2.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -O2 -DOPTIMIZATION_LEVEL=2 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

# O3
$(OBJDIR)/%.NoTplInst.O3.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -O3 -DOPTIMIZATION_LEVEL=3 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

$(OBJDIR)/%.O3.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -O3 -DOPTIMIZATION_LEVEL=3 -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

# Os
$(OBJDIR)/%.NoTplInst.Os.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -fno-implicit-templates -Os -DOPTIMIZATION_LEVEL=s -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

$(OBJDIR)/%.Os.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -Os -DOPTIMIZATION_LEVEL=s -o $(OBJDIR)/$(notdir $@) $<
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

#
# rules below won't use any precompiled headers as OPTIMIZATION_LEVEL is not defined.
#
$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJDIR)/$(notdir $@) $<
	(echo -n $(MAXCOMPILERDIR_QUOTE) ; cat $<) | md5sum | cut -f 1 -d " " > $(OBJDIR)/$(notdir $(@:.o=.md5))
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(OBJDIR)/$(notdir $@) \
		$< > $(OBJDIR)/$(notdir $(@:.o=.d))

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $(OBJDIR)/$(notdir $@) $<
	(echo -n $(MAXCOMPILERDIR_QUOTE) ; cat $<) | md5sum | cut -f 1 -d " " > $(OBJDIR)/$(notdir $(@:.o=.md5))
	$(CC) $(CINCLUDES) -MM -MP -MT $(OBJDIR)/$(notdir $@) $< \
		> $(OBJDIR)/$(notdir $(@:.o=.d))


$(HDRDIR):
	mkdir -p $@

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@


-include $(OBJS:.o=.d)


clean:
	$(RM) -rf $(OBJDIR)
	$(RM) -rf $(BINDIR)
	$(RM) -rf $(TARGET_BIN)
	$(RM) -rf $(HDRDIR)


.PHONY: $(PHONY_TARGETS)
.PRECIOUS: $(OBJS) $(PRECOMPILED_OBJS)
