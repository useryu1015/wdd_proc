#			-readme-			#
# 功能：自动捕获指定路径下的工程文件，再进行编译&链接
# 变量：
#	include：头文件路径
#	source： 源文件路径
TARGET	:= proc_test
CC	:= gcc
CXX := g++
DEBUG = -g -O2
CFLAGS  = -c $(DEBUG) $(INCS_P) -Wall				#1 设置编译选项：		# 在编译.c文件时	#-I:指定头文件路径 -L:指定库文件路径 -Wl,R:动态链接lib库
LDFLAGS	= -lcrypto -lpthread -lm $(lib) $(LIB_P)  	#2 指定链接器要链接那些库文件	# 在链接.o文件时

OBJ_PATH = ./4.mbuild/objs/

# 			指定项目头文件和源文件路径		 #
include := ./ #../inc
source  := ./ #../src
#			指定不编译的文件				#
src_out := ./2.Tool/generate_json/* ./3.WDD/* ./3.WDD/CPP_TEST/* ./2.Tool/StringTable/* ./2.Tool/shm_test/* ./2.Tool/wdd_shm_test/* #./shm/source/* #./libwebsockets-x86/* #./cpp_bak.c #./realtime/source/rtdb_gdw376.c ./commons/test/*.c ./config/test/*.c ./mqtt/test/*.c ./realtime/test/*.c ./modbus/test/*.c

#			指定库文件和路径				#
lib	:= -lwebsockets -lssl
LIB_P	:= -L./1.RunEnv/lib	# tips: /test_file/lib-->/lib-->/usr/lib-->/usr/local/lib


## 添加链接器-I文件路径
incs := $(shell find ${include} -name "*.h")    # 1、获取当前目录下，所有.h文件
incs := $(dir ${incs} hacks)                    # 2、过滤：只取文件路径，丢弃
incs := $(sort $(incs))                         # 3、过滤相同的单词， 并排序
INCS_P := $(foreach n,$(incs),-I$(n))           # 4、循环追加-I

## 汇编目录下.c源文件
srcs := $(shell find ${source} -name "*.c")     # 预汇编：捕获目录下所有源文件
out := $(shell ls ${src_out})           # 展开需要过滤的源文件
srcs := $(filter-out $(out),$(srcs))    # 过滤源文件
OBJS := $(patsubst %.c, %.o, $(srcs))			# 汇编：替换(编译)所有%.c -> %.o

## 汇编目录下.cpp源文件
cpps := $(shell find ${source} -name "*.cpp")	# 预汇编：捕获目录下所有源文件
out := $(shell ls ${src_out})           # 展开需要过滤的源文件
cpps := $(filter-out $(out),$(cpps))    # 过滤源文件
OBJ_CPP := $(patsubst %.cpp, %.o, $(cpps))		# 汇编：替换(编译)所有%.c -> %.o


# OBJ_CPPS := $(shell mv ${OBJ_CPP} ${OBJ_PATH}; ls ${OBJ_PATH})
# OBJ_CPPS := $(foreach n,$(OBJ_CPPS),${OBJ_PATH}$(n))
# OBJ_CPP := $(shell ls ${OBJ_PATH})
# $(OBJ_PATH)/%.o: $(OBJ_CPP)			# 
# 	$(CXX) $(CFLAGS) -o $@ $<


$(TARGET): $(OBJS) $(OBJ_CPP)
	$(CXX) $^ -o $@ $(LDFLAGS)  

$(OBJ_CPP):%.o : %.cpp			#生成*.o文件
	$(CXX) $(CFLAGS) $< -o $@

$(OBJS):%.o : %.c
	$(CXX) $(CFLAGS) $< -o $@




.PHONY:clean clean_obj
clean:
	rm -f $(TARGET) $(OBJS) $(OBJ_CPP)
clean_obj:
	rm -f $(OBJS)












# 调试
.PHONY:debug debug_as debug_ld
debug:
	@echo "变量": "INCS_P"
	@echo ${INCS_P}
	@echo ${}
	@echo ${srcs}
	@echo ${cpps}
	@echo ${}
	@echo ${OBJS}
	@echo ${OBJ_CPP}
debug_as:
	@echo "汇编器as: "
	@echo $(CC) { $(CFLAGS) } -c -o xx.o xx.c
debug_ld:
	@echo "链接器ld: "
	@echo $(CC) xx.o -o $(TARGET) { $(LDFLAGS) }

