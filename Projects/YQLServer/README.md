# 软件说明

本软件名：`YQLServer.exe`(以下简称`YQLServer`)，用来控制眼千里`CameraTest.exe`程序，实现1~8个CameraTest.exe自动测试的功能。

## 软件功能

软件界面如下：

<img src="https://gitee.com/Innern/typroa-pictures/raw/master/img/image-20230301100256375.png" alt="image-20230301100256375" style="zoom:80%;" />

软件分为四部分：

1. 菜单项，从左到右依次是：退出程序、保存界面设置、加载dccf、复制程序、切光功能、打开/关闭MES、绑定测试盒、开启程序、关闭程序、点亮模组、关闭模组、清除log、界面显示windows样式、界面显示windowsvista样式、界面显示fusion样式。
2. 测试的log显示。
3. 功能设置，包括：测试程序打开的ip和port设置、共有几个测试窗口、输入的编号、调试用的指令、切光延时、开始测试按钮。
4. 连接状态，窗口是否与`YQLServer`进行了连接。

具体功能包括：

1. 打印日志消息；
2. 登录操作，有两种登录模式：管理员和操作员，默认登录模式为操作员。管理员可以修改密码，操作员不可以修改密码。密码保存在数据库中。
3. 保存应用及界面设置。
4. 复制程序配置文件、打开程序、关闭程序、运行程序；
5. 设置界面风格；
6. 绑定测试盒；
7. 打开/关闭MES；
8. 切换光源；

## 功能简单介绍

切光：切光时需要选择正确的COM口。

<img src="https://gitee.com/Innern/typroa-pictures/raw/master/img/image-20230301100956654.png" alt="image-20230301100956654" style="zoom:100%;" />

MES状态设置：MES打开或关闭是针对即将打开的测试程序，而不是针对源程序，即设置了MES状态后，只对`D:/DccmTest/`路径下的程序有效。

绑定测试盒：默认会根据枚举到的测试盒设置绑定到的工装，可以自行选择某个测试盒要绑定的工装。当测试盒重新打开或关闭后，点击刷新可以显示打开的测试盒。选择完毕后点击绑定即可。

<img src="https://gitee.com/Innern/typroa-pictures/raw/master/img/image-20230301101211015.png" alt="image-20230301101211015" style="zoom:100%;" />