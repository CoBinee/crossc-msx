#! make -f
#
# makefile - start
#


# option
#

# directory
#

# source file directory
SRCDIR			=	sources

# include file directory
INCDIR			=	sources

# depend file directory
DEPDIR			=	depends

# object file directory
OBJDIR			=	objects

# binary file directory
BINDIR			=	bin

# output file directory
OUTDIR			=	rom

# tool directory
TOOLDIR			=	tools

# vpath search directories
VPATH			=	$(SRCDIR):$(INCDIR):$(DEPDIR):$(OBJDIR):$(BINDIR)

# assembler
#

# assembler command
AS				=	sdasz80

# assembler flags
ASFLAGS			=	-ls -I$(INCDIR) -I.

# c compiler
#

# c compiler command
CC				=	sdcc

# c compiler flags
CFLAGS			=	-mz80 --opt-code-speed -I$(INCDIR) -I.

# linker
#

# linker command
LD				=	sdcc

# linker flags
# LDFLAGS			=	-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x4020 --data-loc 0xc000
LDFLAGS			=	-mz80 --no-std-crt0 --code-loc 0x4020 --data-loc 0xc000

# suffix rules
#
.SUFFIXES:			.s .c .d .rel

# assembler source suffix
.s.rel:
	$(AS) $(ASFLAGS) -o $(OBJDIR)/$@ $<

# c source suffix
.c.rel:
	$(CC) -MM -MP -MF -c $< > $(DEPDIR)/$*.d
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ -c $<


# project files
#

# target name
TARGET			=	CROSSC

# assembler source files
ASSRCS			=	crt0.s \
					main.s System.s \
					C.s \
					pattern.s sprite.s

# c source files
CSRCS			=	App.c \
					Title.c \
					Game.c Map.c Player.c Enemy.c

# object files
OBJS			=	$(ASSRCS:.s=.rel) $(CSRCS:.c=.rel)

# depend files
CDEPS			=	$(CSRCS:.c=.d)


# build project target
#
$(TARGET).com:		$(OBJS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/$(TARGET).ihx $(foreach file,$(OBJS),$(OBJDIR)/$(file))
	$(TOOLDIR)/ihx2rom32k -o $(OUTDIR)/$(TARGET).ROM $(BINDIR)/$(TARGET).ihx

# clean project
#
clean:
	@rm -f $(DEPDIR)/*
	@rm -f $(OBJDIR)/*
	@rm -f $(BINDIR)/*
##	@rm -f makefile.s.depend
##	@rm -f makefile.c.depend

# build depend file
#
depend:
#ifneq ($(strip $(CSRCS)),)
#	$(foreach file,$(CSRCS),$(CC) $(CFLAGS) -MM $(SRCDIR)/$(file)) > makefile.c.depend
#endif

# build resource file
#
resource:
	@$(TOOLDIR)/image2pattern -n patternTable -o sources/pattern.s resources/picture/pattern.png
	@$(TOOLDIR)/image2sprite -n spriteTable -o sources/sprite.s resources/picture/sprite.png

# build tools
#
tool:
	@gcc -o $(TOOLDIR)/bin2s $(TOOLDIR)/bin2s.cpp
	@gcc -o $(TOOLDIR)/ihx2bload $(TOOLDIR)/ihx2bload.cpp
	@gcc -o $(TOOLDIR)/ihx2rom32k $(TOOLDIR)/ihx2rom32k.cpp
	@g++ -o $(TOOLDIR)/image2pattern `sdl2-config --cflags --libs` -lSDL2_image $(TOOLDIR)/image2pattern.cpp
	@g++ -o $(TOOLDIR)/image2screen1 `sdl2-config --cflags --libs` -lSDL2_image $(TOOLDIR)/image2screen1.cpp
	@g++ -o $(TOOLDIR)/image2sprite `sdl2-config --cflags --libs` -lSDL2_image $(TOOLDIR)/image2sprite.cpp
	@g++ -o $(TOOLDIR)/chr2png -lpng $(TOOLDIR)/chr2png.cpp

# phony targets
#
.PHONY:				clean depend

# include depend file
#
-include makefile.depend
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(foreach file,$(CDEPS),$(DEPDIR)/$(file))
endif


# makefile - end
