app = borne.bin

srcExt = cpp
srcDir = src
objDir = obj
binDir = bin

debug = 1

CFlags = -DGLES2 -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wall -Wno-psabi
LDFlags = -rdynamic
libs = GLESv2 EGL openmaxil bcm_host ilclient
libDir = $(SDKSTAGE)/opt/vc/lib/ libs/
inc = $(SDKSTAGE)/opt/vc/include/ $(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads src/OpenUtility


#************************ DO NOT EDIT BELOW THIS LINE! ************************

ifeq ($(debug),1)
	debug=-g
else
	debug=
endif
inc := $(addprefix -I,$(inc))
libs := $(addprefix -l,$(libs))
libDir := $(addprefix -L,$(libDir))
CFlags += -c $(debug) $(inc)
LDFlags += $(libDir) $(libs)
sources := $(shell find $(srcDir) -name '*.$(srcExt)')
srcDirs := $(shell find . -name '*.$(srcExt)' -exec dirname {} \; | sort | uniq)
objects := $(patsubst %.$(srcExt),$(objDir)/%.o,$(sources))

ifeq ($(srcExt),cpp)
	CC = $(CXX)
else
	CFlags += -std=gnu99
endif

.phony: all clean distclean


all: $(binDir)/$(app)

$(binDir)/$(app): buildrepo $(objects)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(objects) $(LDFlags) -o $@

$(objDir)/%.o: %.$(srcExt)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFlags) $< -o $@

clean:
	$(RM) -r $(objDir)

distclean: clean
	$(RM) -r $(binDir)/$(app)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(srcDirs); \
   do \
	mkdir -p $(objDir)/$$dir; \
   done
endef


# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFlags) \
        $1
endef