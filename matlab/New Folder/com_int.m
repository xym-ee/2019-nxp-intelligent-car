%串口中断函数：
function com_int(Serial_Com)        % 创建函数  
  global s;                 % 全局变量，你删除它发现程序仍然可以运行，因为可以通过参数传递传入串口。这只是为了以后更改串口的预留。
  s = Serial_Com;      %传递串口句柄给s  
  set(s,'BytesAvailableFcnMode','Terminator'); %设置中断触发方式，这个除了可以百度了解外，还可以通过get()来查询他所有的可选模式。
  set(s,'Terminator','H');  %设置触发中断的字符为"H"，这里也可以设为别的。只要和Arduino那边一致即可
  s.BytesAvailableFcn =@ReceiveCallback;       % 定义中断响应函数对象，当串口中断发生时，执行ReceiveCallback函数。 
end 