编译和使用方法
==========================

1. 确保des.c, des.h 和run_des.c 在同级目录下
2. 编译指令: gcc -O3 des.c run_des.c -o run_des.o   

使用
=====
以加密/解密一个/home/user/sample.txt为例

1. 生成一个密钥文件::

    run_des.o -g /tmp/keyfile.key
2. 加密此文件::

    run_des.o -e /tmp/keyfile.key /home/user/sample.txt /home/user/sample.enc
3. 解密此文件::

    run_des.o -d /tmp/keyfile.key /home/user/sample.enc /home/user/sample_decrypted.txt
    
若用户填写密钥，保存为密钥文件即可。

