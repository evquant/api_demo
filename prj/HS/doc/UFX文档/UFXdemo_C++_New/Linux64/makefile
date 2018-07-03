ufxdemo:ufxdemo.o ufxbusiness.o ufxcallback.o
	g++ -lt2sdk -o $@ $^

ufxdemo.o:ufxdemo.cpp t2sdk_interface.h ufxbusiness.h ufxcallback.h
	g++ -c $<

ufxbusiness.o:ufxbusiness.cpp ufxbusiness.h t2sdk_interface.h ufxcallback.h
	g++ -c $<

ufxcallback.o:ufxcallback.cpp ufxcallback.h t2sdk_interface.h
	g++ -c $<

clean:
	rm *.o
