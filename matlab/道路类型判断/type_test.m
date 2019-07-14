clc;
clear;
 
IMG_HIGH = 240; 
IMG_WIDTH = 376; 

Image = imread('右闪左.bmp');

 subplot(2,2,1);
 imshow(Image);
 title('原始图像');
 
 
 img_roadtype = 0;
 %计算赛道边沿测试代码
    leftslide1 = 0;
    leftslide2 = 0;
    leftslide3 = 0;
    rightslide1 = 0;
    rightslide2 = 0; 
    rightslide3 = 0;      
%取三个位置的冗余计算 
for i = IMG_HIGH:-1:50
    if Image(i,10) == 255 && Image(i-1,10) == 0
        leftslide1 = leftslide1 + 1;    
    end
end
for i = IMG_HIGH:-1:50
    if Image(i,15) == 255 && Image(i-1,15) == 0
        leftslide2 = leftslide2 + 1;
    end
end  
for i = IMG_HIGH:-1:50
    if Image(i,20) == 255 && Image(i-1,20) == 0
        leftslide3 = leftslide3 + 1;
    end
end
    
  for i = IMG_HIGH:-1:50
     if Image(i,IMG_WIDTH-10) == 255 && Image(i-1,IMG_WIDTH-10) == 0
        rightslide1 = rightslide1 + 1;
     end
  end
  for i = IMG_HIGH:-1:50
     if Image(i,IMG_WIDTH-15) == 255 && Image(i-1,IMG_WIDTH-15) == 0
        rightslide2 = rightslide2 + 1;
     end
  end
  for i = IMG_HIGH:-1:50
     if Image(i,IMG_WIDTH-20) == 255 && Image(i-1,IMG_WIDTH-20) == 0
        rightslide3 = rightslide3 + 1;
     end
  end
 
  
 if leftslide1 == 2 && rightslide1 ==1
     img_roadtype = 6;
     disp('左侧环岛')
 end
 if leftslide1 == 2 && rightslide1 ==2
     img_roadtype = 3;
     disp('十字路口')
 end
 
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


%先看左右边线的交点在哪里，
for i = IMG_HIGH:-1:1
    if midline(i) == -1
        break;
    end
end


%左右线交点位于视野太靠下，肯定是弯道
if i>90
    disp('弯道');
    if midline(i+1) > IMG_WIDTH/2
        disp('右弯')
    else
        disp('左弯')
    end

else%其他情况
    
    if midline(i+1) < IMG_WIDTH/2
        disp('路靠左，车靠右');
        p_line = rightline;
    else
        disp('路靠右，车靠左');
        p_line = leftline;
    end
    
    %选一侧线段判断路的类型
    k1 = p_line(80) - p_line(120);
    k2 = p_line(120) - p_line(160);  
    deltaK = k1 - k2;
    
   %算出了远像素斜率k1,近像素斜率k2,像素斜率的变化率deltaK
   if deltaK >= -6 && deltaK <= 6
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
end

