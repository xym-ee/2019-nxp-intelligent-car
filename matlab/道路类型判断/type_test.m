clc;
clear;

IMG_HIGH = 240; 
IMG_WIDTH = 376; 

Image = imread('入小环.bmp');

 subplot(2,2,1);
 imshow(Image);
 title('原始图像');
 
 
 
 
 
 
 
 
 %/* 储存赛道信息 */
midline(IMG_HIGH)   = int16(0);
leftline(IMG_HIGH)    = int16(0);
rightline(IMG_HIGH) = int16(0);
