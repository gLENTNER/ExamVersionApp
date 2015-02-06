Exam Version Application
========================

This application is a useful tool I've put together to help me in creating versioned exams for the courses I teach/assist.

Unfortunately, educators must sometimes resign themselves to the use of multiple choice exams (particularly at universities with large traditional lecture courses). In these situtations it is often necessary to implement multiple versions of the same test to minimize the occurrance of copying/cheating by students. This small application might be helpful in automating the processes of shuffling not only the order of the questions, but for a particular question, shuffling the order of the possible answers. Further, the application auto-genarates an answer key for the new version.

**Dependencies:** latex (full version to be safe), c++ compiler

The code should compile and run in most unix environments.

##Building the application

After unzipping/etc... the repository to a desired location, the code should be compiled with:
  
  `cd src/ && make
  

##Configuring for a new exam

Essentially, the idea is that by writing the exam in latex we can script the process of shuffling the questions and compiling a PDF document. The repository comes with everything needed as an example. There are three questions, one with a figure, included in the `que/` directory as illustrations.

- `tex/source/preamble.tex`:
  The preamble to the latex document should go in this file. The repository comes with a preamble already included that has most of what would be needed. The essentials are at the top and additional packages for itemization of the questions and captioning of figures are already included. My recommendation is not to change these unless you want to add something to the top of the exam that needs a package not already included here.

...under construction
