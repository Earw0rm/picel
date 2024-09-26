# .PHONY => target is not a file
# target always by default threated as file. If timestamp between target file and they
# dependencies is different then we build it up. Else we say that they are already up to date
.PHONY:all clean test  renderer_run


render:
	make -C ./ogl_renderer


all: 
	make -C ./ogl_renderer run 




yasyc:
	make -C ./yasyc  

clean:
	make -C ./ogl_renderer clean 
	make -C ./yasyc clean 