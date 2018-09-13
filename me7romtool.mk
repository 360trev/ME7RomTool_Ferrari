##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=me7romtool
ConfigurationName      :=Release
WorkspacePath          :=C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari
ProjectPath            :=C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=hp
Date                   :=13/09/2018
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="me7romtool.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IntermediateDirectory)/needles.c$(ObjectSuffix) $(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/mlhfm.c$(ObjectSuffix) $(IntermediateDirectory)/fixsums.c$(ObjectSuffix) $(IntermediateDirectory)/crc32.c$(ObjectSuffix) $(IntermediateDirectory)/show_tables.c$(ObjectSuffix) $(IntermediateDirectory)/table_spec.c$(ObjectSuffix) $(IntermediateDirectory)/find_dppx.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/utils.c$(ObjectSuffix): utils.c $(IntermediateDirectory)/utils.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/utils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utils.c$(DependSuffix): utils.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utils.c$(ObjectSuffix) -MF$(IntermediateDirectory)/utils.c$(DependSuffix) -MM utils.c

$(IntermediateDirectory)/utils.c$(PreprocessSuffix): utils.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utils.c$(PreprocessSuffix) utils.c

$(IntermediateDirectory)/needles.c$(ObjectSuffix): needles.c $(IntermediateDirectory)/needles.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/needles.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/needles.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/needles.c$(DependSuffix): needles.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/needles.c$(ObjectSuffix) -MF$(IntermediateDirectory)/needles.c$(DependSuffix) -MM needles.c

$(IntermediateDirectory)/needles.c$(PreprocessSuffix): needles.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/needles.c$(PreprocessSuffix) needles.c

$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

$(IntermediateDirectory)/mlhfm.c$(ObjectSuffix): mlhfm.c $(IntermediateDirectory)/mlhfm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/mlhfm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mlhfm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mlhfm.c$(DependSuffix): mlhfm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mlhfm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mlhfm.c$(DependSuffix) -MM mlhfm.c

$(IntermediateDirectory)/mlhfm.c$(PreprocessSuffix): mlhfm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mlhfm.c$(PreprocessSuffix) mlhfm.c

$(IntermediateDirectory)/fixsums.c$(ObjectSuffix): fixsums.c $(IntermediateDirectory)/fixsums.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/fixsums.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fixsums.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fixsums.c$(DependSuffix): fixsums.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fixsums.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fixsums.c$(DependSuffix) -MM fixsums.c

$(IntermediateDirectory)/fixsums.c$(PreprocessSuffix): fixsums.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fixsums.c$(PreprocessSuffix) fixsums.c

$(IntermediateDirectory)/crc32.c$(ObjectSuffix): crc32.c $(IntermediateDirectory)/crc32.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/crc32.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crc32.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crc32.c$(DependSuffix): crc32.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/crc32.c$(ObjectSuffix) -MF$(IntermediateDirectory)/crc32.c$(DependSuffix) -MM crc32.c

$(IntermediateDirectory)/crc32.c$(PreprocessSuffix): crc32.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crc32.c$(PreprocessSuffix) crc32.c

$(IntermediateDirectory)/show_tables.c$(ObjectSuffix): show_tables.c $(IntermediateDirectory)/show_tables.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/show_tables.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/show_tables.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/show_tables.c$(DependSuffix): show_tables.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/show_tables.c$(ObjectSuffix) -MF$(IntermediateDirectory)/show_tables.c$(DependSuffix) -MM show_tables.c

$(IntermediateDirectory)/show_tables.c$(PreprocessSuffix): show_tables.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/show_tables.c$(PreprocessSuffix) show_tables.c

$(IntermediateDirectory)/table_spec.c$(ObjectSuffix): table_spec.c $(IntermediateDirectory)/table_spec.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/table_spec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/table_spec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/table_spec.c$(DependSuffix): table_spec.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/table_spec.c$(ObjectSuffix) -MF$(IntermediateDirectory)/table_spec.c$(DependSuffix) -MM table_spec.c

$(IntermediateDirectory)/table_spec.c$(PreprocessSuffix): table_spec.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/table_spec.c$(PreprocessSuffix) table_spec.c

$(IntermediateDirectory)/find_dppx.c$(ObjectSuffix): find_dppx.c $(IntermediateDirectory)/find_dppx.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/hp/Documents/GitHub/ME7RomTool_Ferrari/find_dppx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/find_dppx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/find_dppx.c$(DependSuffix): find_dppx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/find_dppx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/find_dppx.c$(DependSuffix) -MM find_dppx.c

$(IntermediateDirectory)/find_dppx.c$(PreprocessSuffix): find_dppx.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/find_dppx.c$(PreprocessSuffix) find_dppx.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


