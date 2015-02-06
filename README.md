Exam Version Application
========================

This application is a useful tool I've put together to help me in creating versioned exams for the courses I teach/assist.

Unfortunately, educators must sometimes resign themselves to the use of multiple choice exams (particularly at universities with large traditional lecture courses). In these situtations it is often necessary to implement multiple versions of the same test to minimize the occurrance of copying/cheating by students. This small application might be helpful in automating the processes of shuffling not only the order of the questions, but for a particular question, shuffling the order of the possible answers. Further, the application auto-genarates an answer key for the new version.

**Dependencies:** latex (full version to be safe), c++ compiler

The code should compile and run in most unix environments.

##Building the application

After unzipping/etc... the repository to a desired location, the code should be compiled with:
  
``
cd src/ && make
``
  

##Configuring for a new exam

Essentially, the idea is that by writing the exam in latex we can script the process of shuffling the questions and compiling a PDF document. The repository comes with everything needed as an example. There are five questions, two with a figures (in the **fig** directory), included in the **que** directory as illustrations.

- `tex/source/preamble.tex`:
  The preamble to the latex document should go in this file. The repository comes with a preamble already included that has most of what would be needed. The essentials are at the top and additional packages for itemization of the questions and captioning of figures are already included. My recommendation is not to change these unless you want to add something to the *header* or *footer* of the exam that needs a package not already included here.

- `tex/source/header.tex`:
Anything that should go at the top of the exam (e.g., name, title page, etc...).

- `tex/source/footer.tex`:
Anything that should go at the end of the exam (e.g., equation sheet, etc ...).

- `que/`:
Type all your questions in the format illustrated in the example questions as .exam files. The relative path to a figure, its intended width, and its label can be included at the top as illustrated. You can choose to select at random a subset of these questions when you configure for a new exam.

Once your *header*, *footer*, *.exam* files, and the any additions to the *preamble* have been written, run `make version` to create the new version. This rewrites the source code to reflect your new exam version.

## Building the exam

After running `make version`, you can run `make exam` to run *pdflatex* and generate the new exam. This deposits the test as a PDF in the **out** directory with a name reflecting that given in the `make version` routine. There is also a file of the same name as a txt file with the answers to the test. pdflatex is run twice on the document because if you include figures, after the shuffle process, labels can be mismatched and it needs to be run again to fix references. If this step 'stalls', enter 'q' or ctrl-c to kill it and check the *logfile* in tex/output/logfile. pdflatex likely encountered an error from your markup in the .exam files.

... more documentation is forthcoming.
