http://www.apuebook.com/apue3e.html

下载Source Code，解压文件，复制include/apue.h，lib/error.c到默认include目录（centos /usr/include），修改apue.h 在最后#endif 前面加上 #include"error.c"