red = [255 0 0];
green = [0 255 0];
blue = [0 0 255];


ImageColor(IMG_HIGH,IMG_WIDTH,3) = uint8(0);
ImageColor(:,:,1) = Image;
ImageColor(:,:,2) = Image;
ImageColor(:,:,3) = Image;

for i = 1:376
    ImageColor(30,i,:) = blue;
    ImageColor(80,i,:) = blue;
    ImageColor(120,i,:) = blue;
    ImageColor(160,i,:) = blue;
end

for i = 1:240
    ImageColor(i,10,:) = blue;
    ImageColor(i,20,:) = blue;
    ImageColor(i,30,:) = blue;
end

for i = 1:IMG_HIGH
    if midline(i) > 0
        ImageColor(i,midline(i),:) = red;
    end
    if leftline(i) > 0
        ImageColor(i,leftline(i),:) = green;
    end
    if rightline(i) > 0
        ImageColor(i,rightline(i),:) = green;
    end
end

 subplot(2,2,1);
 imshow(Image);
 ImageColor(90,188,:) = blue;
 subplot(2,2,2);
imshow(ImageColor)