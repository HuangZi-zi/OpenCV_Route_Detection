# OpenCV_Route_Detection
图像处理与道路标线识别

# 0627
读取图片与视频，完成视角校正

但未能拓展视角校正到整幅图像

# 0706
将相机升高，重新标定了视角校正

完成了图像初步处理

# 0811
放弃视角校正，更新了处理逻辑，相关函数正在编写

开始指令发送相关函数的编写，今日暂未测试

删除了工程的ipch文件夹

# 0812
完成指令发送相关函数的测试

完善了图像处理的函数，测试效果不佳

# 0813
完成基本的测试，现存在一定概率的跟丢问题，且跟踪速度慢

现存在压线的问题

# 0816
放弃扫线的方案，改为图像切割，然后读取标线中心座标

改用面向对象编程

目前完成图像切割和部分中心座标读取函数

# 0817
完成标线中心座标读取

完成控制指令的判断函数，但偶尔仍有误判的情况出现，需要改进

# 0818
改进了指令判断函数

修改了图像处理函数，但仍未能解决地板反光造成的判别失误