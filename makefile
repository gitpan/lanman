#
# project name
#
prj=lanman

#
# output directory for the appropriate build
#
outdir6xx=.\perl.6xx.release
outdir5xx=.\perl.5xx.release
outdir3xx=.\perl.3xx.release

#
# where is the perl appropriate directory (the include header files must reside 
# in $(outdir)\lib\core); for perl 5.003_07 build 3xx this is also the 
# directory where the project is installed in if you call nmake install
#
perldir6xx="c:\program files\perl.6xx"
perldir5xx="c:\program files\perl.5xx"
perldir3xx="c:\program files\perl.3xx"

#
# set the misc directory
#
miscdir=.\misc

#
# set the install directory (to install from)
#
instdir=.\inst_dir

#
# set the zip directory (not commonly used)
#
zipdir=.\zip_dir

#
# where is the platform sdk installed?
#
platformsdk=c:\program files\microsoft sdk
#platformsdk=c:\microsoft sdk

#
# compiler, linker, resource compiler, zipper
#
cpp=cl.exe
link=link.exe
rc=rc.exe
zip=pkzip.exe

#
# check configuration
#
!if "$(cfg)" == ""
cfg=perl.6xx
!message
!message No configuration specified; use configuration for perl 5.6 build 6xx
!message
!endif

!if "$(cfg)" != "perl.6xx" && "$(cfg)" != "perl.5xx" && "$(cfg)" != "perl.3xx"
!message
!message Invalid configuration "$(cfg)" specified. Valid configurations are:
!message
!message perl.6xx (for perl 5.6 build 6xx)
!message perl.5xx (for perl 5.005_03 build 5xx)
!message perl.3xx (for perl 5.003_07 build 3xx)
!message
!message If you run nmake, specify the configuration at the command line, e.g.
!message
!message nmake cfg=perl.6xx
!message
!message If you omit the cfg parameter, perl.6xx configuration is used.
!message
!error Invalid configuration specified.
!endif 

!if "$(cfg)" == "perl.6xx"

#
# set parameters to build for (for perl 5.6 build 6xx)
#

# set the output directory
outdir=$(outdir6xx)
# where is perl installed? 
perldir=$(perldir6xx)
# where do I find the perl include files and the perl56.lib?
perlsrcdir=$(perldir)\lib\core
# additional compiler settings
cpp_add_flags=/D PERL_5_6_0
# additional linker settings
link_add_flags=perl56.lib
# dll name to create
dllname=$(prj).dll
# prefix output tar.gz file
targz_prefix=
# tar.gz file directory
targz_dir=mswin32-x86-multi-thread

!elseif "$(cfg)" == "perl.5xx"

#
# set parameters to build for (for perl 5.005_03 build 5xx)
#

# set the output directory
outdir=$(outdir5xx)
# where is perl installed? 
perldir=$(perldir5xx)
# where do I find the perl include files?
perlsrcdir=$(perldir)\lib\core
# additional compiler settings
cpp_add_flags=/D PERL_5005_03
# additional linker settings
link_add_flags=
# dll name to create
dllname=$(prj).dll
# prefix output tar.gz file
targz_prefix=
# tar.gz file directory
targz_dir=x86

!elseif "$(cfg)" == "perl.3xx"

#
# set parameters to build for (for perl 5.003_07 build 3xx)
#

# set the output directory
outdir=$(outdir3xx)
# where is perl installed? 
perldir=$(perldir3xx)
# where do I find the perl include files?
perlsrcdir=$(perldir)\lib\core
# additional compiler settings
cpp_add_flags=/D PERL_5003_07
# additional linker settings
link_add_flags=
# dll name to create
dllname=$(prj).pll
# prefix output tar.gz file
targz_prefix=
# tar.gz file directory
targz_dir=

!endif 

#
# set compiler flags
#
cpp_flags=/nologo /G4 /MT /W3 /GX /O1 /D WIN32 /D _WINDOWS /D MSWIN32 /YX /FD /c	\
	/I $(perlsrcdir) /I "$(platformsdk)\include" /D NDEBUG /Fp$(outdir)\$(prj).pch \
	/Fo$(outdir)/ /Fd$(outdir)/ $(cpp_add_flags)

#
# set linker flags
#
link_flags=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib \
	shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib \
	netapi32.lib mpr.lib /nologo /subsystem:windows /dll /machine:I386 \
	/pdb:$(outdir)\$(prj).pdb /def:.\$(prj).def /out:$(outdir)\$(dllname)	\
	/implib:$(outdir)\$(prj).lib /incremental:no "/libpath:$(platformsdk)\lib"	\
	/libpath:$(perlsrcdir) $(link_add_flags) /opt:nowin98

