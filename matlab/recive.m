%串口中断函数：
function H_Serial_interrupt(Serial_Com)        % 创建函数  
  global s;                 % 全局变量，你删除它发现程序仍然可以运行，因为可以通过参数传递传入串口。这只是为了以后更改串口的预留。
  s = Serial_Com;      %传递串口句柄给s  
  set(s,'BytesAvailableFcnMode','Terminator'); %设置中断触发方式，这个除了可以百度了解外，还可以通过get()来查询他所有的可选模式。
  set(s,'Terminator','H');  %设置触发中断的字符为"H"，这里也可以设为别的。只要和Arduino那边一致即可
  s.BytesAvailableFcn =@ReceiveCallback;       % 定义中断响应函数对象，当串口中断发生时，执行ReceiveCallback函数。 
end 

%中断响应函数：
function ReceiveCallback( obj,event)     %创建中断响应函数  
   global s;
   global I;
   global Route;
   a = fscanf(s);       % 接收数据 ，当然你也可以不接收。把串口清掉就可以了。我用这个只是教给你如果让计算机执行Arduino的命令的方法。
   fwrite(s,Route(I));    %写串口，给Arduino命令。我这个暂时没设置停止，你可以加个关于I的if语句判断。
   I = I+1;   % 记录当下运行到矩阵的哪一行  。当然你也可以做点别的事情，这只是一个普通的函数
end  

%主函数了，当然你可以封装，我这里没有这么做。
global s;
global I;
global Route;
s=serial('COM3'); %这是我Arduino占用的com口，你按完驱动后在计算机设备管理器中可以看到usb-serial。那个便是。
fopen(s); %打开串口。确定串口不要被其他东西占用。