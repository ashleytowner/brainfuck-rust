GCC="g++"
CXX="c++11"

bfi: main.cpp Machine.cpp
	$(GCC) $^ -o $@ -std=$(CXX)

clean:
	rm bfi