#
# set resource compiler flags
#
rc_flags=/l 0x407 /fo$(outdir)\resource.res /d NDEBUG

#
# object file names
#
objfiles=$(outdir)\access.obj	\
	$(outdir)\addloader.obj \
	$(outdir)\alert.obj	\
	$(outdir)\browse.obj	\
	$(outdir)\dfs.obj	\
	$(outdir)\domain.obj	\
	$(outdir)\ds.obj	\
	$(outdir)\eventlog.obj	\
	$(outdir)\file.obj	\
	$(outdir)\get.obj	\
	$(outdir)\group.obj	\
	$(outdir)\handle.obj	\
	$(outdir)\lanman.obj	\
	$(outdir)\message.obj	\
	$(outdir)\misc.obj	\
	$(outdir)\plmisc.obj	\
	$(outdir)\policy.obj	\
	$(outdir)\reghlp.obj	\
	$(outdir)\repl.obj	\
	$(outdir)\schedule.obj	\
	$(outdir)\server.obj	\
	$(outdir)\service.obj	\
	$(outdir)\session.obj	\
	$(outdir)\share.obj	\
	$(outdir)\stat.obj	\
	$(outdir)\strhlp.obj	\
	$(outdir)\termserv.obj	\
	$(outdir)\timeofd.obj	\
	$(outdir)\use.obj	\
	$(outdir)\user.obj	\
	$(outdir)\wnetwork.obj	\
	$(outdir)\workst.obj	\
	$(outdir)\wstring.obj

#
#default rule
#
!if "$(cfg)" != "perl.3xx"

all : $(outdir) $(outdir)\$(dllname) $(instdir)\$(cfg)\$(targz_dir) $(instdir)\$(cfg)\$(targz_dir)\win32-$(prj).$(targz_prefix)tar.gz	\
	$(instdir)\$(cfg)\win32-$(prj).ppd

!else

all : $(outdir) $(outdir)\$(dllname) $(instdir)\$(cfg) $(instdir)\$(cfg)\$(dllname)	\
	$(instdir)\$(cfg)\$(prj).pm

!endif

#
# clean up to prepare a complete build
#
clean :
	-@del $(outdir)\*.obj
	-@del $(outdir)\$(prj).*
	-@del $(outdir)\vc60.*
	-@del $(outdir)\resource.res
	-@rmdir /s /q $(instdir)
	-@rmdir /s /q $(zipdir)

#
# create output directory
#
$(outdir) :
	if not exist $(outdir) mkdir $(outdir)

#
# build resource.res file
#
$(outdir)\resource.res : .\resource.rc
	$(rc) $(rc_flags) .\resource.rc

#
# build object files
#
$(outdir)\access.obj : .\access.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\addloader.obj : .\addloader.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\alert.obj : .\alert.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\browse.obj : .\browse.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\dfs.obj : .\dfs.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\domain.obj : .\domain.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\ds.obj : .\ds.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\eventlog.obj : .\eventlog.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\file.obj : .\file.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\get.obj : .\get.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\group.obj : .\group.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\handle.obj : .\handle.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\lanman.obj : .\lanman.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\message.obj : .\message.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\misc.obj : .\misc.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\plmisc.obj : .\plmisc.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\policy.obj : .\policy.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\reghlp.obj : .\reghlp.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\repl.obj : .\repl.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\schedule.obj : .\schedule.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\server.obj : .\server.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\service.obj : .\service.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\session.obj : .\session.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\share.obj : .\share.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\stat.obj : .\stat.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\strhlp.obj : .\strhlp.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\termserv.obj : .\termserv.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\timeofd.obj : .\timeofd.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\use.obj : .\use.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\user.obj : .\user.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\wnetwork.obj : .\wnetwork.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\workst.obj : .\workst.cpp
	$(cpp) $(cpp_flags) $?

$(outdir)\wstring.obj : .\wstring.cpp
	$(cpp) $(cpp_flags) $?

#
# link files
#
$(outdir)\$(dllname) : $(outdir) .\$(prj).def $(outdir)\resource.res $(objfiles) 
	$(link) $(link_flags) $(objfiles) $(outdir)\resource.res

#
# create install directory (6.xx, 5.xx)
#
$(instdir)\$(cfg)\$(targz_dir) :
	if not exist $(instdir)\$(cfg)\$(targz_dir) mkdir $(instdir)\$(cfg)\$(targz_dir)

#
# create install directory (3.xx)
#
$(instdir)\$(cfg) :
	if not exist $(instdir)\$(cfg) mkdir $(instdir)\$(cfg)

