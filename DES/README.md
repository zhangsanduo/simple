加解密代码使用方法
---------------

编译指令 gcc des_run.c -o des_run.o

1.加密：

des_run.o -e key msg

2.解密：

des_run.o -d key enc

执行加密后命令行结果为加密后的数据enc

***

拆分代码使用方法
----------------

编译指令: gcc des_process.c -o run_des.o   
1.加密：

run_des.o -e key msg

2.解密：

run_des.o -d key enc

执行加密后命令行结果为面板图片对应的3-18部分。（面板图片在同级目录下）


***

des_example.c 为展示源码（带注释）
------------------------------

***

md5算法
-------
编译命令 gcc md5.c -o md5

使用方法：./md5 012345678