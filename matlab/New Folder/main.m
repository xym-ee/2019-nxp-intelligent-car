%主函数了，当然你可以封装，我这里没有这么做。
global s;
global I;
global Route;
s = serial('COM3'); %这是我Arduino占用的com口，你按完驱动后在计算机设备管理器中可以看到usb-serial。那个便是。

set(s, 'BaudRate', 115200, 'Parity', 'none', 'DataBits',...
        8, 'StopBits', 1,'DataTerminalReady','off',...
        'RequestToSend','off');

%fopen(s); %打开串口。确定串口不要被其他东西占用。
%fclose(s);