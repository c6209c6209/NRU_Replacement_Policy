CC = g++ -lm -std=c++0x

#SRCS = ../*.cpp
RO_SRCS = ./*.cpp

project:
	$(CC) -o $@ $(RO_SRCS)


#RO: $(RO_SRCS)
#	$(CC) -c $(RO_SRCS)

.PHONY: clean

clean:
ifeq ($(OS),Windows_NT)
	del main.exe *.o
else
	rm main *.o
endif