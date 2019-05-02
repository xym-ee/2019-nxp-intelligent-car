clc;
clear;

image0 = imread('qidian.bmp');     %读取图片
image1 = image0;                      %存放滤波后图片      
image2 = image0;                       %存放二值化后图片
[h,w]=size(image0);
%权重矩阵
quanzhong = [ 0.0947416,0.118318, 0.0947416;0.118318,0.147761,0.118318;0.0947416,0.118318, 0.0947416 ];

%显示原图
subplot(2,2,1);
imshow(image0);
title('原图');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1个像素的高斯滤波处理
for i = 2:h-1
    for j = 2:w-1
        a = double(image0(i-1:i+1 , j-1:j+1)) .* quanzhong;
        image1(i,j) = sum(sum(a));
    end
end

subplot(2,2,2);
imshow(image1);  
title('高斯滤波');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%OSTU
%变量初始化
pixelCount = [];        %像素计数
pixelPro =[];               %像素比例
pixelCount(256) = 1;
pixelPro(256) = 1;

%相同灰度像素点统计
for i = 1:h
    for j = 1:w
         k = image0(i,j) +1 ;  %matlab数组1开头
        pixelCount(k) = (pixelCount(k)) +1;
    end
end

%像素点比例计算
for i = 1:256
    pixelPro(i) = pixelCount(i)/(188*120);
end

% 变量定义
P1 = 0;
P2 = 0;
m1 = 0;
m2 = 0;
u0 = 0;
u1 = 0;
mG = 0;
Sigma = 0;
SigmaMax = 0;

%遍历0-255灰度，计算方差最大的灰度值
for i = 1:256
    P1 = 0;
    P2 = 0;
    m1 = 0;
    m2 = 0;
    for j = 1:256
        if j<=i             %背景部分
            P1 = P1 + pixelPro(j);                  %背景累积和
            m1 = m1 +  j*pixelPro(j);              %背景累计均值
        else                %前景部分
            P2 = P2 + pixelPro(j);                  %前景累积和
            m2 =m2 + j*pixelPro(j);                 %前景累计均值
        end
    end
    
    
    u0 = m1 / P1;        %第一类平均灰度
    u1 = m2 / P2;        %第二类平均灰度
    mG = m1 + m2;       %全局灰度
    Sigma = P1 * (u0 - mG)*(u0 - mG) + P2 * (u1 - mG)*(u1 - mG); %计算类间方差
    
    if Sigma > SigmaMax          %
        SigmaMax = Sigma;
        threshold = i;
    end
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%二值化处理
for i = 1:h
    for j =1:w
        if(image1(i,j) >threshold)
            image2(i,j) = 255;
        else
            image2(i,j) = 0;
        end
    end
end

subplot(2,2,4);
imshow(image2);     
title('动态阈值处理');


mid = w/2;
%中线寻找
for k = 1:h
    i= h-k+1;
	
	left = 1;
    for j = 1:mid       %往左边找
        jj=mid-j+1;
            if image2(i,jj)==0
                left = jj;
                break
            end
    end

    right = w;
	for j = mid:w       %往右边找
         if image2(i,j)==0
            right = j;
            break
         end
	end  

    mid = uint8((left+right)/2);
    image2(i,mid) = 0; %画出中线
end
    
subplot(2,2,3);
imshow(image2);     
title('中线'); 
            
            
                



            
            


        