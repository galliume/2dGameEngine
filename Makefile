build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	-o 2dgameengine.exe \
	-I"C:\Users\Guillaume\Documents\workspace\libsdl\SDL2\include" \
	-L"C:\Users\Guillaume\Documents\workspace\libsdl\SDL2\lib" \
	-I".\lib\lua" \
	-L".\lib\lua" \
	-llua53 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lSDL2

clean:
	del 2dgameengine.exe

run:
	2dgameengine.exe
