CROSS =     #在开发时候，有时候会有嵌入式的选项
CC = $(CROSS)gcc
CXX = $(CROSS)g++
DEBUG = -g -O2   #这里一般设置为空，在编译时候手动指定，例如：make DEBUG="-g -O2"，如果使用release版本就不用这个
CFLAGS = $(DEBUG) -Wall -c -I./common
RM = rm -rf
 
SRCS = cpp_main.cpp   #最笨的方法，将所有的.cpp文件添加到SRCS中
OBJS = $(patsubst %.cpp, %.o, $(SRCS))       #通过*.cpp文件获得同名的*.o文件

SRCC = hello.c
OBJ_GCC = $(patsubst %.c, %.o, $(SRCC))       #通过*.cpp文件获得同名的*.o文件

 
VERSION = 1.0.0.0   #版本信息，这里的版本信息只是用来加在生成可执行文件的后面
TARGET = test.$(VERSION) #组装文件名和版本信息
 
$(TARGET) : $(OBJS) $(OBJ_GCC)			#连接
	$(CXX) $^ -o $@
 
$(OBJS):%.o : %.cpp			#生成*.o文件
	$(CXX) $(CFLAGS) $< -o $@

$(OBJ_GCC):%.o : %.c			#生成*.o文件
	$(CXX) $(CFLAGS) $< -o $@
 
clean:
	$(RM) $(TARGET) *.o 