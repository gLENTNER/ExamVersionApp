# build variables
compile = g++
ccflags = 
inc_dir = inc/
lib_dir = lib/
obj_dir = obj/
exe     = bin/build
setup   = bin/setup
view    = open

# output directory for new exams
output_directory = ../out/
exam_name        = <exam name>

# latex directory variables
tex_flags          = --output-directory
tex_build_location = ../tex/output/
tex_src_directory  = ../tex/

# primary build target
link: main libFramework
	${compile} -o ${exe} main.o -L. -lFramework ${ccflags}
	mv *.o *.a ${obj_dir}

# contents of archive (a target and a variable)
Framework:   mt19937.o question.o exam.o
Framework_ = mt19937.o question.o exam.o

# main object has no header file
main: ${lib_dir}main.cc
	${compile} -c ${lib_dir}main.cc ${ccflags}

# library construction template
lib%: %
	ar -rv $@.a ${$*_}
	ranlib $@.a

# all objects compiled with this template
%.o: ${lib_dir}%.cc ${inc_dir}%.hh
	${compile} -c ${lib_dir}$*.cc ${ccflags}

# always run when called
.PHONY: clean exam version latex

# build questions.tex and compile document
exam: build latex

# configure source for new exam version
version:
	@if [ ! -f ${setup} ]; then ${compile} -o ${setup} etc/setup.cc; fi
	@./${setup} ../que/*.exam

# build questions.tex
build:
	@if [ ! -f ${exe} ]; then \
		echo ; echo " You can't run \"make exam\" before running \"make\"!"; fi
	@echo
	@echo " Generating ${exam_name} ... "
	@echo
	@echo " Building  ..."
	./${exe} ../que/*.exam

# compile new latex document, twice for figures
latex:
	@echo
	@echo " Compiling ... "
	@echo " " >> ${tex_build_location}logfile
	@echo "--> ${exam_name} on $(shell date), build 1" \
		>> ${tex_build_location}logfile
	@echo "  " >> ${tex_build_location}logfile
	@echo pdflatex ${tex_flags} ${tex_build_location} ${tex_src_directory}exam.tex 
	@pdflatex ${tex_flags} ${tex_build_location} ${tex_src_directory}exam.tex \
		>> ${tex_build_location}logfile
	@echo "  " >> ${tex_build_location}logfile
	@echo "--> ${exam_name}, build 2" >> ${tex_build_location}logfile
	@echo "  " >> ${tex_build_location}logfile
	@pdflatex ${tex_flags} ${tex_build_location} ${tex_src_directory}exam.tex \
		>> ${tex_build_location}logfile
	@cp ${tex_build_location}exam.pdf ${output_directory}${exam_name}.pdf
	@mv ${output_directory}exam.answers ${output_directory}${exam_name}.txt
	@echo " --> document written to ${output_directory}${exam_name}.pdf"
	@echo " --> answer key written to ${output_directory}${exam_name}.txt"
	@echo " --> debugging messages appended to ${tex_build_location}logfile"
	@echo

# quick view
open:
	${view} ${output_directory}${exam_name}.pdf

# remove build files and executable
clean:
	@rm -f ${obj_dir}* ${exe} ${setup}

veryclean: clean
	@rm -f ${tex_src_directory}{header,footer,questions,preamble}.tex 
	@rm -f ${tex_build_location}logfile
