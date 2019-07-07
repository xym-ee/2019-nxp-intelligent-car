cameraReceiver = imread('IMGtest.bmp');
[IMG_BUF_HIGH,IMG_BUF_WIDTH] = size(cameraReceiver);

IMG_HIGH = 240; IMG_WIDTH = 376 ; %实际使用94x56

%转换成C中地址的形式
fullCameraBufferAddr(IMG_BUF_HIGH*IMG_BUF_WIDTH) = uint8(0);
Image(IMG_HIGH,IMG_WIDTH) = uint8(0);
for i = 1:IMG_BUF_HIGH
	fullCameraBufferAddr(IMG_BUF_WIDTH*(i-1)+1:IMG_BUF_WIDTH*i) = cameraReceiver(i,:);
end

for i = 1:IMG_HIGH
    for j = 1:IMG_WIDTH
        Image(i,j) = fullCameraBufferAddr( (i-1)*IMG_WIDTH+j );
    end
end

imshow(Image)