#
# copy dll to the install directory for perl 3xx
#
$(instdir)\$(cfg)\$(dllname) : $(outdir)\$(dllname)
	copy $(outdir)\$(dllname) $(instdir)\$(cfg)\$(dllname)

#
# copy pm file to the install directory for perl 3xx
#
$(instdir)\$(cfg)\$(prj).pm : $(miscdir)\$(prj).pm
	copy $(miscdir)\$(prj).pm $(instdir)\$(cfg)\$(prj).pm

#
# create tar.gz file
#
$(instdir)\$(cfg)\$(targz_dir)\win32-$(prj).$(targz_prefix)tar.gz : $(outdir)\$(dllname) $(instdir)\$(cfg)\$(targz_dir)
	if not exist $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj) mkdir $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)
	if not exist $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\.exists copy nul $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\.exists
	if not exist $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\.exists copy nul $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\.exists
	if not exist $(instdir)\$(cfg)\mkgz\blib\lib\win32 mkdir $(instdir)\$(cfg)\mkgz\blib\lib\win32
	if not exist $(instdir)\$(cfg)\mkgz\blib\lib\win32\.exists copy nul $(instdir)\$(cfg)\mkgz\blib\lib\win32\.exists
	if not exist $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\$(dllname) copy $(outdir)\$(dllname) $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\$(dllname)
	if not exist $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\$(prj).lib copy $(outdir)\$(prj).lib $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\$(prj).lib
	if not exist $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\$(prj).exp copy $(outdir)\$(prj).exp $(instdir)\$(cfg)\mkgz\blib\arch\auto\win32\$(prj)\$(prj).exp
	if not exist $(instdir)\$(cfg)\mkgz\blib\lib\win32\$(prj).pm copy $(miscdir)\$(prj).pm $(instdir)\$(cfg)\mkgz\blib\lib\win32\$(prj).pm
	if not exist $(instdir)\$(cfg)\mkgz\blib\html\site mkdir $(instdir)\$(cfg)\mkgz\blib\html\site
	if not exist $(instdir)\$(cfg)\mkgz\blib\html\site\.exists copy nul $(instdir)\$(cfg)\mkgz\blib\html\site\.exists
	if not exist $(instdir)\$(cfg)\mkgz\blib\html\site\lib mkdir $(instdir)\$(cfg)\mkgz\blib\html\site\lib
	if not exist $(instdir)\$(cfg)\mkgz\blib\html\site\lib\.exists copy nul $(instdir)\$(cfg)\mkgz\blib\html\site\lib\.exists
	if not exist $(instdir)\$(cfg)\mkgz\blib\html\site\lib\win32 mkdir $(instdir)\$(cfg)\mkgz\blib\html\site\lib\win32
	if not exist $(instdir)\$(cfg)\mkgz\blib\html\site\lib\win32\.exists copy nul $(instdir)\$(cfg)\mkgz\blib\html\site\lib\win32\.exists
	pod2html --infile=$(instdir)\$(cfg)\mkgz\blib\lib\win32\$(prj).pm --outfile=$(instdir)\$(cfg)\mkgz\blib\html\site\lib\win32\$(prj).html
	if exist .\pod2htmd.x~~ del .\pod2htmd.x~~
	if exist .\pod2htmi.x~~ del .\pod2htmi.x~~
	perl	-e "chdir '$(instdir)\$(cfg)\mkgz'; use Archive::Tar; $$tar = Archive::Tar->new();"	\
		-e "@files=qw/"	\
		-e	"blib"	\
		-e	"blib\arch"	\
		-e	"blib\lib"	\
		-e	"blib\html"	\
		-e	"blib\arch\auto"	\
		-e	"blib\arch\auto\win32"	\
		-e	"blib\arch\auto\win32\$(prj)"	\
		-e	"blib\arch\auto\win32\$(prj)\.exists"	\
		-e	"blib\arch\auto\win32\$(prj)\$(dllname)"	\
		-e	"blib\arch\auto\win32\$(prj)\$(prj).exp"	\
		-e	"blib\arch\auto\win32\$(prj)\$(prj).lib"	\
		-e	"blib\lib\win32"	\
		-e	"blib\lib\win32\.exists"	\
		-e	"blib\lib\win32\$(prj).pm"	\
		-e	"blib\html\site"	\
		-e	"blib\html\site\.exists"	\
		-e	"blib\html\site\lib"	\
		-e	"blib\html\site\lib\.exists"	\
		-e	"blib\html\site\lib\win32"	\
		-e	"blib\html\site\lib\win32\.exists"	\
		-e	"blib\html\site\lib\win32\$(prj).html/;"	\
		-e "$$tar->add_files(@files); $$tar->write('win32-$(prj).tar.gz', 1);"
	copy $(instdir)\$(cfg)\mkgz\win32-$(prj).tar.gz $(instdir)\$(cfg)\$(targz_dir)\win32-$(prj).$(targz_prefix)tar.gz
         
