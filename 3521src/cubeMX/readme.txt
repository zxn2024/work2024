说明
使用cubemx的项目，把cubeMX工程放在此文件夹，以便后续使用cubeMX维护。

------------------------
CubeMX生成文件移植说明

1.Core
去掉main.c和freertos.c,这两个文件由项目开发者自己实现.
其他文件直接拷贝到工程目录对应文件夹下

2.Drivers
将其中内容直接拷贝到工程目录libs对应文件夹下

3.Middlewares
将其中内容直接拷贝到工程目录Middlewares对应文件夹下

其他:
USB设备应用层代码,拷贝到app/modules文件夹下