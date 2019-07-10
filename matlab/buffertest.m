clc;clear;
%7/8 道路类型判断

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%收到的图像从这里导入
IMG_SIZE = 2;       %图像尺寸，1-94*60  2-188*120   4-376*240
%cameraReceiver = imread('3.bmp');
bin_image = textread('o.txt','%c');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
subplot(2,2,1);
imshow(cameraReceiver);

%实际使用的图像数组创建
IMG_HIGH = 120*IMG_SIZE; 
IMG_WIDTH = 188*IMG_SIZE; 

Image(IMG_HIGH,IMG_WIDTH) = uint8(0);

%如果读进来的是bmp数据，检查变量是否定义
if exist('cameraReceiver','var')
    %模拟从缓冲区取数据
    [IMG_BUF_HIGH,IMG_BUF_WIDTH] = size(cameraReceiver);
    %转换成C中地址的形式
    fullCameraBufferAddr(IMG_BUF_HIGH*IMG_BUF_WIDTH) = uint8(0);
    for i = 1:IMG_BUF_HIGH
        fullCameraBufferAddr(IMG_BUF_WIDTH*(i-1)+1:IMG_BUF_WIDTH*i) = cameraReceiver(i,:);
    end

    %！从缓冲区地址取出图像
    for i = 1:IMG_HIGH
        for j = 1:IMG_WIDTH
            if i==1 || i==IMG_HIGH ||j==1 || j==IMG_WIDTH
                 Image(i,j) = fullCameraBufferAddr( (i-1)*IMG_WIDTH+j );
            else
                 Image(i,j) = ...
                      fullCameraBufferAddr( (i-1-1)*IMG_WIDTH+j-1 )/9 +  fullCameraBufferAddr( (i-1-1)*IMG_WIDTH+j )/9 +  fullCameraBufferAddr( (i-1-1)*IMG_WIDTH+j+1 )/9 +...
                      fullCameraBufferAddr( (i-1)*IMG_WIDTH+j-1 )/9 +  fullCameraBufferAddr( (i-1)*IMG_WIDTH+j )/9 +  fullCameraBufferAddr( (i-1)*IMG_WIDTH+j+1 )/9 +...
                      fullCameraBufferAddr( (i-1+1)*IMG_WIDTH+j-1 )/9 +  fullCameraBufferAddr( (i-1+1)*IMG_WIDTH+j )/9 + fullCameraBufferAddr( (i-1+1)*IMG_WIDTH+j+1 )/9;
            end
        end
    end
    
end
subplot(2,2,2);
imshow(Image);

Image = imread('2.BMP');

%如果读进来的是txt数据，检查变量是否定义
if exist('bin_image','var')
    
	image_data_length = size(bin_image);
    frame_size = IMG_HIGH*IMG_WIDTH/8;
    %计数变量    dec2hex(image(1))    dec2hex(abs(a(1)))
    ImageByteCount = 1;
    %帧头判断 0x55 0x55
    if hex2dec('55') == bin_image(ImageByteCount) && hex2dec('55') == bin_image(ImageByteCount+1)
        ImageByteCount = ImageByteCount + 2; %跳过帧头
        for high = 0:(IMG_HIGH/8-1)                     %缓冲区的高度除8
            for j = 1:IMG_WIDTH        %列数增加，缓冲区的宽度
                for i = 1:8         %转换一个字节
                    PixleTemp = bitget(abs(bin_image(ImageByteCount)),i) * 255;
                    Image(i+high*8,j) = PixleTemp;
                end
                ImageByteCount = ImageByteCount + 1;
            end
        end
    end
end


%%%%%%%%收到的图像已经导入到了Image数组中%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 subplot(2,2,1);
 imshow(Image);
 title('原始图像');


 
%/* 储存赛道信息 */
midline(IMG_HIGH)   = int16(0);
leftline(IMG_HIGH)    = int16(0);
rightline(IMG_HIGH) = int16(0);


%中线校正前的预处理，用最简单的 左加右除以二
mid = int16(IMG_WIDTH/2);
for i = IMG_HIGH:-1:1
	leftline(i) = 1;
    for j = mid:-1:1       %往左边找
            if Image(i,j) == 0
                leftline(i) = j;
                break
            end
    end

    rightline(i) = IMG_WIDTH;
    for j = mid:IMG_WIDTH   %往右边找
         if Image(i,j) == 0
            rightline(i) = j;
            break
         end  
    end

    mid = uint16((leftline(i)+rightline(i))/2);

    if leftline(i) == rightline(i)
        midline(i) = -1;    %%有效的中线已经寻找结束  中线和边线交点
    else
        midline(i) = mid;       
    end
    
end


%算斜率
% 1.选择比较长的边线作为斜率的参考线
%     --1 找车与路的关系
%第一种做法 ，通过比较左侧和右侧赛道边界与视野的交点坐标
% for i = IMG_HIGH:-1:1
%     if leftline(i) ~= 1
%         leftbound = i;
%         break;
%     end
% end
% 
% for i = IMG_HIGH:-1:1
%     if rightline(i) ~= IMG_WIDTH
%         rightbound = i;
%         break;
%     end
% end

%第二种做法 ，直接比较摄像头中点和预处理获得的中点。
if midline(IMG_HIGH/8*3) < int16(IMG_WIDTH/2-4)   %比较中间靠略靠上的点
    disp('路靠左，车靠右');
    %选右侧线段判断路的类型
    k1 = rightline(IMG_SIZE*16) - rightline(IMG_SIZE*32);
    k2 = rightline(IMG_SIZE*32) - rightline(IMG_SIZE*48); 
    
else
    if midline(IMG_HIGH/8*3) > int16(IMG_WIDTH/2 +4)   %比较中间靠略靠上的点
        disp('路靠右，车靠左');
        k1 = leftline(IMG_SIZE*16) - leftline(IMG_SIZE*32);       
        k2 = leftline(IMG_SIZE*32) - leftline(IMG_SIZE*48);
    else
        disp('车与路正');
        k1 = leftline(IMG_SIZE*16) - leftline(IMG_SIZE*32);       
        k2 = leftline(IMG_SIZE*32) - leftline(IMG_SIZE*48);
    end
end

    deltaK = k1 - k2;
    
   %算出了远像素斜率k1,近像素斜率k2,像素斜率的变化率deltaK
   if deltaK >= -4 && deltaK <= 4
       roadfalg = 1;
       disp('直道');
   else
       disp('弯道');
       if k1 < 0
           roadfalg = 2;
           disp('左弯');
       else
           roadfalg = 2;
           disp('右弯');
       end
   end 
    
   % k1,k2,deltaK



%imcolorshow