#
# create ppm file
#
$(instdir)\$(cfg)\win32-$(prj).ppd :
	copy $(miscdir)\win32-$(prj).ppd $(instdir)\$(cfg)\win32-$(prj).ppd

#
# install the package
#
!if "$(cfg)" != "perl.3xx"

install : $(instdir)\$(cfg)\$(targz_dir)\win32-$(prj).$(targz_prefix)tar.gz $(instdir)\$(cfg)\win32-$(prj).ppd
	ppm install --location=$(instdir)\$(cfg) win32-$(prj)

!else

$(perldir3xx)\lib\auto\win32\$(prj) :
	if not exist $(perldir3xx)\lib\auto\win32\$(prj) mkdir $(perldir3xx)\lib\auto\win32\$(prj)

$(perldir3xx)\lib\auto\win32\$(prj)\$(dllname) : $(instdir)\$(cfg)\$(dllname)
	copy $(instdir)\$(cfg)\$(dllname) $(perldir3xx)\lib\auto\win32\$(prj)\$(dllname)

$(perldir3xx)\lib\win32\$(prj).pm : $(instdir)\$(cfg)\$(prj).pm
	copy $(instdir)\$(cfg)\$(prj).pm $(perldir3xx)\lib\win32\$(prj).pm

install : $(perldir3xx)\lib\auto\win32\$(prj) $(perldir3xx)\lib\auto\win32\$(prj)\$(dllname)	\
	$(perldir3xx)\lib\win32\$(prj).pm 

!endif

#
# create zip directories
#
$(zipdir)\mswin32-x86-multi-thread :
	if not exist $(zipdir)\mswin32-x86-multi-thread mkdir $(zipdir)\mswin32-x86-multi-thread

$(zipdir)\x86 :
	if not exist $(zipdir)\x86 mkdir $(zipdir)\x86

#
# create tar.gz files
#
$(zipdir)\mswin32-x86-multi-thread\win32-$(prj).tar.gz : $(instdir)\perl.6xx\mswin32-x86-multi-thread\win32-$(prj).tar.gz
	copy $(instdir)\perl.6xx\mswin32-x86-multi-thread\win32-$(prj).tar.gz $(zipdir)\mswin32-x86-multi-thread\win32-$(prj).tar.gz

$(zipdir)\x86\win32-$(prj).tar.gz : $(instdir)\perl.5xx\x86\win32-$(prj).tar.gz
	copy $(instdir)\perl.5xx\x86\win32-$(prj).tar.gz $(zipdir)\x86\win32-$(prj).tar.gz

#
# create pm files
#
$(zipdir)\win32-$(prj).ppd : $(instdir)\perl.5xx\win32-$(prj).ppd
	copy $(instdir)\perl.5xx\win32-$(prj).ppd $(zipdir)\win32-$(prj).ppd

#
# zip the package
#
zip : $(zipdir)\x86 $(zipdir)\mswin32-x86-multi-thread $(zipdir)\x86\win32-$(prj).tar.gz \
	$(zipdir)\mswin32-x86-multi-thread\win32-$(prj).tar.gz $(zipdir)\win32-$(prj).ppd
	copy .\*.cpp .\$(zipdir)\*
	copy .\*.h .\$(zipdir)\*
	copy .\resource.rc .\$(zipdir)\*
	copy .\$(prj).def .\$(zipdir)\*
	copy .\$(miscdir)\$(prj).pm .\$(zipdir)\*
	copy .\$(miscdir)\install.pl .\$(zipdir)\*
	copy .\$(miscdir)\restrict.txt .\$(zipdir)\*
	copy .\$(miscdir)\history.txt .\$(zipdir)\*
	copy .\$(miscdir)\readme.txt .\$(zipdir)\*
	copy .\$(outdir6xx)\$(prj).dll .\$(zipdir)\$(prj).6xx.dll
	copy .\$(outdir5xx)\$(prj).dll .\$(zipdir)\$(prj).5xx.dll
	copy .\$(outdir3xx)\$(prj).pll .\$(zipdir)\$(prj).3xx.pll
	copy .\makefile .\$(zipdir)\makefile
	pkzip -add .\$(zipdir)\$(prj).zip -r -path=relative .\$(zipdir)\*
