%中断响应函数：
function ReceiveCallback( obj,event)     %创建中断响应函数  
   global s;
   global I;
   global Route;
   a = fscanf(s);       % 接收数据 ，当然你也可以不接收。把串口清掉就可以了。我用这个只是教给你如果让计算机执行Arduino的命令的方法。
   fwrite(s,Route(I));    %写串口，给Arduino命令。我这个暂时没设置停止，你可以加个关于I的if语句判断。
   I = I+1;   % 记录当下运行到矩阵的哪一行  。当然你也可以做点别的事情，这只是一个普通的函数
end  
