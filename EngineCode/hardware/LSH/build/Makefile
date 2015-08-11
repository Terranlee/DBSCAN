####################################################
#       Makefile ------- Thomas Chau 2013          #
#       Tested for MaxCompiler 2013.2.2            #
####################################################
# 1)    For SLiC interface only
#       If you are using MaxCompilerRT,
#       use maxfilecompile instead of sliccompile
#       and change LFLAGS -lslic to -lmaxcompilerrt
# 2)    This makefile uses common settings and
#       compilation targets defined in the root folder
# 3)    Usage
#       -----
#       Build and run simulation:  make runsim
#       Kill failed simulation:    make stopsim
#       Debug simulation:          make debug
#       Build DFE:                 make build
#       Run DFE:                   make run
#       Clean binaries:            make clean
#       Clean all generated files: make distclean


# ---- Project settings ----

WHEREISROOT=../..


# Assuming Kernel and Manager class names follow the pattern below:
PRJ=LSH
KERNEL=$(PRJ)Kernel
MANAGER=$(PRJ)Manager


# Using known hardware settings
include $(WHEREISROOT)/Makefile.Maia.hardware


# Using common compillation targets and compiler settings
# Include this makefile when you are doing unity test
#include $(WHEREISROOT)/Makefile.unity.common

# Include this makefile when you are doing a real DBSCAN computation on DFE
include $(WHEREISROOT)/Makefile.lsh.common
