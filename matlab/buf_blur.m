%从串口返回的赛道图像处理
clc;
clear;

%读取图像
cameraReceiver = imread('188x120.bmp');
[IMG_BUF_HIGH,IMG_BUF_WIDTH] = size(cameraReceiver);%缓冲区188x120
IMG_HIGH = 56; IMG_WIDTH = 93 ; %实际使用94x56

%转换成C中地址的形式
fullCameraBufferAddr(IMG_BUF_HIGH*IMG_BUF_WIDTH) = uint8(0);
Image(56,94) = uint8(0);

%fullCameraBufferAddr为C中数据起始地址
for i = 1:120
	fullCameraBufferAddr(188*(i-1)+1:188*i) = cameraReceiver(i,:);
end


for i = 2:IMG_HIGH
    for j = 2:IMG_WIDTH

        x = 2*188*i;
        y = 2*j;
        Image(i, j) =     ...
        fullCameraBufferAddr( 2*188*(i-1) + 2*(j-1) )/9 + fullCameraBufferAddr( 2*188*(i-1)  + 2*j )/9 + fullCameraBufferAddr( 2*188*(i-1) + 2*(j+1) )/9 +...
        fullCameraBufferAddr( 2*188*(i)    + 2*(j-1) )/9 + fullCameraBufferAddr( 2*188*(i+1)     + 2*(j+1) )/9 + fullCameraBufferAddr( 2*188*(i)     + 2*(j+1) )/9 + ...
        fullCameraBufferAddr( 2*188*(i+1)+ 2*(j-1) )/9 + fullCameraBufferAddr( 2*188*(i+1) + 2*j )/9 + fullCameraBufferAddr( 2*188*(i+1) + 2*(j+1) )/9;
        
        
    end
end



%显示原图
subplot(2,2,1);
imshow(cameraReceiver);
title('原图');

subplot(2,2,2);
imshow(Image);
title('原图');